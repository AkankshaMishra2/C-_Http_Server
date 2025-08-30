#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <string>

// A centralized logging utility to enforce consistent output formatting.
class Logger
{
	public:
		enum LogLevel {
			DEBUG,
			INFO,
			WARN,
			ERROR
		};

		static void log(LogLevel level, const std::string& message);
		static void debug(const std::string& message);
		static void info(const std::string& message);
		static void warn(const std::string& message);
		static void error(const std::string& message);

	private:
		// Static class only, prevent instantiation
		Logger();
		Logger(const Logger& copy);
		~Logger();
		Logger& operator=(const Logger& copy);
		
		static std::string getCurrentTimestamp();
};

#endif
