#ifndef SERVER_STATS_HPP
# define SERVER_STATS_HPP

# include <string>
# include <ctime>

// Singleton class tracking global server statistics
class ServerStats
{
	public:
		static ServerStats& getInstance();

		void addRequest();
		void addBytesReceived(size_t bytes);
		void addBytesSent(size_t bytes);
		void incrementActiveConnections();
		void decrementActiveConnections();

		std::string getReport() const;

	private:
		ServerStats();
		~ServerStats();
		ServerStats(const ServerStats& copy);
		ServerStats& operator=(const ServerStats& copy);

		time_t m_startTime;
		size_t m_totalRequests;
		size_t m_bytesReceived;
		size_t m_bytesSent;
		int m_activeConnections;
};

#endif
