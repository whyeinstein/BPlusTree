#include <bits/types/clock_t.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <fstream>
#include <ostream>
#include <sstream>

#include "../include/B+tree.h"

class InsertPerformance : public ::testing::Test {
 public:
  BPlusTree<int> *treeTwo = new BPlusTree<int>;
  void SetUp() override {
    treeTwo->SetMaxKey(4);
    treeTwo->SetMinKey(2);
  }
  void TearDown() override { delete treeTwo; }
};

TEST_F(InsertPerformance, insertPerformanceTest) {
  std::string s1, s2, s;
  clock_t start;
  clock_t end;

  double pTime1 = 0, pTime2 = 0;

  for (int j = 3; j <= 3; j++) {
    treeTwo->SetMaxKey(j - 1);
    treeTwo->SetMinKey(j / 2);

    start = clock();
    for (int i = 1; i <= 10000000; i++) {
      treeTwo->Insert(i, i);
    }
    end = clock();
    pTime1 = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "插入花费时间：" << pTime1 << std::endl;

    start = clock();
    treeTwo->DeleteAll();
    end = clock();
    pTime2 = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "删除花费时间：" << pTime2 << std::endl;

    std::cout << std::endl;
  }
  // close(fd);
};