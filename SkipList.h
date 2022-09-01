#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include "node.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <fstream>

#define STORE_FILE "store/dumpFile"

// 锁
std::mutex mtx;
std::string delimiter = ":";

// 跳表类
template <typename K, typename V>
class SkipList {

public:
    SkipList(int);
    ~SkipList();
    int get_random_level();
    Node<K, V>* create_node(K, V, int);
    int insert_element(K, V);
    void display_list();
    bool search_element(K);
    void delete_element(K);
    void dump_file();
    void load_file();
    int size();

private:
    void get_key_value_from_string(const std::string& str, std::string* key, std::string* value);
    bool is_valid_string(const std::string& str);

private:
    // 这个跳表的最大层次
    int _max_level;

    // 当前所在层
    int _skip_list_level;

    // 指向整个链表的头节点的指针（不是头节点，_header->next才是头节点）
    Node<K, V> *_header;

    // 文件操作 ofstream ifstream
    std::ofstream _file_writer;
    std::ifstream _file_reader;

    // 跳表当前的节点个数
    int _element_count;
};


// 构造一个新的节点
template<typename K, typename V>
Node<K, V>* SkipList<K, V>::create_node(const K k, const V v, int level) {
    Node<K, V> *n = new Node<K, V>(k, v, level);
    return n;
}



// 向跳表中插入节点
// return 1 该元素已存在
// return 0 插入成功
/*
                           +------------+
                           |  insert 50 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |                      insert +----+
level 3         1+-------->10+---------------> | 50 |          70       100
                                               |    |
                                               |    |
level 2         1          10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 1         1    4     10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 0         1    4   9 10         30   40  | 50 |  60      70       100
                                               +----+

*/
template<typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value) {

    // 为了满足多线程，要加锁
    mtx.lock();
    // _header是指向头节点的指针
    Node<K, V> *current = this->_header;

    // 创建update数组，并且初始化
    // update是一个数组，用于放置node->forward[i]（node不同级别的下一个节点），即以后应该被操作的节点。
    // 这里是什么原理呢：和单链表做类比，就是待插入位置的前一个节点prev，具体可以看OneNote笔记
    Node<K, V> *update[_max_level+1];
    memset(update, 0, sizeof(Node<K, V>*)*(_max_level+1));

    // 从跳表的最高层开始 不断找待插入的位置
    for(int i = _skip_list_level; i >= 0; i--) {
        // 当前节点（初始为头节点）的下一个节点 不为空
        // 并且 当前节点的下一个节点的值 小于要插入的节点指
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        // current节点后面就是待插入节点的位置，放到update数组中记录下来
        update[i] = current;
    }

    // 达到0级，即最底层，并将指针指向右边的节点，这是想要插入的键。
    current = current->forward[0];

    // 如果当前节点的键值等于搜索到的键值，我们就得到它
    if (current != NULL && current->get_key() == key) {
        std::cout << "key: " << key << ", exists" << std::endl;
        mtx.unlock();
        return 1;
    }

        // 如果current是NULL，意味着我们已经到达了本级的终点。
        // 如果当前的键不等于键，意味着我们必须在update[0]和当前节点之间插入节点。
    else if (current == NULL || current->get_key() != key ) {

        // 给节点生成一个随机等级
        int random_level = get_random_level();

        // 如果随机等级大于跳表的当前等级，则用头的指针初始化更新值。
        // 就是从之前的最高层 到 随机等级之间，这些层次都是空的，将头节点（最底层）作为待插入节点的prev节点
        if (random_level > _skip_list_level) {
            for (int i = _skip_list_level + 1; i < random_level + 1; i++) {
                update[i] = _header;
            }
            _skip_list_level = random_level;
        }

        // 创建新的节点
        Node<K, V>* inserted_node = create_node(key, value, random_level);

        // 插入节点
        // 类比单链表，上面update数组，获取到了待插入位置的前一个节点prev
        // inserted_node->next = prev->next   (next即forward) (prev即update)
        // prev->next = inserted_node
        for (int i = 0; i <= random_level; i++) {
            inserted_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = inserted_node;
        }
        std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
        _element_count ++;
    }
    // 操作完了后解锁
    mtx.unlock();
    return 0;
}


// 打印显示跳表
template<typename K, typename V>
void SkipList<K, V>::display_list() {

    std::cout << "\n*****Skip List*****"<<"\n";
    // 从最底层开始输出
    for (int i = 0; i <= _skip_list_level; i++) {
        Node<K, V> *node = this->_header->forward[i];
        std::cout << "Level " << i << ": ";
        while (node != NULL) {
            std::cout << node->get_key() << ":" << node->get_value() << ";";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }
}


// 加载数据，不断调用insert，将文件里的单链表插入成跳表
template<typename K, typename V>
void SkipList<K, V>::load_file() {

    _file_reader.open(STORE_FILE);
    std::cout << "load_file-----------------" << std::endl;
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }
        insert_element(*key, *value);
        std::cout << "key:" << *key << "value:" << *value << std::endl;
    }
    _file_reader.close();
}





#endif //SKIPLIST_SKIPLIST_H
