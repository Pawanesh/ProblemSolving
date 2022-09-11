#include <iostream>
#include <vector>
#include <memory>

const int SIZE = 26;

struct TrieNode {
    using Ptr = std::shared_ptr<TrieNode>;
    TrieNode():
        children(SIZE, TrieNode::Ptr()) {

    }
    std::vector<TrieNode::Ptr> children;
    bool end = false;
};

struct Trie {
    Trie () : 
        rootPtr(std::make_shared<TrieNode>()) {
        
    }
    void insert(const std::string &key) {
        TrieNode::Ptr ptr = rootPtr;
        for (auto ch : key) {
            int index  = ch - 'a';
            if (ptr->children[index]) {
                ptr = ptr->children[index];
            }
            else {
                ptr->children[index] = std::make_shared<TrieNode>();
                ptr = ptr->children[index];
            }
        }
        ptr->end = true;
    }

    bool find(const std::string &key) {
        TrieNode::Ptr ptr = rootPtr;
        for (auto ch : key) {
            int index  = ch - 'a';
            if (!ptr->children[index])
                return false;
            
            ptr = ptr->children[index];
        }

        return ptr->end;
    }

    TrieNode::Ptr rootPtr;
};

int main() {
    std::cout << "Tries example" << std::endl;
    Trie trie;
    trie.insert("the");
    trie.insert("a");
    trie.insert("there");
    trie.insert("answer");
    trie.insert("any");
    trie.insert("by");
    trie.insert("bye");
    trie.insert("their");

    std::cout << std::boolalpha << trie.find("the") << std::endl;
    std::cout << std::boolalpha << trie.find("these") << std::endl;
    std::cout << std::boolalpha << trie.find("their") << std::endl;
    std::cout << std::boolalpha << trie.find("thaw") << std::endl;
    
    return 0;
}