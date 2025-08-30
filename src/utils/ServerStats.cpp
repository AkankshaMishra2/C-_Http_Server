#include "../include/ServerStats.hpp"
#include "../include/utils.hpp"

// Initialize singleton instance
ServerStats& ServerStats::getInstance()
{
	static ServerStats instance;
	return instance;
}

ServerStats::ServerStats() : 
	m_totalRequests(0), 
	m_bytesReceived(0), 
	m_bytesSent(0), 
	m_activeConnections(0)
{
	time(&m_startTime);
}

ServerStats::~ServerStats() {}

void ServerStats::addRequest() { m_totalRequests++; }
void ServerStats::addBytesReceived(size_t bytes) { m_bytesReceived += bytes; }
void ServerStats::addBytesSent(size_t bytes) { m_bytesSent += bytes; }
void ServerStats::incrementActiveConnections() { m_activeConnections++; }
void ServerStats::decrementActiveConnections() { if (m_activeConnections > 0) m_activeConnections--; }

std::string ServerStats::getReport() const
{
	time_t now;
	time(&now);
	double uptime = difftime(now, m_startTime);

	std::string report = "<html><head><title>Server Statistics</title>";
	report += "<style>";
	report += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; margin: 40px; background-color: #f4f4f9; color: #333; }";
	report += "h1 { color: #2c3e50; border-bottom: 2px solid #3498db; padding-bottom: 10px; }";
	report += ".stat-box { background: white; border-radius: 8px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); padding: 20px; margin-bottom: 20px; }";
	report += ".stat-row { display: flex; justify-content: space-between; border-bottom: 1px solid #eee; padding: 10px 0; }";
	report += ".stat-label { font-weight: bold; color: #555; }";
	report += ".stat-value { color: #e74c3c; font-weight: bold; }";
	report += "</style></head><body>";
	
	report += "<h1>akanksha-serv/1.0 - Live Statistics</h1>";
	report += "<div class='stat-box'>";
	
	report += "<div class='stat-row'><span class='stat-label'>Uptime:</span><span class='stat-value'>" + toStr(uptime) + " seconds</span></div>";
	report += "<div class='stat-row'><span class='stat-label'>Total Requests Handled:</span><span class='stat-value'>" + toStr(m_totalRequests) + "</span></div>";
	report += "<div class='stat-row'><span class='stat-label'>Total Bytes Received:</span><span class='stat-value'>" + toStr(m_bytesReceived) + " B</span></div>";
	report += "<div class='stat-row'><span class='stat-label'>Total Bytes Sent:</span><span class='stat-value'>" + toStr(m_bytesSent) + " B</span></div>";
	report += "<div class='stat-row'><span class='stat-label'>Active Connections:</span><span class='stat-value'>" + toStr(m_activeConnections) + "</span></div>";
	
	report += "</div></body></html>";
	return report;
}
