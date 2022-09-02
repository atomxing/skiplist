## skiplist c++

---
### KV存储引擎

基于跳表实现的轻量级键值型存储引擎，使用c++11实现。
支持插入数据、删除数据、查询数据、范围查询、数据显示、数据存盘、文件加载、数据库大小显示。

随机读写测试，该项目可以处理写请求数（QPS)：24w，每秒可处理读请求数：18w。


---
### 项目文件

* main.cpp 实现的跳表例子
* Node.h 节点类
* SkipList.h 跳表类
* stress_test.sh 压力测试
* READEME.md 介绍
* makefile 编译脚本
* /bin 可执行文件目录
* /store 数据存盘目录


---
### 接口

* insertElement（插入数据）
* deleteElement（删除数据）
* searchElement（查询数据）
* search_range （范围查找）
* displayList（显示跳表）
* dumpFile（数据存盘）
* loadFile（加载数据）
* size（返回跳表大小）


---
### 运行
```shell
make
./bin/main
```

```shell
sh stress_test.sh
```


---
thanks to youngyangyang04