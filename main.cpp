#include <iostream>
#include "SkipList.h"
#define FILE_PATH "./store/dumpFile"

int main() {

    SkipList<std::string, std::string> skipList(6);
    skipList.insert_element("1", "q");
    skipList.insert_element("5", "a");
    skipList.insert_element("6", "z");
    skipList.insert_element("9", "li");
    skipList.insert_element("12", "hao");
    skipList.insert_element("18", "wang");
    skipList.insert_element("18", "wang");
    skipList.insert_element("18", "wu");
    skipList.insert_element("20", "hello word!");
    skipList.insert_element("64", "竹杖芒鞋轻胜马");
    skipList.insert_element("65", "谁怕？");
    skipList.insert_element("66", "一蓑烟雨任平生");

    skipList.search_range("4", "10");

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.dump_file();

    // skipList.load_file();

    skipList.search_element("9");
    skipList.search_element("19");


    skipList.display_list();

    skipList.delete_element("5");
    skipList.delete_element("9");

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.display_list();
}
