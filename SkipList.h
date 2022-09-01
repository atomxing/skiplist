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



#endif //SKIPLIST_SKIPLIST_H
