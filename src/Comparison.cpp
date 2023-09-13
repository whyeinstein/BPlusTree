#include <bits/types/clock_t.h>
#include <stx/btree_map.h>

#include <ctime>
#include <iostream>
#include <string>
#include <utility>

void compare() {
  clock_t start, end;
  stx::btree_map<int, char> btree;
  //   stx::btree_map<int, std::string, std::less<int>,
  //                  stx::btree_default_map_traits<int, std::string, 4>>
  //       btree;
  start = clock();
  for (int i = 1; i < 10000000; i++) {
    btree.insert(std::make_pair(i, 'a'));
  }
  end = clock();

  //   for (auto it = btree.begin(); it != btree.end(); ++it) {
  //     std::cout << "Key: " << it->first << ", Value: " << it->second <<
  //     std::endl;
  //   }

  std::cout << "插入花费时间：" << double(end - start) / CLOCKS_PER_SEC
            << std::endl;
  std::cout << btree.innerslotmax << std::endl;
  std::cout << btree.leafslotmax << std::endl;
}
