#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <string>
#include <map>
#include <iostream>
#include <sstream>

class HttpRequest {
private:
	std::string m_method;
	std::string m_uri;
	std::string m_queryString;
	std::string m_version;
	std::map<std::string, std::string> m_headers;
	std::string m_body;
	bool m_isComplete;
	bool m_isValid;

public:
	HttpRequest() : m_isComplete(false), m_isValid(false) {}
	
	// Getters
	const std::string& getMethod() const { return m_method; }
	const std::string& getUri() const { return m_uri; }
	const std::string& getQueryString() const { return m_queryString; }
	const std::string& getVersion() const { return m_version; }
	const std::map<std::string, std::string>& getHeaders() const { return m_headers; }
	const std::string& getBody() const { return m_body; }
	bool isComplete() const { return m_isComplete; }
	bool isValid() const { return m_isValid; }

	// Setters
	void setMethod(const std::string& method) { m_method = method; }
	void setUri(const std::string& uri) { m_uri = uri; }
	void setQueryString(const std::string& qs) { m_queryString = qs; }
	void setVersion(const std::string& version) { m_version = version; }
	void setHeader(const std::string& key, const std::string& value) { m_headers[key] = value; }
	void appendBody(const std::string& chunk) { m_body += chunk; }
	void setBody(const std::string& body) { m_body = body; }
	void setComplete(bool complete) { m_isComplete = complete; }
	void setValid(bool valid) { m_isValid = valid; }
	
	void reset() {
		m_method.clear();
		m_uri.clear();
		m_queryString.clear();
		m_version.clear();
		m_headers.clear();
		m_body.clear();
		m_isComplete = false;
		m_isValid = false;
	}

	void print() const {
		std::cout << "Method: " << m_method << std::endl;
		std::cout << "URI: " << m_uri << std::endl;
		std::cout << "Query: " << m_queryString << std::endl;
		std::cout << "Version: " << m_version << std::endl;
		std::cout << "Headers:" << std::endl;
		
		for (std::map<std::string, std::string>::const_iterator it = m_headers.begin(); 
			it != m_headers.end(); ++it) {
			std::cout << "  " << it->first << ": " << it->second << std::endl;
		}
		
		if (!m_body.empty()) {
			std::cout << "Body: " << m_body << std::endl;
		}
	}
};

class HttpParser {
private:
	std::string buffer;
	bool headersParsed;
	size_t contentLength;
	
	bool parseHeaders(const std::string& headerSection, HttpRequest& request);
	bool parseRequestLine(const std::string& line, HttpRequest& request);
	bool parseHeader(const std::string& line, HttpRequest& request);
	std::string trim(const std::string& str);
	bool isValidMethod(const std::string& method);

public:
	HttpParser();
	bool parse(const char* data, size_t length, HttpRequest& request);
	void reset();
	bool isRequestComplete(const HttpRequest& request);
};

#endif
