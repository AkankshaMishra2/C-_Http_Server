#ifndef SERVERCONFIGPARSER_HPP
# define SERVERCONFIGPARSER_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <vector>
# include <map>
# include <fstream>
# include <sstream>
# include "../utils.hpp"
# include "ServerConfig.hpp"
# include "GenericConfigParser.hpp"

class ServerConfigParser : public GenericConfigParser<ServerConfig>
{
	private:
		std::vector<ServerConfig> m_servers;
	public:
		ServerConfigParser();
		ServerConfigParser(const ServerConfigParser &copy);
		~ServerConfigParser();
		ServerConfigParser& operator=(const ServerConfigParser& copy);
		void	parseServerDirectives(std::stringstream &ss, ServerConfig &server);
};

#endif
