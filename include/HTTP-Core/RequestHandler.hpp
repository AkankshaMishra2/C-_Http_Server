#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "HttpParser.hpp"
#include "HttpResponse.hpp"
#include "../parsing/ServerConfig.hpp"
#include "../parsing/LocationConfig.hpp"
#include "../utils/LRUCache.hpp"
#include <string>

class RequestHandler {
private:
	std::string documentRoot;
	LRUCache m_fileCache;
	
	std::string getMimeType(const std::string& filename);
	std::string readFile(const std::string& filepath);
	bool fileExists(const std::string& filepath);
	std::string getErrorPage(int errorCode);
	std::string generateDirectoryListing(const std::string& dirPath, const std::string& uri);
	HttpResponse executeCGI(const HttpRequest& request, const ServerConfig& server, const LocationConfig* location, const std::string& final_uri);
	std::string resolvePath(const std::string& path);

public:
	RequestHandler(const std::string& docRoot = "./www");
	
	// Méthodes principales (Translated from French)
	HttpResponse handleRequest(const HttpRequest& request, const ServerConfig& server, const LocationConfig* location);
	HttpResponse handleGet(const HttpRequest& request, const ServerConfig& server, const LocationConfig* location);
	HttpResponse handlePost(const HttpRequest& request, const ServerConfig& server, const LocationConfig* location);
	HttpResponse handleDelete(const HttpRequest& request, const ServerConfig& server, const LocationConfig* location);
	
	void setDocumentRoot(const std::string& root);
	
	// Utility for extracting directory from path
	std::string getDirectoryListingHtml(const std::string& dirPath, const std::string& uri);
};

#endif
