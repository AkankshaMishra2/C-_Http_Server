#ifndef TRIEROUTER_HPP
#define TRIEROUTER_HPP

#include <string>
#include <map>
#include <vector>
#include "../parsing/LocationConfig.hpp"

class TrieNode {
public:
    std::map<std::string, TrieNode*> children;
    LocationConfig* location; // nullptr if not an exact location end

    TrieNode() : location(NULL) {}
    ~TrieNode() {
        for (std::map<std::string, TrieNode*>::iterator it = children.begin(); it != children.end(); ++it) {
            delete it->second;
        }
    }
};

class TrieRouter {
private:
    TrieNode* m_root;

    // Helper to split a path into segments, e.g., "/api/v1/" -> ["api", "v1"]
    std::vector<std::string> splitPath(const std::string& path) const;

public:
    TrieRouter();
    ~TrieRouter();

    // Prevent copy for safety unless explicitly needed (Rule of Three)
    TrieRouter(const TrieRouter& copy);
    TrieRouter& operator=(const TrieRouter& copy);

    // Insert a LocationConfig into the Trie
    void insert(const std::string& path, LocationConfig* location);

    // Find the longest matching prefix LocationConfig for a given URI
    LocationConfig* match(const std::string& uri) const;
};

#endif
