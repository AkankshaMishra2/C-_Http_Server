<div align="center">

# 🚀 42-Webserv
### High-Performance HTTP/1.1 Server in C++

[![C++](https://img.shields.io/badge/C++-98/11-blue.svg?style=for-the-badge&logo=c%2B%2B)](https://cplusplus.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)

</div>

---

## 📖 About the Project

**42-Webserv** is a deeply optimized, fully compliant HTTP/1.1 web server built from scratch in C++. Inspired by NGINX, this project demonstrates structural architectural design, strict Object-Oriented Programming (OOP) principles, and advanced Data Structures to handle networking at scale without relying on external libraries.

The server is fully capable of serving static files, executing CGI scripts (PHP, Python), handling file uploads, and routing domains with dynamic constraints. It simulates the core multiplexing engine of NGINX utilizing the `poll()` system call on non-blocking I/O sockets. 

Recently, the core architecture was augmented with advanced DSA capabilities (such as Trie Prefix Trees, LRU Cache, and Token Bucket algorithms) to ensure the server remains highly performant under massive loads while eliminating disk I/O bottlenecks.

---

## 💻 Tech Stack

- **Language:** C++ (C++98 / C++11 compliant)
- **Architecture:** Object-Oriented Programming (OOP), Non-blocking I/O Multiplexing
- **System Calls:** `poll()`, `socket`, `bind`, `listen`, `accept`, `recv`, `send`
- **Data Structures (DSA):**
  - **Trie Prefix Tree:** For O(1) URI routing and wildcard handling.
  - **LRU Cache:** In-memory static file cache to bypass slow disk reads.
  - **Token Bucket Algorithm:** For IP rate limiting and DDoS mitigation.
- **Design Patterns:** Singleton (Metrics tracking), Strategy Pattern
- **Build System:** GNU Make

---

## ✨ Key Features

1. **Trie-based URI Routing**: Path matching uses a Prefix-Tree `TrieRouter` for O(1) string segment resolution, completely bypassing slow iterative array lookups.
2. **LRU Static File Cache**: A heavily optimized in-memory `LRUCache` avoids redundant `read()` syscalls by caching repetitive asset requests natively in RAM.
3. **Token Bucket Rate Limiting**: Built-in IP mitigation to prevent DoS via `RateLimiter` flooding policies.
4. **Multiplexed I/O Engine**: True non-blocking sockets mapped with `poll()` for handling thousands of concurrent connections efficiently.
5. **CGI Support**: PHP & Python executing via system piping, forks, and standard CGI environment variables.
6. **Robust Error Handling**: Exhaustive C++ try-catch mapping to raw HTTP Error Status pages.

---

## 📂 Folder Structure

```text
├── main.cpp                  # Application Entry Point
├── Makefile                  # Build instructions
├── confs/                    # Configuration files
├── include/                  # Header files (.hpp)
│   ├── parsing/              # Core NGINX-style configuration parsing
│   ├── HTTP-Core/            # WebServ engine (Server loop, Handlers)
│   └── utils/                # DSA & Networking utilities (Trie, Cache, etc.)
└── src/                      # Source files (.cpp) logic implementations
```

---

## 🚀 How to Start the Project

### Prerequisites
- Unix System (Linux, macOS, WSL) or Windows with MinGW (Make sure to remove POSIX dependencies if running purely native on Windows).
- `g++` Compiler (with C++98/C++11 standard support).
- GNU `Make`.

### 1. Build the Server

Run the `make` command to compile the source code:

```bash
# Compile the Project
make

# Clean Object Files (optional)
make clean

# Clean Objects + Executable (optional)
make fclean

# Rebuild entirely (optional)
make re
```

### 2. Run the Server

Start the server by passing a configuration file (.conf) path as the first argument.

```bash
./webserv confs/default.conf
```
*The server will boot and immediately bind multiplexed listeners against the server blocks defined in your `.conf` files.*

### 3. Test the Connections

You can interact with the server via any web browser, cURL, or Postman:

- Open your browser and navigate to `http://127.0.0.1:8080` (or whichever port is specified in your `default.conf`).
- Test with cURL:
  ```bash
  curl -I http://127.0.0.1:8080/
  ```

---

## 📊 Sample Output (Server Logs)

```text
[INFO] Parsing Configuration File...
[INFO] Listening on 127.0.0.1:8080
[INFO] Starting run loop
...
[DEBUG] Accepted client 127.0.0.1:52192
[DEBUG] Cache miss, loaded into cache: ./www/index.html
[DEBUG] Cache hit for: ./www/index.html
```

---

## 🔮 Future Enhancements

- **Thread Pool Integration**: Currently single-threaded with multiplexed `poll`. Upgrading to multithreaded `epoll/kqueue` event-loop architecture.
- **HTTPS implementation**: Porting `<openssl>` TLS Handshake implementation.
- **FastCGI Protocol Support**: Replacing brute UNIX fork executing with dedicated FastCGI connection sockets mapping.
