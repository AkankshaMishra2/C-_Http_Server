#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include <string>
# include "ServerConfigParser.hpp"
# include "LocationConfigParser.hpp"

class ConfigParser
{
	private:
		ServerConfigParser m_serverConfig;
		LocationConfigParser m_locationConfig;
		std::vector<ServerConfig> m_servers;

	public:
		ConfigParser();
		ConfigParser(const ConfigParser &copy);
		~ConfigParser();
		ConfigParser& operator=(const ConfigParser& copy);
		void	parseConfigFile(const std::string& configFile);
		void	parseServerBlock(const std::vector<std::string> &stack, size_t &c_index, bool &serverOn);
		void	parseLocationBlock(const std::vector<std::string> &stack, ServerConfig &server, const std::string &path, size_t &c_index, bool &locationOn);
		const std::vector<ServerConfig>& getServers() const;
};

#endif
