
#include <gtest/gtest.h>

#include "B+tree.h"

TEST(SquareRootTest, PositiveNos)  // normal cases
{
  BPlusTree<int> x;
  ASSERT_EQ(true, x.Insert(1, 1));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}