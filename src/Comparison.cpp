#include <bits/types/clock_t.h>
#include <stx/btree_map.h>

#include <ctime>
#include <iostream>
#include <string>
#include <utility>

#include "B+tree.h"

void compare() {
  clock_t start, end;

  std::cout << "---------STX B+ Tree---------" << std::endl;
  stx::btree_map<int, int> btree;
  start = clock();
  for (int i = 1; i <= 10000000; i++) {
    btree.insert(std::make_pair(i, i));
  }
  end = clock();
  std::cout << "插入花费时间：" << double(end - start) / CLOCKS_PER_SEC
            << std::endl;
  std::cout << "中间节点max：" << btree.innerslotmax << std::endl;
  std::cout << "叶子节点max:" << btree.leafslotmax << std::endl;
  start = clock();
  btree.clear();
  end = clock();
  std::cout << "清空花费时间：" << double(end - start) / CLOCKS_PER_SEC
            << std::endl;

  std::cout << "---------My B+ Tree---------" << std::endl;
  BPlusTree<int> *myTree = new BPlusTree<int>;
  std::cout << "节点max:32" << std::endl;
  myTree->SetMaxKey(33 - 1);
  myTree->SetMinKey(33 / 2);
  start = clock();
  for (int i = 1; i <= 10000000; i++) {
    myTree->Insert(i, i);
  }
  end = clock();
  std::cout << "插入花费时间：" << double(end - start) / CLOCKS_PER_SEC
            << std::endl;
  start = clock();
  myTree->DeleteAll();
  end = clock();
  std::cout << "清空花费时间：" << double(end - start) / CLOCKS_PER_SEC
            << std::endl;
}
