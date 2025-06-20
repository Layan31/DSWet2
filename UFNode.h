
#ifndef WET2_UFNODE_H
#define WET2_UFNODE_H

template <typename V>
class UFNode {
public:
    UFNode<V>* parent;
    int r;
    V* value;

    explicit UFNode(V* val) : parent(this), r(0), value(val) {};

    ~UFNode();
};

template <typename V>
UFNode<V>::~UFNode() {
    delete value;
}

#endif //WET2_UFNODE_H
