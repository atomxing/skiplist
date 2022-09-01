#ifndef SKIPLIST_NODE_H
#define SKIPLIST_NODE_H

// 节点类 模板实现
template<typename K, typename V>
class Node {

public:

    Node() {}

    Node(K k, V v, int);

    ~Node();

    // 获取key 关键词
    K get_key() const;

    // 获取value 值
    V get_value() const;

    void set_value(V);

    // 线性数组，用于保存指向不同级别的下一个节点的指针
    // 意思就是对于一个节点来说，可能会在不同层次然后不同层的下一个节点是什么就存在这个二维数组里面（可以对照OneNote笔记的图片看）
    Node<K, V> **forward;
    // 一级指针等同于数组（名），即一维数组；二级指针等同于指针数组，即二维数组

    // 代表节点所在层次
    int node_level;

private:
    K key;
    V value;
};


template<typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level) {
    this->key = k;
    this->value = v;
    this->node_level = level;

    // level + 1, because array index is from 0 - level
    this->forward = new Node<K, V>*[level+1];

    // Fill forward array with 0(NULL)
    memset(this->forward, 0, sizeof(Node<K, V>*)*(level+1));
}

template<typename K, typename V>
Node<K, V>::~Node() {
    delete []forward;
};

template<typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
}

template<typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
};
template<typename K, typename V>
void Node<K, V>::set_value(V value) {
    this->value=value;
}


#endif //SKIPLIST_NODE_H
