#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <set>
# include <vector>
# include <map>
# include <poll.h>
# include <fcntl.h>
# include <memory>
# include "../parsing/ConfigParser.hpp"
# include "../utils/RateLimiter.hpp"
# include "../utils/TrieRouter.hpp"
# include "Client.hpp"
# include "RequestHandler.hpp"

class WebServ
{
	private:
		std::vector<ServerConfig> m_servers;
		std::vector<int> m_sockets;
		std::map<int, std::vector<ServerConfig*> > m_socketToServer;
		std::vector<std::shared_ptr<Client>> m_clients;
		RequestHandler m_handler;
		RateLimiter m_rateLimiter;
		std::map<ServerConfig*, TrieRouter*> m_routers;
		void	setupSocket(void);
		ServerConfig* resolveServer(const std::string &host, int socket);
		LocationConfig* resolveLocation(const std::string &uri, ServerConfig* server);
		std::string buildPath(const std::string &uri, LocationConfig *location, ServerConfig *server);
		void acceptClient(int serverFd);
		void handleClient(Client* client);
		void removeClient(Client* client);
		int	getStatusError(const std::string &uri, const std::string &method, LocationConfig *location, ServerConfig *server);

	public:
		WebServ(const std::vector<ServerConfig>& servers);
		WebServ(const WebServ &copy);
		~WebServ();
		WebServ& operator=(const WebServ& copy);
		int	createSocket(ServerConfig &server);
		void run();
		// getters
		// getters
		std::vector<ServerConfig*>	getServerBySocket(int socket);
};

#endif
