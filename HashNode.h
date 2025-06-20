#pragma once
template <typename K, typename V>
class HashNode {
public:
    K key;
    V* value;
    HashNode* next;
    HashNode* prev;
    
    HashNode(K key, V* value) : key(key), value(value), next(nullptr), prev(nullptr) {}
    ~HashNode();
};

template <typename K, typename V>
HashNode<K, V>::~HashNode() {
    delete value;
}

