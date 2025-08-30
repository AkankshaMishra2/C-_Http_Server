#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <map>
#include <sstream>

class HttpResponse {
public:
	int statusCode;
	std::string statusText;
	std::map<std::string, std::string> headers;
	std::string body;
	
	HttpResponse();
	HttpResponse(int code, const std::string& text);
	
	void setStatus(int code, const std::string& text);
	void setHeader(const std::string& name, const std::string& value);
	void setBody(const std::string& content);
	void setContentType(const std::string& type);
	
	std::string serialize() const;
	
	// Réponses prédéfinies selon le sujet webserv
	static HttpResponse ok(const std::string& content);
	static HttpResponse notFound();
	static HttpResponse methodNotAllowed();
	static HttpResponse internalServerError();
	static HttpResponse badRequest();
	static HttpResponse created();
	static HttpResponse noContent();
	static HttpResponse forbidden();
	static HttpResponse requestTimeOut();
	static HttpResponse requestTooLarge();
	static HttpResponse redirect(const std::string& url);
};

#endif
