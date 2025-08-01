# 🗣️ 42-Webserv: Viva & Interview Reference Guide

This document is designed to help you crush technical interviews and code defenses regarding this C++ Web Server. It isolates the high-level architecture, breaks down complex data structures we've integrated, and preps you for interviewer questions.

---

## 🔎 File Architecture & Role Breakdown

### 1. `main.cpp`
- **Role**: Entry point of the server. Validates command-line arguments, triggers the `ConfigParser`, and instantiates the core `WebServ` object with the resolved configuration list. Encapsulates the execution in a central `try-catch` block.

### 2. `include/HTTP-Core/WebServ.hpp` & `src/HTTP-Core/WebServ.cpp`
- **Role**: This is the engine. It binds ports (`socket()`, `bind()`, `listen()`) and manages the primary non-blocking event loop using `poll()`.
- **Flow**:
  1. Initialize Server sockets based on unique host/port bindings.
  2. Map `m_sockets` into `pollfd` structures.
  3. Wait indefinitely on `poll()`.
  4. If a socket is ready, either `acceptClient()` for new connections, or `read/sendData()` for existing HTTP interactions.
  5. Offload fully constructed requests to `RequestHandler`.

### 3. `include/HTTP-Core/RequestHandler.hpp` / `.cpp`
- **Role**: The dispatcher. Examines `GET`, `POST`, or `DELETE`. Interacts natively with the filesystem (File Input/Output). Serves CGI forks on specific location mappings. Accesses `LRUCache`.

### 4. `include/utils/TrieRouter.hpp` `(Feature 1)`
- **Role**: Instead of linear iterations (`O(N)`) looking for a matching `LocationConfig`, we assemble URIs into a Trie Prefix Tree. 
- **Example**: `/api/v1/users` takes exactly 3 tree-node steps `O(L)`, instantly resolving the strict matching constraints.

### 5. `include/utils/LRUCache.hpp` `(Feature 2)`
- **Role**: A hybrid data structure (`std::map` mapping to `std::list`) that caches disk reads.
- **Why?**: Syscalls (`read`, `open`, `stat`) are incredibly slow compared to RAM accesses. The application pushes cached char buffers up to 10MB, discarding the **Least Recently Used** node automatically.

### 6. `include/utils/RateLimiter.hpp` `(Feature 3)`
- **Role**: Implementation of the Token Bucket algorithm to protect against DoS attacks. Clients lose tokens per request and slowly refill them via timestamps.

---

## 🏎️ Execution Flow Step-by-Step

1. **Initialization phase**: `./webserv confs/default.conf`
2. **Tokenizer & Parser**: Reads standard file streams, identifies `#` as comments, isolates `server {}` and `location {}` blocks, filling `ServerConfig` arrays.
3. **Socket Setup**: Identifies all globally unique `listen` directives to prevent `EADDRINUSE` failures.
4. **Poll Loop** block (`WebServ::run()`):
   - System drops to C sleep mode natively via `poll()`.
   - On Wakeup: Event triggers `POLLIN`.
   - If Event == Server Socket: Handshake completed, file descriptor spawned, integrated as new `std::shared_ptr<Client>`.
5. **HTTP Parsing Context**: Loop pushes chunked incoming reads to `Client::receiveData()`. Validates Header formats (`\r\n\r\n` bounds) natively storing `HttpRequest`.
6. **Execution Phase**: `RequestHandler` triggers. `TrieRouter` is traversed to locate directory configurations.
   - If static asset requested: Fetch from `LRUCache` (RAM hit `200 OK`) -> Fallback to Disk (RAM miss `200 OK`) -> Fallback `404 Not Found`.
   - If CGI requested: Invoke POSIX `fork()`, bridge `pipe()`, copy environment strings into child shell block, `waitpid()`, format response.
7. **Write Frame**: Back to `WebServ`, socket switches to `POLLOUT`. Dump HTTP string headers and payload back to networking stack sequentially. Close if `connection: close` or time out asynchronously.

---

## 💡 Top 10 Interview/Viva Questions & Answers

### Q1. Why use `poll()` over standard threaded connections?
**Answer**: Thread-per-request models (like Apache) consume huge amounts of stack memory per client, bottlenecking quickly. Our multiplexed event-driven architecture (like NGINX) uses a single thread checking multiple non-blocking sockets, providing immense scale `O(1)` threading cost.

### Q2. How do you prevent Memory Leaks in this project?
**Answer**: We replaced native legacy `new/delete` Client instances with C++11 `std::shared_ptr` encapsulation. Whenever the vector of clients drops a reference, the C++ garbage-managements (`RAII` concepts) organically destruct the socket cleanly without needing deep leak-proof logic.

### Q3. Explain exactly how `TrieRouter` works conceptually in C++.
**Answer**: By dividing an incoming path (e.g., `/img/logos/foo.png`) by the delimiter `/`, we traverse a set of nodes. Node `img` -> Node `logos`. It holds pointers to location settings. It stops at the deepest matched point natively finding the longest prefix match rather than string comparing all 50 configs manually.

### Q4. Walk me through the implementation logic of your `LRUCache`.
**Answer**: It fundamentally requires constant time `O(1)` read/write. I used `std::map<string, list::iterator>`. The `map` resolves the key instantly. The `list` manipulates positions instantly. If you read a value, I traverse via the iterator directly into the list, slice it out, and plop it at the FRONT of the list. If it breaches 10MB capacity, I just `pop_back()` the tail of the list and erase it from the map.

### Q5. What is the Token Bucket algorithm used for Rate Limiting?
**Answer**: We assign 'tokens' mapped by IP keys. E.g., Max 100 tokens refilling at 10/second. On every request, I subtract `1`. When it hits `0`, they get blocked. Instead of running a complex threaded clock loop to refill everyone, we use "Lazy Evaluation". We store a `last_refill` timestamp. When they interact again, we calculate `(now - last_refill) * refill_rate`, add the difference, and clamp it to `100`.

### Q6. Are the file descriptors Blocking or Non-Blocking? Why?
**Answer**: Strictly Non-Blocking via `fcntl(fd, F_SETFL, O_NONBLOCK)`. If it was blocking, reading from a client sending data too slowly (`dial-up speeds`) would freeze our entire singular thread server loop, causing a Denial of Service.

### Q7. How does HTTP `fork()` CGI handle memory boundaries safely?
**Answer**: We build a unidirectional `pipe()` before calling `fork()`. The OS duplicates the Process Memory via Copy-on-Write (COW). The child overwrites its standard execution interface `STDOUT` directly straight into the write end of the Pipe via `dup2()`. It then runs `exec()`, replacing its execution stack utterly with Python/PHP. The parent passively hangs on `waitpid()`, flushing the output buffer chunks natively.

### Q8. What does `const std::string&` do compared to `std::string`?
**Answer**: It prevents heavy copy-constructor instantiation logic. Passing a parameter by `value` duplicates the string array char allocations. Passing it by `const &` effectively passes a `const *` (reference), which takes virtually `0` CPU cycles and prevents unsafe mutations.

### Q9. Give me a concrete scenario on what happens if an unhandled Exception occurs in `Client::receiveData()`.
**Answer**: The C++ program will unwind the local stack until caught by the master catch block in `WebServ::run()` or `main.cpp`.

### Q10. What defines "const correctness" in your methods?
**Answer**: Methods are appended with `const` ending decorators (e.g., `getServer() const`). It physically restricts the method from ever altering Member Fields (`this->value = 1` fails compilation) proving to the compiler, and readers, that the state is immutable here.
