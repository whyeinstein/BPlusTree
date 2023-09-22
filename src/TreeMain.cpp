#include <bits/types/clock_t.h>
#include <pthread.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "B+tree.h"

std::vector<BPlusTree<int> *> forest;
std::vector<BPlusTree<std::string> *> sforest;
int main() {
  while (1) {
    int exit = 0;
    int degree;

    std::cout << "树(int)总共：" << forest.size() << "棵" << std::endl;
    std::cout << "树(string)总共：" << sforest.size() << "棵" << std::endl;

    std::cout << "输入0退出，输入1创建树（int），输入2创建树（string），输入3操"
                 "作树（int），输入4操作树（string）,"
                 "\n输入5删除树（int），输入6删除树（string）,7反序列化(int),"
                 "8反序列化(string),9与STX B+Tree比较，10并发"
              << std::endl;

    std::cin >> exit;

    //清空
    if (exit == 0) {
      while (forest.size() != 0) {
        BPlusTree<int> *dx = forest[0];
        delete dx;
        forest.erase(forest.begin());
      }
      while (sforest.size() != 0) {
        BPlusTree<std::string> *dsx = sforest[0];
        delete dsx;
        sforest.erase(sforest.begin());
      }
      break;
    }
    BPlusTree<int> *x = new BPlusTree<int>;
    BPlusTree<std::string> *sx = new BPlusTree<std::string>;
    switch (exit) {
      case 1:
        delete sx;
        forest.push_back(x);
        std::cout << "输入MAX_KEYNUM" << std::endl;
        std::cin >> degree;
        x->SetMaxKey(degree - 1);
        x->SetMinKey(degree / 2);
        Tree(x);
        std::cout << "树" << forest.size() << "（int）创建" << std::endl;
        break;
      case 2:
        delete x;
        sforest.push_back(sx);
        std::cout << "输入MAX_KEYNUM" << std::endl;
        std::cin >> degree;
        sx->SetMaxKey(degree - 1);
        sx->SetMinKey(degree / 2);
        TreeS(sx);
        std::cout << "树" << forest.size() << "（string）创建" << std::endl;
        break;
      case 3:
        delete x;
        delete sx;
        std::cout << "输入树序号" << std::endl;
        std::cin >> degree;
        if ((degree > static_cast<int>(forest.size())) || (degree <= 0)) {
          std::cout << "没有这棵树" << std::endl;
        } else {
          x = forest[degree - 1];
          std::cout << "树" << forest.size() << "（int）操作" << std::endl;
          Tree(x);
        }

        break;
      case 4:
        delete x;
        delete sx;
        std::cout << "输入树序号" << std::endl;
        std::cin >> degree;
        if ((degree > static_cast<int>(sforest.size())) || (degree <= 0)) {
          std::cout << "没有这棵树" << std::endl;
        } else {
          sx = sforest[degree - 1];
          std::cout << "树" << sforest.size() << "（string）操作" << std::endl;
          TreeS(sx);
        }
        break;
      case 5:
        delete x;
        delete sx;
        std::cout << "输入树序号" << std::endl;
        std::cin >> degree;
        if ((degree > static_cast<int>(forest.size())) || (degree <= 0)) {
          std::cout << "没有这棵树" << std::endl;
        } else {
          x = forest[degree - 1];
          forest.erase(forest.begin() + degree - 1);
          delete x;

          std::cout << "树" << degree << "（int）删除" << std::endl;
        }
        break;
      case 6:
        delete x;
        delete sx;
        std::cout << "输入树序号" << std::endl;
        std::cin >> degree;
        if ((degree > static_cast<int>(sforest.size())) || (degree <= 0)) {
          std::cout << "没有这棵树" << std::endl;
        } else {
          sx = sforest[degree - 1];
          sforest.erase(sforest.begin() + degree - 1);
          delete sx;

          std::cout << "树" << degree << "（string）删除" << std::endl;
        }
        break;
      case 7:
        delete x;
        delete sx;
        Load(1);
        break;
      case 8:
        delete x;
        delete sx;
        Load(2);
        break;
      case 9:
        delete x;
        delete sx;
        compare();
        break;
      case 10:
        ThreadReg();
        break;
      default:
        break;
    }
  }
  return 0;
}