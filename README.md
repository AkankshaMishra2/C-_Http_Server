# 🚀 42-Webserv: High-Performance HTTP Server

Welcome to **42-Webserv**, a deeply optimized, fully compliant HTTP/1.1 web server built from scratch in C++. This project demonstrates structural architectural design, strict OOP principles, and advanced Data Structures to handle networking at scale.

## 📝 Description

This Web Server is capable of serving static files, executing CGI scripts, handling file uploads, and routing domains with dynamic constraints. It simulates the core multiplexing engine of NGINX utilizing the `poll()` system call on non-blocking I/O sockets.

We recently augmented the core architecture by adding advanced DSA capabilities (Trie Prefix Trees, LRU Cache, and Token Bucket algorithms) to ensure the server remains highly performant under massive loads while eliminating disk I/O bottlenecks.

---

## ✨ Key Features

1. **Trie-based URI Routing**: Path matching uses a Prefix-Tree `TrieRouter` for O(1) string segment resolution, completely bypassing slow iterative array lookups.
2. **LRU Static File Cache**: A heavily optimized in-memory `LRUCache` avoids redundant `read()` syscalls by caching repetitive asset requests natively in RAM.
3. **Token Bucket Rate Limiting**: Built-in IP mitigation to prevent DoS via `RateLimiter` flooding policies.
4. **Multiplexed I/O Engine**: True non-blocking sockets mapped with `poll()`.
5. **CGI Support**: PHP & Python executing via piping and standard environment variables.
6. **Robust Error Handling**: Exhaustive C++ try-catch mapping to raw HTTP Error Status pages.

---

## 🛠️ Concepts Used

- **Object-Oriented Programming (OOP)**: Absolute encapsulation (Getters/Setters), RAII, Rule of Three.
- **Modern Standards**: Usage of `<memory>` `std::shared_ptr` for eliminating dangling `Client*` leaks.
- **STL Data Structures**: Intense usage of `std::map`, `std::vector`, `std::list`, `std::pair`.
- **Operating Systems / Networking**: Sockets, `fd` manipulation, `bind/listen`, TCP framing.
- **Design Patterns**: Singleton Pattern (`ServerStats`), Strategy Pattern.

---

## 📂 Folder Structure

```
├── main.cpp                  # Entry point
├── Makefile                  # Build instructions
├── include/                  # Header files (.hpp)
│   ├── parsing/              # Core NGINX-style configuration parsing
│   ├── HTTP-Core/            # WebServ engine (Server loop, Handlers)
│   └── utils/                # DSA & Networking utilities (Trie, Cache)
└── src/                      # Source files (.cpp) logic implementations
```

---

## 🚀 Compilation & Execution

### Prerequisites
- Unix System (Linux, macOS, WSL) or Windows with MinGW (Make sure to remove POSIX dependencies if running purely native on Windows).
- `g++` Compiler (with C++11 standard support).
- GNU `Make`.

### Build Commands

```bash
# Compile the Project
make

# Clean Object Files
make clean

# Clean Objects + Executable
make fclean

# Rebuild
make re
```

### Running the Server

Pass the supplied `.conf` path as the first argument:

```bash
./webserv confs/default.conf
```
*The server will boot and immediately bind multiplexed listeners against the blocks defined in your `.conf` files.*

---

## 📊 Sample Output

### Server Startup
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
