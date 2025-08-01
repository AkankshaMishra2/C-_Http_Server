#ifndef RATELIMITER_HPP
#define RATELIMITER_HPP

#include <iostream>
#include <map>
#include <string>
#include <ctime>

// A simple Token Bucket rate limiter per IP address
class RateLimiter {
private:
    struct Bucket {
        size_t tokens;
        time_t last_refill;
    };

    size_t m_capacity;
    size_t m_refillRate; // tokens per second
    std::map<std::string, Bucket> m_clients;

public:
    RateLimiter(size_t capacity = 100, size_t refillRate = 10);
    ~RateLimiter();

    // Prevent copy
    RateLimiter(const RateLimiter& copy);
    RateLimiter& operator=(const RateLimiter& copy);

    // Determines if a request from 'ip' is allowed. Consumes 1 token if true.
    bool allowRequest(const std::string& ip);
};

#endif
