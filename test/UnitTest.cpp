
#include <gtest/gtest.h>

#include "../include/B+tree.h"

class InsAndDelTest : public ::testing::Test {
 public:
  BPlusTree<int> *treeOne = new BPlusTree<int>;
  void SetUp() override {
    treeOne->SetMaxKey(4);
    treeOne->SetMinKey(2);
  }
  void TearDown() override { delete treeOne; }
};

TEST_F(InsAndDelTest, insertTest) {
  std::vector<int> ans;
  treeOne->Insert(1, 1);
  ans.push_back(1);
  ASSERT_EQ(treeOne->LevelTraversal(), ans) << "test: insert a null tree";

  treeOne->Insert(5, 1);
  treeOne->Insert(6, 1);
  treeOne->Insert(2, 1);
  ans = {1, 2, 5, 6};
  ASSERT_EQ(treeOne->LevelTraversal(), ans) << "test: insert without split";

  treeOne->Insert(4, 1);
  ans = {5, 1, 2, 4, 5, 6};
  ASSERT_EQ(treeOne->LevelTraversal(), ans) << "test: insert with leaf split";

  treeOne->Insert(8, 1);
  treeOne->Insert(9, 1);
  treeOne->Insert(12, 1);
  treeOne->Insert(3, 2);
  treeOne->Insert(55, 7);
  treeOne->Insert(67, 14);
  treeOne->Insert(13, 56);
  treeOne->Insert(555, 42);
  treeOne->Insert(666, 1);
  treeOne->Insert(454, 77);
  treeOne->Insert(121, 77);
  treeOne->Insert(22, 77);
  treeOne->Insert(33, 33);
  ans = {22, 5,  9,  55, 555, 1,  2,  3,   4,   5,   6,  8,
         9,  12, 13, 22, 33,  55, 67, 121, 454, 555, 666};
  ASSERT_EQ(treeOne->LevelTraversal(), ans)
      << "test: insert with top node split";

  treeOne->Insert(44, 33);
  treeOne->Insert(37, 33);
  treeOne->Insert(38, 33);
  treeOne->Insert(66, 33);
  treeOne->Insert(68, 33);
  treeOne->Insert(56, 33);
  ans = {22, 67, 5,  9,  38, 55, 121, 555, 1,  2,  3,  4,  5,   6,   8,   9,
         12, 13, 22, 33, 37, 38, 44,  55,  56, 66, 67, 68, 121, 454, 555, 666};
  ASSERT_EQ(treeOne->LevelTraversal(), ans)
      << "test: insert with internal node split";
};

TEST_F(InsAndDelTest, deleteTest) {
  std::vector<int> ans = {22, 67, 5,  9,  38, 55, 121, 555, 1,   2,  3,
                          4,  5,  6,  8,  9,  12, 13,  22,  33,  37, 38,
                          44, 55, 56, 66, 67, 68, 121, 454, 555, 666};
  ;
  std::vector<int> ins = {1,   5,   6,   2,   4,  8,  9,  12, 3,  55, 67, 13,
                          555, 666, 454, 121, 22, 33, 44, 37, 38, 66, 68, 56};
  for (int m = 0; m < 24; m++) {
    treeOne->Insert(ins[m], 1);
  }
  treeOne->Delete(22);
  ans = {33, 67, 5,  9,  38, 55, 121, 555, 1,  2,  3,  4,   5,   6,   8,  9,
         12, 13, 33, 37, 38, 44, 55,  56,  66, 67, 68, 121, 454, 555, 666};
  ASSERT_EQ(treeOne->LevelTraversal(), ans) << "test: delete in path";

  treeOne->Delete(33);
  ans = {67, 5,  9,  37, 55, 121, 555, 1,  2,  3,  4,   5,   6,   8,  9,
         12, 13, 37, 38, 44, 55,  56,  66, 67, 68, 121, 454, 555, 666};
  ASSERT_EQ(treeOne->LevelTraversal(), ans)
      << "test: delete with leafnode and internal node combine";

  treeOne->Delete(1);
  ans = {67, 5,  9,  37, 55, 121, 555, 2,  3,  4,  5,   6,   8,   9,
         12, 13, 37, 38, 44, 55,  56,  66, 67, 68, 121, 454, 555, 666};
  ASSERT_EQ(treeOne->LevelTraversal(), ans) << "test: delete only in leafnode";

  treeOne->Delete(67);
  ans = {55, 5,  9,  37, 68, 555, 2,  3,  4,  5,   6,   8,   9,
         12, 13, 37, 38, 44, 55,  56, 66, 68, 121, 454, 555, 666};
  ASSERT_EQ(treeOne->LevelTraversal(), ans)
      << "test: delete and internal node borrow from  bro";

  treeOne->Delete(666);
  treeOne->Delete(68);
  ans = {55, 5,  9,  37, 66, 454, 2,  3,  4,  5,   6,   8,
         9,  12, 13, 37, 38, 44,  55, 56, 66, 121, 454, 555};
  ASSERT_EQ(treeOne->LevelTraversal(), ans)
      << "test: delete and leaf node borrow from left bro";

  treeOne->Delete(55);
  treeOne->Delete(5);
  treeOne->Delete(8);
  ans = {37, 6,  12, 56, 454, 2,  3,  4,   6,   9,
         12, 13, 37, 38, 44,  56, 66, 121, 454, 555};
  ASSERT_EQ(treeOne->LevelTraversal(), ans)
      << "delete and leaf node borrow from right bro";

  treeOne->Delete(12);
  ans = {6, 37, 56, 454, 2, 3, 4, 6, 9, 13, 37, 38, 44, 56, 66, 121, 454, 555};
  ASSERT_EQ(treeOne->LevelTraversal(), ans) << "delete and root node delete";

  treeOne->Delete(12);
  ans = {6, 37, 56, 454, 2, 3, 4, 6, 9, 13, 37, 38, 44, 56, 66, 121, 454, 555};
  ASSERT_EQ(treeOne->LevelTraversal(), ans) << "delete with no existence";

  treeOne->Delete(6);
  treeOne->Delete(37);
  treeOne->Delete(56);
  treeOne->Delete(454);
  treeOne->Delete(9);
  treeOne->Delete(38);
  treeOne->Delete(66);
  treeOne->Delete(4);
  treeOne->Delete(3);
  treeOne->Delete(121);
  treeOne->Delete(44);
  treeOne->Delete(2);
  treeOne->Delete(13);
  treeOne->Delete(555);
  ans = {};
  ASSERT_EQ(treeOne->LevelTraversal(), ans) << "delete to null tree";
};