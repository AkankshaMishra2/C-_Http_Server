#include "../include/Logger.hpp"
#include "../include/utils.hpp"
#include <iostream>
#include <ctime>

std::string Logger::getCurrentTimestamp()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return std::string(buf);
}

void Logger::log(LogLevel level, const std::string& message)
{
	std::string timestamp = "[" + getCurrentTimestamp() + "] ";
	switch (level)
	{
		case DEBUG:
			std::cout << "\033[36m" << timestamp << "[DEBUG] " << message << "\033[0m" << std::endl;
			break;
		case INFO:
			std::cout << "\033[32m" << timestamp << "[INFO] " << message << "\033[0m" << std::endl;
			break;
		case WARN:
			std::cout << "\033[33m" << timestamp << "[WARN] " << message << "\033[0m" << std::endl;
			break;
		case ERROR:
			std::cerr << "\033[31m" << timestamp << "[ERROR] " << message << "\033[0m" << std::endl;
			break;
	}
}

void Logger::debug(const std::string& message) { log(DEBUG, message); }
void Logger::info(const std::string& message)  { log(INFO, message); }
void Logger::warn(const std::string& message)  { log(WARN, message); }
void Logger::error(const std::string& message) { log(ERROR, message); }
