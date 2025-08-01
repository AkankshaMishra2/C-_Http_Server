#include "../../include/utils/TrieRouter.hpp"
#include <sstream>

TrieRouter::TrieRouter() : m_root(new TrieNode()) {}

TrieRouter::~TrieRouter() {
    delete m_root;
}

TrieRouter::TrieRouter(const TrieRouter& copy) {
    (void)copy;
    m_root = new TrieNode(); // Deep copy could be implemented if necessary
}

TrieRouter& TrieRouter::operator=(const TrieRouter& copy) {
    (void)copy;
    return *this; // Assignment operator if strictly needed
}

std::vector<std::string> TrieRouter::splitPath(const std::string& path) const {
    std::vector<std::string> segments;
    std::stringstream ss(path);
    std::string item;
    
    // Split by '/'
    while (std::getline(ss, item, '/')) {
        if (!item.empty()) {
            segments.push_back(item);
        }
    }
    return segments;
}

void TrieRouter::insert(const std::string& path, LocationConfig* location) {
    std::vector<std::string> segments = splitPath(path);
    TrieNode* current = m_root;

    for (size_t i = 0; i < segments.size(); ++i) {
        if (current->children.find(segments[i]) == current->children.end()) {
            current->children[segments[i]] = new TrieNode();
        }
        current = current->children[segments[i]];
    }
    current->location = location;
}

LocationConfig* TrieRouter::match(const std::string& uri) const {
    std::vector<std::string> segments = splitPath(uri);
    TrieNode* current = m_root;
    LocationConfig* bestMatch = current->location; // Usually the root location '/'

    for (size_t i = 0; i < segments.size(); ++i) {
        if (current->children.find(segments[i]) != current->children.end()) {
            current = current->children[segments[i]];
            if (current->location != NULL) {
                bestMatch = current->location; // Update Best Match
            }
        } else {
            break; // No more matching segments
        }
    }
    return bestMatch;
}
