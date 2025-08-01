#ifndef LRUCACHE_HPP
#define LRUCACHE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <utility>

// LRU Cache for static file contents
class LRUCache {
private:
    size_t m_capacityBytes;
    size_t m_currentBytes;

    // The list holds pairs of path and file content {path, content}
    typedef std::pair<std::string, std::vector<char> > CacheItem;
    std::list<CacheItem> m_list;

    // The map holds the path as key, and iterator to list item as value
    typedef std::map<std::string, std::list<CacheItem>::iterator> CacheMap;
    CacheMap m_map;

    void evict(); // Evicts LRU item until current bytes <= capacity bytes

public:
    LRUCache(size_t capacityBytes = 10485760); // Default 10MB
    ~LRUCache();

    // Prevent copy
    LRUCache(const LRUCache& copy);
    LRUCache& operator=(const LRUCache& copy);

    // Get item from cache, returns empty vector if not found
    std::vector<char> get(const std::string& key);

    // Put item in cache
    void put(const std::string& key, const std::vector<char>& content);

    // Invalidate
    void invalidate(const std::string& key);

    size_t getCurrentBytes() const;
};

#endif
