#include "../../include/parsing/LocationConfigParser.hpp"

/*------------------------------- CONSTRUCTOR --------------------------------*/

LocationConfigParser::LocationConfigParser()
{
	return ;
}

LocationConfigParser::LocationConfigParser(const LocationConfigParser &copy)
{
	*this = copy;
	return ;
}

/*------------------------------- DESTRUCTOR --------------------------------*/

LocationConfigParser::~LocationConfigParser()
{
	return ;
}

/*------------------------------- OVERLOAD OPERATOR --------------------------------*/

LocationConfigParser& LocationConfigParser::operator=(const LocationConfigParser& copy)
{
	(void)copy;
	return (*this);
}

/*------------------------------- FUNCTIONS --------------------------------*/

// Depending on the directive, the function initializes by calling the famous template functions for more simplicity in the process
void	LocationConfigParser::parseLocationDirectives(std::stringstream &ss, LocationConfig &location)
{
	std::string directive;
	ss.seekg(0);
	ss >> directive;
	if (directive == "root")
		parseAddDirectiveSimple(ss, location, &LocationConfig::addRoot, directive);
	else if (directive == "index")
		parseAddDirectiveSimple(ss, location, &LocationConfig::addIndex, directive);
	else if (directive == "upload")
		parseAddDirectiveSimple(ss, location, &LocationConfig::addUpload, directive);
	else if (directive == "autoindex")
		parseAddDirectiveSimple(ss, location, &LocationConfig::addAutoIndex, directive);
	else if (directive == "http_methods")
	{
		parseDirectiveMultipleValues<std::vector<std::string> >(ss, location, &LocationConfig::addHTTPMethods, directive);
		location.initCheckDirective(directive, false);
	}
	else if (directive == "cgi")
		parseDirectiveTwoValues(ss, location, &LocationConfig::addCgi, directive);
	else
		errorTypeExt("Unknown directive in configuration file!", -1);
}
