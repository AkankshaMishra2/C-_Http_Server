#include "../../include/HTTP-Core/RequestHandler.hpp"
#include "../../include/ServerStats.hpp"
#include "../../include/Logger.hpp"
#include <fstream>
#include <cstdio>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <limits.h>
#include <algorithm>
#include <sstream>

// Helper structure for directory sorting
struct DirEntry {
	std::string name;
	size_t size;
	time_t modTime;
	bool isDir;

	bool operator<(const DirEntry& other) const {
		if (isDir != other.isDir)
			return isDir; // Directories come first
		return name < other.name; // Then alphabetical
	}
};

RequestHandler::RequestHandler(const std::string& docRoot) : documentRoot(docRoot) {
	// Convert to absolute path
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		documentRoot = std::string(cwd) + "/" + docRoot;
	}
}

std::string RequestHandler::resolvePath(const std::string& path) {
	if (path.empty() || path[0] == '/') return path;
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		return std::string(cwd) + "/" + path;
	}
	return path;
}

void RequestHandler::setDocumentRoot(const std::string& root) {
	documentRoot = root;
}

HttpResponse RequestHandler::handleRequest(const HttpRequest& request, const ServerConfig& server, const LocationConfig* location) {
	// Basic validation
	if (!request.isValid() || !request.isComplete()) {
		return HttpResponse::badRequest();
	}
	
	// Check body size limit
	if (request.getBody().size() > server.getClientMaxBodySize()) {
		return HttpResponse::requestTooLarge();
	}
	
	// Feature 2: Stats execution hook
	if (request.getMethod() == "GET" && request.getUri() == "/stats") {
		HttpResponse resp = HttpResponse::ok(ServerStats::getInstance().getReport());
		resp.setContentType("text/html");
		return resp;
	}
	
	// Route based on HTTP method
	if (request.getMethod() == "GET") {
		return handleGet(request, server, location);
	} else if (request.getMethod() == "POST") {
		return handlePost(request, server, location);
	} else if (request.getMethod() == "DELETE") {
		return handleDelete(request, server, location);
	} else {
		// Method not supported
		return HttpResponse::methodNotAllowed();
	}
}

HttpResponse RequestHandler::handleGet(const HttpRequest& request, const ServerConfig& server, const LocationConfig* location) {
	std::string root = resolvePath(location && !location->getRoot().empty() ? location->getRoot() : server.getRoot());
	std::string filepath = root + request.getUri();
	
	// Security: Prevent directory traversal path escaping
	if (request.getUri().find("..") != std::string::npos) {
		return HttpResponse::forbidden();
	}
	
	std::string final_uri = request.getUri();
	
	struct stat st;
	if (stat(filepath.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
		std::string index = location ? location->getIndex() : server.getIndex();
		if (!index.empty()) {
			std::string indexPath = filepath + "/" + index;
			if (fileExists(indexPath)) {
				filepath = indexPath;
				final_uri = request.getUri() + (request.getUri().empty() || request.getUri()[request.getUri().size()-1] == '/' ? "" : "/") + index;
			} else if (location && location->getAutoIndex()) {
				return HttpResponse::ok(getDirectoryListingHtml(filepath, request.getUri()));
			} else if (server.getAutoIndex()) {
				return HttpResponse::ok(getDirectoryListingHtml(filepath, request.getUri()));
			} else {
				return HttpResponse::notFound();
			}
		} else if (location && location->getAutoIndex()) {
			return HttpResponse::ok(getDirectoryListingHtml(filepath, request.getUri()));
		} else if (server.getAutoIndex()) {
			return HttpResponse::ok(getDirectoryListingHtml(filepath, request.getUri()));
		} else {
			return HttpResponse::notFound();
		}
	}
	
	// Check CGI now using final_uri (so /api/index.php is detected)
	if (location && location->hasCGI(final_uri)) {
	    return executeCGI(request, server, location, final_uri);
	}
	
	// Verify that the requested file exists
	if (!fileExists(filepath)) {
		return HttpResponse::notFound();
	}
	
	// Read the file's contents
	std::string content = readFile(filepath);
	if (content.empty() && errno != 0) {
		Logger::error("Failed reading file: " + filepath);
		return HttpResponse::internalServerError();
	}
	
	// Build response with correct MIME type
	HttpResponse response = HttpResponse::ok(content);
	response.setContentType(getMimeType(filepath));
	
	return response;
}

HttpResponse RequestHandler::handlePost(const HttpRequest& request, const ServerConfig& server, const LocationConfig* location) {
	// Check CGI execution
	if (location && location->hasCGI(request.getUri())) {
		return executeCGI(request, server, location, request.getUri());
	}
	
	// Simple file upload flow
	std::string uploadDir = resolvePath(location ? location->getUploadDir() : server.getUpload());
	if (uploadDir.empty()) {
		return HttpResponse::methodNotAllowed();
	}
	
	std::string filepath = uploadDir + request.getUri();
	
	// Security
	if (request.getUri().find("..") != std::string::npos) {
		return HttpResponse::forbidden();
	}
	
	// Write HTTP body to the specified file
	std::ofstream file(filepath.c_str(), std::ios::binary);
	if (!file.is_open()) {
		Logger::error("Failed to open file for POST upload: " + filepath);
		return HttpResponse::internalServerError();
	}
	
	file.write(request.getBody().c_str(), request.getBody().length());
	file.close();
	
	Logger::info("Successfully uploaded file via POST: " + filepath);
	return HttpResponse::created();
}

HttpResponse RequestHandler::handleDelete(const HttpRequest& request, const ServerConfig& server, const LocationConfig* location) {
	std::string root = resolvePath(location && !location->getRoot().empty() ? location->getRoot() : server.getRoot());
	std::string filepath = root + request.getUri();
	
	// Security constraint
	if (request.getUri().find("..") != std::string::npos) {
		return HttpResponse::forbidden();
	}
	
	// Verify file existence
	if (!fileExists(filepath)) {
		return HttpResponse::notFound();
	}
	
	// Remove the file
	if (remove(filepath.c_str()) == 0) {
		Logger::info("Deleted file: " + filepath);
		return HttpResponse::noContent(); // 204 No Content
	} else {
		Logger::error("Failed to delete file: " + filepath);
		return HttpResponse::internalServerError();
	}
}

// Feature 3: Styled and Sorted Directory Listing 
std::string RequestHandler::getDirectoryListingHtml(const std::string& dirPath, const std::string& uri) {
	std::string html = "<!DOCTYPE html><html><head><title>Index of " + uri + "</title>";
	html += "<style>body { font-family: monospace; background-color: #f9f9f9; padding: 20px; } "
	        "table { width: 80%; margin-top: 20px; border-collapse: collapse; } "
	        "th, td { padding: 10px; border-bottom: 1px solid #ddd; text-align: left; } "
	        "th { background-color: #2c3e50; color: white; } "
	        "a { text-decoration: none; color: #3498db; font-weight: bold; } "
	        "a:hover { text-decoration: underline; }</style></head><body>";
	html += "<h1>Index of " + uri + "</h1><table><tr><th>Name</th><th>Size</th><th>Last Modified</th></tr>";
	
	std::vector<DirEntry> entries;
	DIR* dir = opendir(dirPath.c_str());
	if (dir) {
		struct dirent* entry;
		while ((entry = readdir(dir)) != NULL) {
			std::string name = entry->d_name;
			if (name == ".") continue;
			
			std::string fullPath = dirPath + "/" + name;
			struct stat st;
			if (stat(fullPath.c_str(), &st) == 0) {
				DirEntry de;
				de.name = name;
				de.size = st.st_size;
				de.modTime = st.st_mtime;
				de.isDir = S_ISDIR(st.st_mode);
				entries.push_back(de);
			}
		}
		closedir(dir);
	}

	std::sort(entries.begin(), entries.end());
	
	std::string currentUri = uri;
	if (currentUri.empty() || currentUri[currentUri.size()-1] != '/')
		currentUri += '/';

	for (size_t i = 0; i < entries.size(); ++i) {
		html += "<tr><td><a href=\"" + currentUri + entries[i].name + "\">" + entries[i].name;
		if (entries[i].isDir) html += "/";
		html += "</a></td>";
		
		if (entries[i].isDir) {
			html += "<td>-</td>";
		} else {
			std::ostringstream sz;
			sz << entries[i].size << " B";
			html += "<td>" + sz.str() + "</td>";
		}
		
		char dateStr[100];
		struct tm* timeinfo = localtime(&entries[i].modTime);
		strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", timeinfo);
		html += "<td>" + std::string(dateStr) + "</td></tr>";
	}
	
	html += "</table><hr><p>akanksha-serv/1.0 - Directory Listing</p></body></html>";
	return html;
}

HttpResponse RequestHandler::executeCGI(const HttpRequest& request, const ServerConfig& server, const LocationConfig* location, const std::string& final_uri) {
	std::string cgiPath = location->getCGI(final_uri);
	std::string root = resolvePath(location && !location->getRoot().empty() ? location->getRoot() : server.getRoot());
	std::string scriptPath = root + final_uri;
	
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		return HttpResponse::internalServerError();
	}
	
	pid_t pid = fork();
	if (pid == -1) {
		close(pipefd[0]);
		close(pipefd[1]);
		return HttpResponse::internalServerError();
	}
	
	if (pid == 0) {
		// Child Process executing CGI
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		
		// Set environment variables required for standard CGI execution
		setenv("REQUEST_METHOD", request.getMethod().c_str(), 1);
		setenv("SCRIPT_FILENAME", scriptPath.c_str(), 1);
		setenv("QUERY_STRING", request.getQueryString().c_str(), 1);
		
		std::ostringstream oss;
		oss << request.getBody().size();
		setenv("CONTENT_LENGTH", oss.str().c_str(), 1);
		
		std::map<std::string, std::string>::const_iterator it = request.getHeaders().find("content-type");
		setenv("CONTENT_TYPE", it != request.getHeaders().end() ? it->second.c_str() : "", 1);
		
		execl(cgiPath.c_str(), cgiPath.c_str(), scriptPath.c_str(), NULL);
		exit(1);
	} else {
		// Parent Process reads derived CGI output
		close(pipefd[1]);
		std::string output;
		char buf[1024];
		ssize_t n;
		while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
			output.append(buf, n);
		}
		close(pipefd[0]);
		int status;
		waitpid(pid, &status, 0);
		
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
			Logger::error("CGI execution failed with exit status: " + toStr(WEXITSTATUS(status)));
			return HttpResponse::internalServerError();
		}
		
		if (output.empty()) {
			Logger::error("CGI script returned empty output");
			return HttpResponse::internalServerError();
		}
		
		// Typically, a robust CGI handler should parse headers from output.
		// For simplicity, adapting to existing behavior: return ok string.
		HttpResponse resp = HttpResponse::ok(output);
		resp.setContentType("text/html");
		return resp;
	}
}

std::string RequestHandler::getMimeType(const std::string& filename) {
	size_t dotPos = filename.find_last_of('.');
	if (dotPos == std::string::npos) {
		return "application/octet-stream";
	}
	
	std::string extension = filename.substr(dotPos + 1);
	
	// Basic MIME mappings common for webserv projects
	if (extension == "html" || extension == "htm") return "text/html";
	if (extension == "css") return "text/css";
	if (extension == "js") return "application/javascript";
	if (extension == "json") return "application/json";
	if (extension == "txt") return "text/plain";
	if (extension == "jpg" || extension == "jpeg") return "image/jpeg";
	if (extension == "png") return "image/png";
	if (extension == "gif") return "image/gif";
	if (extension == "pdf") return "application/pdf";
	
	return "application/octet-stream";
}

std::string RequestHandler::readFile(const std::string& filepath) {
	// Check Cache
	std::vector<char> cachedFile = m_fileCache.get(filepath);
	if (!cachedFile.empty()) {
		Logger::debug("Cache hit for: " + filepath);
		return std::string(cachedFile.begin(), cachedFile.end());
	}

	std::ifstream file(filepath.c_str(), std::ios::binary);
	if (!file.is_open()) {
		return "";
	}
	
	// Read entire file via input iterators
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);
	
	std::string content(size, '\0');
	file.read(&content[0], size);
	file.close();
	
	// Insert into cache
	if (!content.empty()) {
		std::vector<char> cacheData(content.begin(), content.end());
		m_fileCache.put(filepath, cacheData);
		Logger::debug("Cache miss, loaded into cache: " + filepath);
	}
	
	return content;
}

bool RequestHandler::fileExists(const std::string& filepath) {
	struct stat st;
	return (stat(filepath.c_str(), &st) == 0 && S_ISREG(st.st_mode));
}
