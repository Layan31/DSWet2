#ifndef DYNAMICHASHTABLE_H
#define DYNAMICHASHTABLE_H

#include "HashNode.h"
#include <new>

class KeyNotInHash {};

template <typename K, typename V>
class DynamicHashTable {
private:
    HashNode<K, V>** table;
    int capacity;
    int size;
    void resize_table();

public:
    explicit DynamicHashTable(int capacity = 11); // Default to a small prime number
    ~DynamicHashTable();
    bool insert(K key, V* value);
    V* find(K key);
    bool remove(K key);
    void remove_node(HashNode<K, V>* node);
    int get_size() const;
    int get_capacity() const;
};

template <typename K, typename V>
DynamicHashTable<K, V>::DynamicHashTable(int capacity) : capacity(2*capacity + 1), size(0) {
    table = new HashNode<K, V>*[this->capacity]();
}

template <typename K, typename V>
DynamicHashTable<K, V>::~DynamicHashTable() {
    for (int i = 0; i < capacity; ++i) {
        HashNode<K, V>* entry = table[i];
        while (entry != nullptr) {
            HashNode<K, V>* prev = entry;
            entry = entry->next;
            delete prev;
        }
    }
    delete[] table;
}

template <typename K, typename V>
void DynamicHashTable<K, V>::resize_table() {
    int old_capacity = capacity;
    capacity = 2 * capacity + 1;
    auto** new_table = new HashNode<K, V>*[capacity]();
    for(int i = 0; i < capacity; i++){
        new_table[i] = nullptr; 
    }

    for (int i = 0; i < old_capacity; ++i) {
        HashNode<K, V>* entry = table[i];
        while (entry != nullptr) {
            HashNode<K, V>* next = entry->next;
            int hash_value = entry->key % capacity;
            entry->next = new_table[hash_value];
            if (new_table[hash_value] != nullptr) {
                new_table[hash_value]->prev = entry;
            }
            entry->prev = nullptr;
            new_table[hash_value] = entry;
            entry = next;
        }
    }
    delete[] table;
    table = new_table;
}

template <typename K, typename V>
bool DynamicHashTable<K, V>::insert(K key, V* value) {
    try{
        if (size >= capacity) {
            resize_table();
        }

        int hash_value = key % capacity;
        auto* new_node = new HashNode<K, V>(key, value);
        new_node->next = table[hash_value];
        if (table[hash_value] != nullptr) {
            table[hash_value]->prev = new_node;
        }
        table[hash_value] = new_node;
        ++size;
    }
    catch(...){
        throw std::bad_alloc();
    }
    return true;
}

template <typename K, typename V>
V* DynamicHashTable<K, V>::find(K key) {
    int hash_value = key % capacity;
    HashNode<K, V>* entry = table[hash_value];
    while (entry != nullptr && entry->key != key) {
        entry = entry->next;
    }
    if (!entry) {
        return nullptr;
    }
    return entry->value;
}

template <typename K, typename V>
bool DynamicHashTable<K, V>::remove(K key) {
    int hash_value = key % capacity;
    if (table[hash_value] == nullptr) {
        return false;
    }

    remove_node(table[hash_value]);

    return true;
}

template <typename K, typename V>
void DynamicHashTable<K, V>::remove_node(HashNode<K, V>* node) {
    int key = node->key;
    int hash_value = key % capacity;
    if (table[hash_value] == node) {
        table[hash_value] = node->next;
        if (node->next != nullptr) {
            node->next->prev = nullptr;
        }
    }
    else {
        // node has a prev at least
        node->prev->next = node->next;
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        }
    }

    size--;
    delete node;
}

template <typename K, typename V>
int DynamicHashTable<K, V>::get_size() const {
    return size;
}

template <typename K, typename V>
int DynamicHashTable<K, V>::get_capacity() const {
    return capacity;
}

#endif // DYNAMICHASHTABLE_H
