#include "../../include/HTTP-Core/WebServ.hpp"
#include "../../include/Logger.hpp"
#include "../../include/ServerStats.hpp"

/*------------------------------- CONSTRUCTOR --------------------------------*/

WebServ::WebServ(const std::vector<ServerConfig>& servers) : m_servers(servers), m_handler("./www"), m_rateLimiter(100, 10)
{
	setupSocket();
	for (size_t i = 0; i < m_servers.size(); i++) {
		TrieRouter* router = new TrieRouter();
		const std::vector<LocationConfig>& locs = m_servers[i].getLocations();
		for (size_t j = 0; j < locs.size(); j++) {
			router->insert(locs[j].getPath(), const_cast<LocationConfig*>(&locs[j]));
		}
		m_routers[&m_servers[i]] = router;
	}
}

WebServ::WebServ(const WebServ &copy) : 
m_servers(copy.m_servers),
m_sockets(copy.m_sockets),
m_socketToServer(copy.m_socketToServer),
m_clients(copy.m_clients),
m_handler(copy.m_handler)
{
}

/*------------------------------- DESTRUCTOR --------------------------------*/

WebServ::~WebServ()
{
	m_clients.clear();
	for (size_t i = 0; i < m_sockets.size(); ++i)
		close(m_sockets[i]);
	for(std::map<ServerConfig*, TrieRouter*>::iterator it = m_routers.begin(); it != m_routers.end(); ++it) {
		delete it->second;
	}
}

/*------------------------------- OVERLOAD OPERATOR --------------------------------*/

WebServ& WebServ::operator=(const WebServ& copy)
{
	if (this != &copy)
	{
		this->m_servers = copy.m_servers;
		this->m_socketToServer = copy.m_socketToServer;
	}
	return (*this);
}

/*------------------------------- FUNCTIONS --------------------------------*/

void	WebServ::setupSocket(void)
{
	std::set<std::string> tmp;
	for (size_t i = 0; i < this->m_servers.size(); i++)
	{
		if (tmp.find(this->m_servers[i].getListen()) == tmp.end())
		{
			tmp.insert(this->m_servers[i].getListen());
			int socketFD = this->createSocket(this->m_servers[i]);
			for (size_t k = 0; k < this->m_servers.size(); k++)
			{
				if (this->m_servers[i].getListen() == this->m_servers[k].getListen())
					this->m_socketToServer[socketFD].push_back(&this->m_servers[k]);
			}
		}
	}
}

int	WebServ::createSocket(ServerConfig &server)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		throw std::runtime_error("Socket creation failed");
	
	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		throw std::runtime_error("setsockopt SO_REUSEADDR failed");
	}

	std::string listenStr = server.getListen();
	std::string ip = listenStr.substr(0, listenStr.find(':'));
	size_t port = std::atoi(listenStr.substr(listenStr.find(':') + 1, listenStr.size()).c_str());
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("bind");
		throw std::runtime_error("Bind failed on " + listenStr);
	}
	if (listen(sockfd, 128) == -1)
		throw std::runtime_error("Listen failed");
	// Set non-blocking
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	this->m_sockets.push_back(sockfd);
	Logger::info("Listening on " + listenStr);
	return (sockfd);
}

void WebServ::run()
{
	Logger::info("Starting run loop");
	std::vector<pollfd> fds;
	for (size_t i = 0; i < m_sockets.size(); ++i)
	{
		pollfd pfd;
		pfd.fd = m_sockets[i];
		pfd.events = POLLIN;
		pfd.revents = 0;
		fds.push_back(pfd);
	}

	while (true)
	{
		try {
			// Add client fds
			size_t serverCount = m_sockets.size();
			for (size_t i = 0; i < m_clients.size(); ++i)
			{
				pollfd pfd;
				pfd.fd = m_clients[i]->getFd();
				pfd.events = POLLIN;
				if (m_clients[i]->isRequestComplete() && !m_clients[i]->isResponseSent())
					pfd.events |= POLLOUT;
				pfd.revents = 0;
				fds.push_back(pfd);
			}

			int ret = poll(&fds[0], fds.size(), -1);
			if (ret < 0)
			{
				Logger::error("poll error");
				break;
			}
			if (ret == 0)
			{
				Logger::warn("poll timeout");
				continue;
			}

			size_t fdIndex = 0;
			// Handle server sockets
			for (size_t i = 0; i < serverCount; ++i, ++fdIndex)
			{
				if (fds[fdIndex].revents & POLLIN)
				{
					acceptClient(m_sockets[i]);
				}
			}

			// Handle clients and exact timeouts
			for (size_t i = 0; i < m_clients.size(); ++i, ++fdIndex)
			{
				Client* client = m_clients[i].get();
				
				// 30 Seconds timeout rule
				if (client->hasTimedOut(30) && !client->isResponseSent()) {
					Logger::warn("Client " + client->getIp() + " timed out.");
					client->setResponse(HttpResponse::requestTimeOut());
					client->sendResponse();
					ServerStats::getInstance().addBytesSent(client->getBytesSent());
					removeClient(client);
					--i;
					--fdIndex;
					continue;
				}
				
				if (fds[fdIndex].revents & POLLIN && !client->isRequestComplete())
				{
					client->receiveData();
				}
				if (client->isRequestComplete() && !client->isResponseSent() && (fds[fdIndex].revents & POLLOUT))
				{
					handleClient(client);
					client->sendResponse();
					
					// Aggregate stats when client interaction is finished
					ServerStats::getInstance().addRequest();
					ServerStats::getInstance().addBytesReceived(client->getBytesReceived());
					ServerStats::getInstance().addBytesSent(client->getBytesSent());
					
					removeClient(client);
					--i; // Adjust index
					--fdIndex;
				}
			}

			// Resize fds to server count
			fds.resize(serverCount);
		} catch (const std::exception& e) {
			Logger::error(std::string("Exception in run: ") + e.what());
			break;
		}
	}
}

void WebServ::acceptClient(int serverFd)
{
	struct sockaddr_in clientAddr;
	socklen_t addrLen = sizeof(clientAddr);
	int clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &addrLen);
	if (clientFd < 0)
	{
		Logger::error("accept failed");
		return;
	}
	fcntl(clientFd, F_SETFL, O_NONBLOCK);
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientAddr.sin_addr, ip, INET_ADDRSTRLEN);
	int port = ntohs(clientAddr.sin_port);
	Logger::debug("Accepted client " + std::string(ip) + ":" + toStr(port));
	if (!m_rateLimiter.allowRequest(ip)) {
		Logger::warn("Rate limited IP: " + std::string(ip));
		close(clientFd);
		return;
	}
	std::shared_ptr<Client> client(new Client(clientFd, serverFd, ip, port));
	m_clients.push_back(client);
	ServerStats::getInstance().incrementActiveConnections();
}

void WebServ::handleClient(Client* client)
{
	const HttpRequest& req = client->getRequest();
	// Resolve server and location
	std::string host = req.getHeaders().count("host") ? req.getHeaders().find("host")->second : "";
	ServerConfig* server = resolveServer(host, client->getServerFd());
	if (!server)
		server = &m_servers[0];
	LocationConfig* location = resolveLocation(req.getUri(), server);
	HttpResponse resp = m_handler.handleRequest(req, *server, location);
	client->setResponse(resp);
}

void WebServ::removeClient(Client* client)
{
	for (std::vector<std::shared_ptr<Client>>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		if (it->get() == client)
		{
			m_clients.erase(it);
			ServerStats::getInstance().decrementActiveConnections();
			break;
		}
	}
}

ServerConfig* WebServ::resolveServer(const std::string &host, int socket)
{
	try {
		std::vector<ServerConfig *> serverBlockPerSocket = this->getServerBySocket(socket);
		for (size_t i = 0; i < serverBlockPerSocket.size(); i++)
		{
			if (serverBlockPerSocket[i]->getServerName() == host)
				return (serverBlockPerSocket[i]);
		}
		return (serverBlockPerSocket[0]);
	}
	catch (const std::runtime_error &e)
	{
		throw std::runtime_error("No match found for socket");
	}
}

LocationConfig* WebServ::resolveLocation(const std::string &uri, ServerConfig* server)
{
	if (!server) return NULL;
	if (m_routers.find(server) != m_routers.end()) {
		return m_routers[server]->match(uri);
	}
	return NULL;
}

std::string WebServ::buildPath(const std::string &uri, LocationConfig *location, ServerConfig *server)
{
	if (!location || location->getRoot().empty())
		return (server->getRoot() + uri);
	else if (!location->getRoot().empty())
		return (location->getRoot() + uri);
	return (server->getRoot() + uri);
}

/*------------------------------- GETTERS --------------------------------*/

std::vector<ServerConfig*>	WebServ::getServerBySocket(int socket)
{
	if (this->m_socketToServer.find(socket) != this->m_socketToServer.end())
		return (this->m_socketToServer[socket]);
	throw std::runtime_error("ServerConfig not found for socket");
}

int	WebServ::getStatusError(const std::string &uri, const std::string &method, LocationConfig *location, ServerConfig *server)
{
	if (location && !location->isMethodAllowed(method))
		return (405);
	else if (!location && !server->isMethodAllowed(method))
		return (405);

	std::string finalPath = buildPath(uri, location, server);
	if (access(finalPath.c_str(), F_OK) != 0)
		return (404);   
	if (access(finalPath.c_str(), R_OK) != 0)
		return (403);
	return (200);
}
