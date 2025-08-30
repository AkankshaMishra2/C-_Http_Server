#include "../../include/parsing/ConfigParser.hpp"

/*------------------------------- CONSTRUCTOR --------------------------------*/

ConfigParser::ConfigParser()
{
	return ;
}

ConfigParser::ConfigParser(const ConfigParser &copy)
{
	*this = copy;
	return ;
}

/*------------------------------- DESTRUCTOR --------------------------------*/

ConfigParser::~ConfigParser()
{
	return ;
}

/*------------------------------- OVERLOAD OPERATOR --------------------------------*/

ConfigParser& ConfigParser::operator=(const ConfigParser& copy)
{
	if (this != &copy)
	{
		this->m_serverConfig = copy.m_serverConfig;
		this->m_locationConfig = copy.m_locationConfig;
	}
	return (*this);
}

/*------------------------------- FUNCTIONS --------------------------------*/

// First block of introduction of configuration file parsing, which takes care of the server part with error cases.
void	ConfigParser::parseConfigFile(const std::string& configFile)
{
	std::ifstream file(configFile.c_str());
	if (!file)
		errorTypeExt("File access problem or non-existent!", -1);
	std::vector<std::string> stack;
	std::string buff;
	std::string token;
	size_t c_index = 0;
	bool serverOn = false;
	while (std::getline(file, buff))
		stack.push_back(buff);
	file.close();
	for (; c_index < stack.size(); c_index++)
	{
		std::stringstream ss(stack[c_index]);
		ss >> token;
		if (token.empty())
			continue ;
		if (token == "server" && ss >> token && token == "{")
		{
			serverOn = true;
			this->parseServerBlock(stack, c_index, serverOn);
		}
		else
			errorTypeExt(" 1 - Unknown directive in configuration file!", -1);
	}
	if (serverOn)
		errorTypeExt("Closing the missing \"Location\" block", -1);
}

// Second introductory block of the parsing which takes care of either pushing the new
// server during its closing bracket or a redirection in the third introductory part of this parsing.
void	ConfigParser::parseServerBlock(const std::vector<std::string> &stack, size_t &c_index, bool &serverOn)
{
	ServerConfig newServer;
	bool locationOn = false;
	c_index++;
	for (; c_index < stack.size(); c_index++)
	{
		std::stringstream ss(stack[c_index]);
		std::string token;
		ss >> token;
		if (token == "}")
		{
			newServer.checkNeedDirective();
			this->m_servers.push_back(newServer);
			serverOn = false;
			c_index++;
			return ;
		}
		else if (token == "location")
		{
			if (ss >> token && token != "{")
			{
				std::string path = token;
				if (ss >> token && token == "{")
				{
					locationOn = true;
					this->parseLocationBlock(stack, newServer, path, c_index, locationOn);
				}
				else
					errorTypeExt("Opening the missing \"Location\" block", -1);
			}
			else
					errorTypeExt("Token (Directive) empty after Location !", -1);
		}
		else
			this->m_serverConfig.parseServerDirectives(ss, newServer);
	}
	if (locationOn)
		errorTypeExt("Closing the missing \"Location\" block", -1);
}

// Third introductory part which takes care of initializing the rental block until it finds the bracket closure, if it does, then it adds this rental to this server.
void	ConfigParser::parseLocationBlock(const std::vector<std::string> &stack, ServerConfig &server, const std::string &path, size_t &c_index, bool &locationOn)
{
	LocationConfig newLocation;
	newLocation.addPath(path);
	c_index++;
	for (; c_index < stack.size(); c_index++)
	{
		std::stringstream ss(stack[c_index]);
		std::string token;
		ss >> token;
		if (token == "}")
		{
			newLocation.checkNeedDirective();
			server.addLocation(newLocation);
			locationOn = false;
			break;
		}
		else
			this->m_locationConfig.parseLocationDirectives(ss, newLocation);
	}
}

const std::vector<ServerConfig>& ConfigParser::getServers() const
{
	return m_servers;
}
