#include "include/utils.hpp"
#include "include/Logger.hpp"
#include "include/parsing/ConfigParser.hpp"
#include "include/HTTP-Core/WebServ.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		Logger::error("Usage: " + std::string(argv[0]) + " <config_file>");
		return 1;
	}

	try
	{
		ConfigParser parser;
		parser.parseConfigFile(argv[1]);
		std::vector<ServerConfig> servers = parser.getServers();

		WebServ webserv(servers);
		webserv.run();
	}
	catch (const std::exception &e)
	{
		Logger::error(std::string("Main Error: ") + e.what());
		return 1;
	}

	return 0;
}
