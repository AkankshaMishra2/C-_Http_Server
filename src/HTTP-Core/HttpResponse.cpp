#include "../../include/HTTP-Core/HttpResponse.hpp"
#include <ctime>
#include <cstdio>

HttpResponse::HttpResponse() : statusCode(200), statusText("OK") {
	// Headers par défaut selon HTTP/1.1
	setHeader("Server", "akanksha-serv/1.0");
	setHeader("Connection", "close");
}

HttpResponse::HttpResponse(int code, const std::string& text) : statusCode(code), statusText(text) {
	setHeader("Server", "akanksha-serv/1.0");
	setHeader("Connection", "close");
}

void HttpResponse::setStatus(int code, const std::string& text) {
	statusCode = code;
	statusText = text;
}

void HttpResponse::setHeader(const std::string& name, const std::string& value) {
	headers[name] = value;
}

void HttpResponse::setBody(const std::string& content) {
	body = content;
	// Automatiquement définir Content-Length
	std::ostringstream oss;
	oss << content.length();
	setHeader("Content-Length", oss.str());
}

void HttpResponse::setContentType(const std::string& type) {
	setHeader("Content-Type", type);
}

std::string HttpResponse::serialize() const {
	std::ostringstream response;
	
	// Status line
	response << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n";
	
	// Headers
	for (std::map<std::string, std::string>::const_iterator it = headers.begin();
		it != headers.end(); ++it) {
		response << it->first << ": " << it->second << "\r\n";
	}
	
	// Date header (obligatoire selon HTTP/1.1)
	time_t now = time(0);
	char dateStr[100];
	struct tm* gmt = gmtime(&now);
	strftime(dateStr, sizeof(dateStr), "%a, %d %b %Y %H:%M:%S GMT", gmt);
	response << "Date: " << dateStr << "\r\n";
	
	// Ligne vide avant le body
	response << "\r\n";
	
	// Body
	response << body;
	
	return response.str();
}

// Réponses prédéfinies conformes au sujet webserv
HttpResponse HttpResponse::ok(const std::string& content) {
	HttpResponse response(200, "OK");
	response.setBody(content);
	response.setContentType("text/html");
	return response;
}

HttpResponse HttpResponse::notFound() {
	HttpResponse response(404, "Not Found");
	std::string errorPage = 
		"<!DOCTYPE html>\n"
		"<html><head><title>404 Not Found</title></head>\n"
		"<body><h1>404 Not Found</h1>\n"
		"<p>The requested resource was not found on this server.</p>\n"
		"<hr><p>akanksha-serv/1.0</p></body></html>\n";
	response.setBody(errorPage);
	response.setContentType("text/html");
	return response;
}

HttpResponse HttpResponse::methodNotAllowed() {
	HttpResponse response(405, "Method Not Allowed");
	std::string errorPage = 
		"<!DOCTYPE html>\n"
		"<html><head><title>405 Method Not Allowed</title></head>\n"
		"<body><h1>405 Method Not Allowed</h1>\n"
		"<p>The requested method is not allowed for this resource.</p>\n"
		"<hr><p>akanksha-serv/1.0</p></body></html>\n";
	response.setBody(errorPage);
	response.setContentType("text/html");
	return response;
}

HttpResponse HttpResponse::internalServerError() {
	HttpResponse response(500, "Internal Server Error");
	std::string errorPage = 
		"<!DOCTYPE html>\n"
		"<html><head><title>500 Internal Server Error</title></head>\n"
		"<body><h1>500 Internal Server Error</h1>\n"
		"<p>The server encountered an internal error.</p>\n"
		"<hr><p>akanksha-serv/1.0</p></body></html>\n";
	response.setBody(errorPage);
	response.setContentType("text/html");
	return response;
}

HttpResponse HttpResponse::badRequest() {
	HttpResponse response(400, "Bad Request");
	std::string errorPage = 
		"<!DOCTYPE html>\n"
		"<html><head><title>400 Bad Request</title></head>\n"
		"<body><h1>400 Bad Request</h1>\n"
		"<p>The request could not be understood by the server.</p>\n"
		"<hr><p>akanksha-serv/1.0</p></body></html>\n";
	response.setBody(errorPage);
	response.setContentType("text/html");
	return response;
}

HttpResponse HttpResponse::created() {
	HttpResponse response(201, "Created");
	std::string successPage = 
		"<!DOCTYPE html>\n"
		"<html><head><title>201 Created</title></head>\n"
		"<body><h1>201 Created</h1>\n"
		"<p>Resource created successfully.</p>\n"
		"<hr><p>akanksha-serv/1.0</p></body></html>\n";
	response.setBody(successPage);
	response.setContentType("text/html");
	return response;
}

HttpResponse HttpResponse::noContent() {
	HttpResponse response(204, "No Content");
// Pas de body pour 204
	return response;
}

HttpResponse HttpResponse::forbidden() {
	HttpResponse response(403, "Forbidden");
	std::string errorPage = 
		"<!DOCTYPE html>\n"
		"<html><head><title>403 Forbidden</title></head>\n"
		"<body><h1>403 Forbidden</h1>\n"
		"<p>You don't have permission to access this resource.</p>\n"
		"<hr><p>akanksha-serv/1.0</p></body></html>\n";
	response.setBody(errorPage);
	response.setContentType("text/html");
	return response;
}

HttpResponse HttpResponse::requestTimeOut() {
	HttpResponse response(408, "Request Timeout");
	std::string errorPage = 
		"<!DOCTYPE html>\n"
		"<html><head><title>408 Request Timeout</title></head>\n"
		"<body><h1>408 Request Timeout</h1>\n"
		"<p>The server timed out waiting for the request.</p>\n"
		"<hr><p>akanksha-serv/1.0</p></body></html>\n";
	response.setBody(errorPage);
	response.setContentType("text/html");
	return response;
}

HttpResponse HttpResponse::requestTooLarge() {
	HttpResponse response(413, "Request Entity Too Large");
	std::string errorPage = 
		"<!DOCTYPE html>\n"
		"<html><head><title>413 Request Entity Too Large</title></head>\n"
		"<body><h1>413 Request Entity Too Large</h1>\n"
		"<p>The request body exceeds the configured maximum size limit.</p>\n"
		"<hr><p>akanksha-serv/1.0</p></body></html>\n";
	response.setBody(errorPage);
	response.setContentType("text/html");
	return response;
}

HttpResponse HttpResponse::redirect(const std::string& url) {
	HttpResponse response(301, "Moved Permanently");
	response.setHeader("Location", url);
	std::string page = 
		"<!DOCTYPE html>\n"
		"<html><head><title>301 Moved Permanently</title></head>\n"
		"<body><h1>301 Moved Permanently</h1>\n"
		"<p>The document has moved <a href=\"" + url + "\">here</a>.</p>\n"
		"<hr><p>akanksha-serv/1.0</p></body></html>\n";
	response.setBody(page);
	response.setContentType("text/html");
	return response;
}
