#include "../../include/utils/LRUCache.hpp"

LRUCache::LRUCache(size_t capacityBytes) : m_capacityBytes(capacityBytes), m_currentBytes(0) {}

LRUCache::~LRUCache() {
    m_list.clear();
    m_map.clear();
}

LRUCache::LRUCache(const LRUCache& copy) {
    (void)copy;
}

LRUCache& LRUCache::operator=(const LRUCache& copy) {
    (void)copy;
    return *this;
}

void LRUCache::evict() {
    while (m_currentBytes > m_capacityBytes && !m_list.empty()) {
        std::string evict_key = m_list.front().first;
        size_t size = m_list.front().second.size();
        
        m_map.erase(evict_key);
        m_list.pop_front();
        m_currentBytes -= size;
    }
}

std::vector<char> LRUCache::get(const std::string& key) {
    CacheMap::iterator it = m_map.find(key);
    if (it == m_map.end()) {
        return std::vector<char>(); // Not found
    }

    // Move to end (most recently used)
    m_list.splice(m_list.end(), m_list, it->second);
    return it->second->second;
}

void LRUCache::put(const std::string& key, const std::vector<char>& content) {
    CacheMap::iterator it = m_map.find(key);

    if (it != m_map.end()) {
        // Update existing item
        m_currentBytes -= it->second->second.size();
        m_currentBytes += content.size();
        it->second->second = content;
        
        // Move to most recently used
        m_list.splice(m_list.end(), m_list, it->second);
    } else {
        // Insert new item
        m_list.push_back(std::make_pair(key, content)); // In C++11 could use emplace_back
        std::list<CacheItem>::iterator new_it = m_list.end();
        --new_it;
        m_map[key] = new_it;
        m_currentBytes += content.size();
    }
    
    // Evict if over capacity
    evict();
}

void LRUCache::invalidate(const std::string& key) {
    CacheMap::iterator it = m_map.find(key);
    if (it != m_map.end()) {
        m_currentBytes -= it->second->second.size();
        m_list.erase(it->second);
        m_map.erase(it);
    }
}

size_t LRUCache::getCurrentBytes() const {
    return m_currentBytes;
}
