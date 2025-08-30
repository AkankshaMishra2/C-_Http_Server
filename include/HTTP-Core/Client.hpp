#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "HttpParser.hpp"
#include "HttpResponse.hpp"
#include <string>
#include <sys/socket.h>

class Client {
	public:
		Client(int clientFd, int servFd, const std::string& clientIp, int clientPort);
		~Client();

		int getFd() const;
		int getServerFd() const;
		const std::string& getIp() const;
		int getPort() const;

		bool isRequestComplete() const;
		bool isResponseSent() const;

		void receiveData();
		void sendResponse();

		const HttpRequest& getRequest() const;
		void setResponse(const HttpResponse& resp);

		bool hasTimedOut(time_t timeout) const;
		size_t getBytesReceived() const;
		size_t getBytesSent() const;

	private:
		int fd;
		int serverFd;
		std::string ip;
		int port;
		HttpParser parser;
		HttpRequest request;
		HttpResponse response;
		bool requestComplete;
		bool responseSent;
		std::string buffer;
		
		time_t m_connectedAt;
		size_t m_bytesReceived;
		size_t m_bytesSent;
};

#endif
