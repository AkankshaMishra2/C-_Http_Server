#include "../../include/parsing/ServerConfigParser.hpp"

/*------------------------------- CONSTRUCTOR --------------------------------*/

ServerConfigParser::ServerConfigParser()
{
	return ;
}

ServerConfigParser::ServerConfigParser(const ServerConfigParser &copy)
{
	*this = copy;
	return ;
}

/*------------------------------- DESTRUCTOR --------------------------------*/

ServerConfigParser::~ServerConfigParser()
{
	return ;
}

/*------------------------------- OVERLOAD OPERATOR --------------------------------*/

ServerConfigParser& ServerConfigParser::operator=(const ServerConfigParser& copy)
{
	(void)copy;
	return (*this);
}

/*------------------------------- FUNCTIONS --------------------------------*/

// Depending on the directive, the function initializes by calling the famous template functions for more simplicity in the process
void	ServerConfigParser::parseServerDirectives(std::stringstream &ss, ServerConfig &server)
{
	std::string directive;
	ss.seekg(0);
	ss >> directive;
	if (directive == "listen")
		parseAddDirectiveSimple(ss, server, &ServerConfig::addListen, directive);
	else if (directive == "server_name")
		parseAddDirectiveSimple(ss, server, &ServerConfig::addServerName, directive);
	else if (directive == "root")
		parseAddDirectiveSimple(ss, server, &ServerConfig::addRoot, directive);
	else if (directive == "index")
		parseAddDirectiveSimple(ss, server, &ServerConfig::addIndex, directive);
	else if (directive == "client_max_body_size")
		parseAddDirectiveSimple(ss, server, &ServerConfig::addClientSize, directive);
	else if (directive == "upload")
		parseAddDirectiveSimple(ss, server, &ServerConfig::addUpload, directive);
	else if (directive == "autoindex")
		parseAddDirectiveSimple(ss, server, &ServerConfig::addAutoIndex, directive);
	else if (directive == "error_page")
		parseDirectiveTwoValues(ss, server, &ServerConfig::addErrorsPages, directive);
	else if (directive == "http_methods")
		{
			parseDirectiveMultipleValues<std::vector<std::string> >(ss, server, &ServerConfig::addHTTPMethods, directive);
			server.initCheckDirective(directive, false);
		}
	else if (directive == "cgi")
		parseDirectiveTwoValues(ss, server, &ServerConfig::addCGI, directive);
	else if (directive.empty())
		return ;
	else
		errorTypeExt("Unknown directive in configuration file!", -1);
}
