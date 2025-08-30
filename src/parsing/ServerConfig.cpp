#include "../../include/parsing/ServerConfig.hpp"

/*------------------------------- CONSTRUCTOR --------------------------------*/

ServerConfig::ServerConfig() :
m_listen(""),
m_server_name("_"),
m_root(""),
m_index("index.html"),
m_client_max_size(1024),
m_upload(""),
m_autoindex(false),
m_min_client_size(1024),
m_max_client_size(104857600)
{
	const char *directives[] = {
		"listen", "server_name", "root", "index", "client_max_body_size",
		"error_page", "http_methods", "cgi", "upload", "autoindex", "location"
	};
	for (size_t i = 0; i < static_cast<size_t>(sizeof(directives) / sizeof(directives[0])); i++)
		this->m_hasDirective[static_cast<std::string>(directives[i])] = false;
	return ;
}

ServerConfig::ServerConfig(const ServerConfig &copy) : 
m_listen(copy.m_listen),
m_server_name(copy.m_server_name),
m_root(copy.m_root),
m_index(copy.m_index),
m_client_max_size(copy.m_client_max_size),
m_upload(copy.m_upload),
m_autoindex(copy.m_autoindex),
m_min_client_size(copy.m_min_client_size),
m_max_client_size(copy.m_max_client_size),
m_errorsPages(copy.m_errorsPages),
m_httpMethods(copy.m_httpMethods),
m_cgi(copy.m_cgi),
m_locations(copy.m_locations),
m_hasDirective(copy.m_hasDirective)
{
	return ;
}

/*------------------------------- DESTRUCTOR --------------------------------*/

ServerConfig::~ServerConfig()
{
	return ;
}

/*------------------------------- OVERLOAD OPERATOR --------------------------------*/

ServerConfig& ServerConfig::operator=(const ServerConfig& copy)
{
	if (this != &copy)
	{
		this->m_listen = copy.m_listen;
		this->m_server_name = copy.m_server_name;
		this->m_root = copy.m_root;
		this->m_index = copy.m_index;
		this->m_client_max_size = copy.m_client_max_size;
		this->m_errorsPages = copy.m_errorsPages;
		this->m_httpMethods = copy.m_httpMethods;
		this->m_cgi = copy.m_cgi;
		this->m_upload = copy.m_upload;
		this->m_autoindex = copy.m_autoindex;
		this->m_locations = copy.m_locations;
		this->m_hasDirective = copy.m_hasDirective;
	}
	return (*this);
}

/*------------------------------- FUNCTIONS --------------------------------*/

// Function to initialize a std::map of my directives to confirm mandatory directives in a location
void	ServerConfig::initCheckDirective(const std::string &directive, bool multipleDirective)
{
	if (!multipleDirective && this->m_hasDirective[directive])
	{
		errorTypeExt(directive + ": Multiple directive prohibited for this one!", -2);
	}
	this->m_hasDirective[directive] = true;
	return ;
}

void	ServerConfig::checkNeedDirective(void)
{
	if (!this->m_hasDirective["listen"] || !this->m_hasDirective["root"])
		errorTypeExt("At a minimum, the \"listen\" and \"root\" directives are required in the server block!", -2);
}

// ValidateIP and ValidatePort, checks that byte blocks do not exceed limits
void	ServerConfig::validateIP(const std::string &listen)
{
	if (listen[0] == '0' && listen.size() > 1)
		errorTypeExt("Listen: Your IP block must not start with 0 if it contains more than 1 digit: " + listen, -3);
	size_t part = std::atoi(listen.c_str());
	if (part > 255)
		errorTypeExt("Listen: A block of your IP does not match the required typing, here is the block: " + listen, -3);
	return ;
}

void	ServerConfig::validatePort(const std::string &listen)
{
	if (listen[0] == '0' && listen.size() > 1)
		errorTypeExt("Listen: Your IP port must not start with 0 if it contains more than 1 digit: " + listen, -3);
	size_t part = std::atoi(listen.c_str());
	if (part < 1 || part > 65535)
		errorTypeExt("Listen: Your IP port is incorrect (Min -> 1 | Max -> 65535): " + listen, -3);
	return ;
}

// Checks in the m_httpMethods vector if the method argument exists (GET, POST, DELETE)
bool	ServerConfig::isMethodAllowed(const std::string &method)
{
	try {
		for (size_t i = 0; i < 3; i++)
		{
			if (this->getHTTPMethods(i) == method)
				return (true);
		}
	}
	catch (const std::out_of_range &e)
	{
		return (false);
	}
	return (false);
}

//------------------------------- SETTERS --------------------------------*/

// Verification of the IP address typing before initialization.
void	ServerConfig::addListen(const std::string &listen)
{
	std::string listen_tmp = listen;
	size_t countBlock = 0;
	bool isColon = false;
	for (int i = 0; i < static_cast<int>(listen_tmp.length()); i++)
	{
		if (!std::isdigit(listen_tmp[i]) && listen_tmp[i] != '.' && listen_tmp[i] != ':')
			errorTypeExt("Listen: Your IP address is not in the correct format. (Example: 111.111.111.111:8080)", -3);
		if (listen_tmp[i] == '.' && countBlock < 4 && !isColon && i > 0 && i < 4)
		{
			countBlock++;
			std::string tmp = listen_tmp.substr(0, i);
			this->validateIP(tmp);
			listen_tmp = listen_tmp.substr(i + 1, listen_tmp.size());
			i = -1;
		}
		else if (listen_tmp[i] == ':' && countBlock == 3 && i > 0 && i < 4)
		{
			std::string tmp = listen_tmp.substr(0, i);
			this->validateIP(tmp);
			tmp = listen_tmp.substr(i + 1, listen_tmp.size());
			this->validatePort(tmp);
			isColon = true;
		}
	}
	if (!isColon || countBlock != 3)
		errorTypeExt("Listen: Your IP address is not in the correct format. (Example: 111.111.111.111:8080)", -3);
	this->m_listen = listen;
	this->initCheckDirective("listen", false);
	return ;
}

void	ServerConfig::addServerName(const std::string &serverName)
{
	this->m_server_name = serverName;
	this->initCheckDirective("server_name", false);
	return ;
}

void	ServerConfig::addRoot(const std::string &root)
{
	this->m_root = root;
	this->initCheckDirective("root", false);
	return ;
}

void	ServerConfig::addIndex(const std::string &index)
{
	this->m_index = index;
	this->initCheckDirective("index", false);
	return ;
}

void	ServerConfig::addClientSize(std::string client_size)
{
	for (size_t i = 0; i < client_size.length(); i++)
	{
		if (!std::isdigit(client_size[i]))
			errorTypeExt("Client_max_body_size: Only numbers possible (Min: 1024 <-> Max: 104857600)", -3);
	}
	size_t tmp = std::atoi(client_size.c_str());
	if (tmp < this->m_min_client_size || tmp > m_max_client_size)
		errorTypeExt("Client_max_body_size: Number limit = Min: 1024 <-> Max: 104857600", -3);
	this->m_client_max_size = tmp;
	this->initCheckDirective("client_max_body_size", false);
	return ;
}

void	ServerConfig::addErrorsPages(const std::string &code, const std::string &url)
{
	for (size_t i = 0; i < code.length(); i++)
	{
		if (!std::isdigit(code[i]))
			errorTypeExt("Error_page: Only numbers possible (Min: 1024 <-> Max: 104857600)", -3);
	}
	size_t tmp = std::atoi(code.c_str());
	this->m_errorsPages[tmp] = url;
	this->initCheckDirective("error_page", true);
	return ;
}

void	ServerConfig::addHTTPMethods(const std::string &httpMethods)
{
	this->m_httpMethods.push_back(httpMethods);
	return ;
}

void	ServerConfig::addCGI(const std::string &index, const std::string &url)
{
	this->m_cgi[index] = url;
	this->initCheckDirective("cgi", true);
	return ;
}

void	ServerConfig::addUpload(const std::string &upload)
{
	this->m_upload = upload;
	this->initCheckDirective("upload", false);
	return ;
}

void	ServerConfig::addAutoIndex(std::string isAutoIndex)
{
	if (isAutoIndex == "on")
		this->m_autoindex = true;
	else if (isAutoIndex == "off")
		this->m_autoindex = false;
	else
		errorTypeExt("autoindex: Invalid input (on or off)!", -4);
	this->initCheckDirective("autoindex", false);
	return ;
}

void	ServerConfig::addLocation(const LocationConfig &location)
{
	this->m_locations.push_back(location);
	this->initCheckDirective("location", true);
	return ;
}

//------------------------------- GETTERS --------------------------------*/


std::string	ServerConfig::getListen(void) const
{
	return (this->m_listen);
}

std::string	ServerConfig::getHost(void) const
{
	size_t pos = this->m_listen.find(':');
	if (pos != std::string::npos)
		return this->m_listen.substr(0, pos);
	return this->m_listen;
}

size_t ServerConfig::getPort(void) const
{
	size_t pos = this->m_listen.find(':');
	if (pos != std::string::npos)
		return static_cast<size_t>(std::atoi(this->m_listen.substr(pos + 1).c_str()));
	return 80; // default HTTP port
}

std::string	ServerConfig::getServerName(void) const
{
	return (this->m_server_name);
}

std::string ServerConfig::getRoot(void) const
{
	return (this->m_root);
}

std::string ServerConfig::getIndex(void) const
{
	return (this->m_index);
}

size_t ServerConfig::getClientSize(void) const
{
	return (this->m_client_max_size);
}

size_t ServerConfig::getClientMaxBodySize(void) const
{
	return (this->m_client_max_size);
}

std::map<size_t, std::string>::const_iterator ServerConfig::getErrorsPages(size_t code) const
{
	return (this->m_errorsPages.find(code));
}

std::string	ServerConfig::getHTTPMethods(size_t index) const
{
	return (this->m_httpMethods.at(index));
}

std::string	ServerConfig::getCGIHandler(const std::string &index) const
{
	std::map<std::string, std::string>::const_iterator it = this->m_cgi.find(index);
	return (it != this->m_cgi.end() ? it->second : "");
}

std::string	ServerConfig::getUpload(void) const
{
	return (this->m_upload);
}

bool	ServerConfig::getAutoIndex(void) const
{
	return (this->m_autoindex);
}

const std::vector<LocationConfig>& ServerConfig::getLocations() const
{
	return this->m_locations;
}
