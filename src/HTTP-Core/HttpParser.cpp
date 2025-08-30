#include "../../include/HTTP-Core/HttpParser.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>

HttpParser::HttpParser() : headersParsed(false), contentLength(0) {
}

bool HttpParser::parse(const char* data, size_t length, HttpRequest& request) {
	// Ajouter les nouvelles données au buffer
	buffer.append(data, length);
	
	// Si les headers ne sont pas encore parsés
	if (!headersParsed) {
		// Chercher la fin des headers (\r\n\r\n)
		size_t headerEnd = buffer.find("\r\n\r\n");
		if (headerEnd == std::string::npos) {
			return false; // Headers incomplets
		}
		
		// Extraire et parser les headers
		std::string headerSection = buffer.substr(0, headerEnd);
		if (!parseHeaders(headerSection, request)) {
			request.setValid(false);
			return false;
		}
		
		headersParsed = true;
		
		// Supprimer les headers du buffer
		buffer = buffer.substr(headerEnd + 4);
		
		// Déterminer la longueur du corps
		std::map<std::string, std::string>::const_iterator it = request.getHeaders().find("content-length");
		if (it != request.getHeaders().end()) {
			contentLength = static_cast<size_t>(std::atoi(it->second.c_str()));
		} else {
			contentLength = 0;
		}
	}
	
	// Parser le corps si nécessaire
	if (contentLength > 0) {
		if (buffer.length() >= contentLength) {
			request.setBody(buffer.substr(0, contentLength));
			buffer = buffer.substr(contentLength);
			request.setComplete(true);
			request.setValid(true);
			return true;
		} else {
			return false; // Corps incomplet
		}
	} else {
		// Pas de corps attendu
		request.setComplete(true);
		request.setValid(true);
		return true;
	}
}

bool HttpParser::parseHeaders(const std::string& headerSection, HttpRequest& request) {
	std::istringstream stream(headerSection);
	std::string line;
	
	// Parser la première ligne (Request Line)
	if (!std::getline(stream, line)) {
		return false;
	}
	
	// Supprimer le \r s'il existe
	if (!line.empty() && line[line.length() - 1] == '\r') {
		line = line.substr(0, line.length() - 1);
	}
	
	if (!parseRequestLine(line, request)) {
		return false;
	}
	
	// Parser les headers suivants
	while (std::getline(stream, line)) {
		// Supprimer le \r s'il existe
		if (!line.empty() && line[line.length() - 1] == '\r') {
			line = line.substr(0, line.length() - 1);
		}
		
		if (line.empty()) {
			break; // Fin des headers
		}
		
		if (!parseHeader(line, request)) {
			return false;
		}
	}
	
	return true;
}

bool HttpParser::parseRequestLine(const std::string& line, HttpRequest& request) {
	std::istringstream stream(line);
	std::string method, uri, version;
	
	// Extraire méthode, URI et version
	if (!(stream >> method >> uri >> version)) {
		return false;
	}
	
	request.setMethod(method);
	
	// Parse query string out of URI
	size_t queryPos = uri.find('?');
	if (queryPos != std::string::npos) {
		request.setQueryString(uri.substr(queryPos + 1));
		request.setUri(uri.substr(0, queryPos));
	} else {
		request.setUri(uri);
		request.setQueryString("");
	}
	
	request.setVersion(version);
	
	// Valider la méthode
	if (!isValidMethod(request.getMethod())) {
		return false;
	}
	
	// Valider la version
	if (request.getVersion() != "HTTP/1.1" && request.getVersion() != "HTTP/1.0") {
		return false;
	}
	
	return true;
}

bool HttpParser::parseHeader(const std::string& line, HttpRequest& request) {
	size_t colonPos = line.find(':');
	if (colonPos == std::string::npos) {
		return false;
	}
	
	std::string name = trim(line.substr(0, colonPos));
	std::string value = trim(line.substr(colonPos + 1));
	
	// Convertir le nom en minuscules pour la normalisation
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	
	request.setHeader(name, value);
	return true;
}

std::string HttpParser::trim(const std::string& str) {
	size_t start = str.find_first_not_of(" \t\r\n");
	if (start == std::string::npos) {
		return "";
	}
	
	size_t end = str.find_last_not_of(" \t\r\n");
	return str.substr(start, end - start + 1);
}

bool HttpParser::isValidMethod(const std::string& method) {
	return (method == "GET" || method == "POST" || method == "DELETE");
}

void HttpParser::reset() {
	buffer.clear();
	headersParsed = false;
	contentLength = 0;
}

bool HttpParser::isRequestComplete(const HttpRequest& request) {
	return request.isComplete() && request.isValid();
}
