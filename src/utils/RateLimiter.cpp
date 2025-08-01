#include "../../include/utils/RateLimiter.hpp"

RateLimiter::RateLimiter(size_t capacity, size_t refillRate)
    : m_capacity(capacity), m_refillRate(refillRate) {}

RateLimiter::~RateLimiter() {
    m_clients.clear();
}

RateLimiter::RateLimiter(const RateLimiter& copy) {
    (void)copy;
}

RateLimiter& RateLimiter::operator=(const RateLimiter& copy) {
    (void)copy;
    return *this;
}

bool RateLimiter::allowRequest(const std::string& ip) {
    std::map<std::string, Bucket>::iterator it = m_clients.find(ip);
    time_t now = time(NULL);

    if (it == m_clients.end()) {
        // New client
        Bucket new_bucket;
        new_bucket.tokens = m_capacity - 1; // Consume 1 for this request
        new_bucket.last_refill = now;
        m_clients[ip] = new_bucket;
        return true;
    }

    Bucket& b = it->second;
    time_t elapsed = now - b.last_refill;

    // Refill tokens based on elapsed time
    if (elapsed > 0) {
        size_t tokens_to_add = static_cast<size_t>(elapsed) * m_refillRate;
        b.tokens += tokens_to_add;
        if (b.tokens > m_capacity) {
            b.tokens = m_capacity;
        }
        b.last_refill = now;
    } // If 0 seconds elapsed, no tokens refilled

    // Consume token
    if (b.tokens > 0) {
        b.tokens--;
        return true;
    }

    return false; // Rate limited!
}
