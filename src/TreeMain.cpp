#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include "B+tree.h"

int main() {
  while (1) {
    //建树
    BPlusTree<int> x;
    std::vector<int> tempxxx;  //测试用
    int iKEY, iValue, delKey;
    int sss;
    int num = 0;
    int pro = 0;

    std::cout << "输入0退出，输入1开始" << std::endl;
    std::cin >> pro;
    if (pro == 0) {
      break;
    }
    std::cout << "输入MAX_KEYNUM" << std::endl;
    std::cin >> sss;
    x.SetMaxKey(sss - 1);
    x.SetMinKey(sss / 2);

    while (1) {
      int choice;
      std::cout << "请选择操作：1插入，2范围查询，3删除，4打印，5删除整颗树,6~"
                   "100为测试，0退出"
                << std::endl;
      std::cin >> choice;
      if (choice == 0) {
        break;
      }

      switch (choice) {
        case 1:
          std::cout << "输入键值对,（0,0）退出" << std::endl;
          std::cin >> iKEY >> iValue;
          while ((iValue != 0) && (iKEY != 0)) {
            x.Insert(iKEY, iValue);
            std::cout << "插入成功" << std::endl;
            x.LevelTraversal(x.GetRoot());
            std::cout << "输入键值对,（0,0）退出" << std::endl;
            std::cin >> iKEY >> iValue;
          }
          break;

        case 2:
          int minKey, maxKey;
          std::cout << "请输入查询范围" << std::endl;
          std::cin >> minKey >> maxKey;
          x.RangeQuery(minKey, maxKey);
          break;
        case 3:
          std::cout << "请输入要删除的key(0退出)" << std::endl;
          std::cin >> delKey;
          while (delKey != 0) {
            bool success = x.Delete(delKey);
            if (success == true) {
              std::cout << "删除后的树：" << std::endl;
              x.LevelTraversal(x.GetRoot());
            } else {
              std::cout << "未找到，删除失败！" << std::endl;
            }
            std::cout << "请输入要删除的key(0退出)" << std::endl;
            std::cin >> delKey;
          }
          break;
        case 4:
          std::cout << "层序遍历" << std::endl;
          x.LevelTraversal(x.GetRoot());
          break;
        case 5:
          x.DeleteAll();
          break;
        case 6:
          x.Insert(1, 5);
          x.Insert(5, 8);
          x.Insert(6, 10);
          x.Insert(2, 12);
          x.Insert(4, 11);
          x.Insert(8, 17);
          x.Insert(9, 55);
          x.Insert(12, 9);
          x.Insert(3, 2);
          x.LevelTraversal(x.GetRoot());
          x.Insert(55, 7);
          x.LevelTraversal(x.GetRoot());
          x.Insert(67, 14);
          x.Insert(13, 56);
          x.Insert(555, 42);
          x.Insert(666, 1);
          x.Insert(454, 77);
          x.LevelTraversal(x.GetRoot());
          x.Insert(121, 77);
          x.Insert(22, 77);
          break;
        case 7:
          std::cin >> num;
          for (int i = 0; i < num; i++) {
            x.Insert(i, i);
          }
          break;
        case 8:
          int n;
          std::cout << "请输入随机个数" << std::endl;
          std::cin >> n;
          for (int i = 0; i < n; i++) {
            tempxxx.push_back(i + 1);
          }
          for (int i = 0; i < n; i++) {
            std::cout << tempxxx[i] << " ";
          }
          std::cout << std::endl;
          srand(2);
          std::random_shuffle(tempxxx.begin(), tempxxx.end());
          for (int i = 0; i < n; i++) {
            std::cout << tempxxx[i] << " ";
          }
          std::cout << std::endl;
          for (int i = 0; i < n; i++) {
            // std::cout << tempxxx[i] << std::endl;
            if (i == 14) {
              // x.LevelTraversal(x.GetRoot());
            }
            // x.LevelTraversal(x.GetRoot());

            x.Insert(tempxxx[i], 1);
          }
          // x.LevelTraversal(x.GetRoot());
          break;
        case 9:
          // x.Insert(16, 1);
          x.Insert(5, 5);
          x.Insert(11, 11);
          x.Insert(12, 12);
          x.Insert(16, 16);
          x.Insert(15, 15);
          x.Insert(17, 17);
          x.Insert(18, 18);
          x.Insert(2, 2);
          x.Insert(7, 7);
          x.Insert(10, 10);
          x.Insert(4, 4);
          x.Insert(8, 8);
          x.Insert(9, 9);
          x.Insert(3, 3);
          x.Insert(1, 1);
          x.Insert(13, 13);
          x.Insert(6, 6);
          x.Insert(14, 14);
        default:
          break;
      }
    }
  }
}