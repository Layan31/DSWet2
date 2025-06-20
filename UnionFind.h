
#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include "UFNode.h"
#include "DynamicHashTable.h"

template<typename F, typename S>
struct Tuple {
    F first;
    S second;

    Tuple(F f, S s) : first(f), second(s) {};
};

/**
 * @tparam V Additional value stored in each union find node, and is assumed to be up to date only in the root of each
 * tree in the union find structure.
 */
template <typename K,typename V>
class UnionFind {
private:
    DynamicHashTable<K, UFNode<V>> nodes;

    Tuple<UFNode<V>*, int> find_tuple(K key);

    UFNode<V>* find_root(K key);

public:
    explicit UnionFind(int capacity = 11);
    ~UnionFind() = default;

    bool insert(K key, V* val);
    /**
     * Merge key2 into key1.
     */
    bool merge(K key1, K key2);
    /**
     * @return Up to date value stored in the root that key belongs to.
     */
    V* get_root_val(K key);
    /**
     * @return Sum of r values from the root of the tree that key belongs to, until key.
     */
    int path_to(K key);
    /**
     * Increase the path of all nodes in the tree that key is the root of to by 1.
     */
    bool update_path_from(K key, int r);

};

template <typename K,typename V>
UnionFind<K, V>::UnionFind(int capacity) : nodes(capacity) {}

template <typename K,typename V>
bool UnionFind<K, V>::insert(K key, V* val) {
    return nodes.insert(key, new UFNode<V>(val));
}

template <typename K,typename V>
/**
 * @return A tuple containing the value of the root that key belongs to, as well as the total t value accumulated on the
 * path from key to said root. Performs all the path corrections.
 */
Tuple<UFNode<V>*, int> UnionFind<K, V>::find_tuple(K key) {
    UFNode<V>* node = nodes.find(key);
    if (!node) {
        return Tuple<UFNode<V>*, int>(nullptr, -1);
    }

    int total_r = 0;

    UFNode<V>* current = node;

    while (current->parent != current) {
        total_r += current->r;
        current = current->parent;
    }

    UFNode<V>* root = current;
    int r = total_r + root->r;

    current = node;

    while (current->parent != current) {
        total_r -= current->r;
        current->r += total_r;
        UFNode<V>* next = current->parent;
        current->parent = root;
        current = next;
    }

    return Tuple<UFNode<V>*, int>(root, r);;
}

template <typename K,typename V>
UFNode<V>* UnionFind<K, V>::find_root(K key) {
    return find_tuple(key).first;
}

template <typename K,typename V>
bool UnionFind<K, V>::merge(K key1, K key2) {
    UFNode<V>* root1 = find_root(key1);
    UFNode<V>* root2 = find_root(key2);

    if (!root1 || !root2) {
        return false;
    }

    // Unite.
    root2->parent = root1;

    // Correct r value of 2nd root.
    root2->r -= root1->r;

    return true;
}

template <typename K,typename V>
V* UnionFind<K, V>::get_root_val(K key) {
    UFNode<V>* root = find_root(key);
    if (!root) {
        return nullptr;
    }
    return root->value;
}

template <typename K,typename V>
int UnionFind<K, V>::path_to(K key) {
    return find_tuple(key).second;
}

template <typename K,typename V>
bool UnionFind<K, V>::update_path_from(K key, int r) {
    UFNode<V>* node = nodes.find(key);
    if (!node) {
        return false;
    }
    node->r += r;
    return true;
}

#endif //WET2_UNIONFIND_H
