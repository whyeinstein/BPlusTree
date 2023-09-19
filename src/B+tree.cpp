// #include <boost/archive/binary_iarchive.hpp>
// #include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/export.hpp>
#include <cstdint>
#include <string>

#include "B+tree.h"

BOOST_CLASS_EXPORT(Node<int>);
BOOST_CLASS_EXPORT(BPlusTree<int>);
BOOST_CLASS_EXPORT(LeafNode<int>);
BOOST_CLASS_EXPORT(InterNode<int>);

BOOST_CLASS_EXPORT(Node<std::string>);
BOOST_CLASS_EXPORT(LeafNode<std::string>);
BOOST_CLASS_EXPORT(BPlusTree<std::string>);
BOOST_CLASS_EXPORT(InterNode<std::string>);

extern std::vector<BPlusTree<int>*> forest;
extern std::vector<BPlusTree<std::string>*> sforest;

//序列化int
void Save(BPlusTree<int>* saveTree) {
  std::string treeFileName;
  std::cout << "请输入文件名" << std::endl;
  std::cin >> treeFileName;
  treeFileName = treeFileName + ".txt";
  std::ofstream fout(treeFileName, std::ios::app);
  if (fout.is_open()) {
    boost::archive::text_oarchive oa(fout);
    oa << saveTree;
    fout.close();
  } else {
    std::cout << "open file failed." << std::endl;
    exit(-1);
  }
}

// //序列化string
void Save(BPlusTree<std::string>* saveTree) {
  std::cout << "请输入文件名" << std::endl;
  std::string treeFileName;
  std::cin >> treeFileName;
  treeFileName = treeFileName + ".txt";
  std::ofstream fout(treeFileName, std::ios::app);
  if (fout.is_open()) {
    boost::archive::text_oarchive oa(fout);
    oa << saveTree;
    fout.close();
  } else {
    std::cout << "open file failed." << std::endl;
    exit(-1);
  }
}

//反序列化(flag=1代表int，flag=2代表string)
void Load(int flag) {
  BPlusTree<int>* loadInt;
  BPlusTree<std::string>* loadString;
  std::string openFile;

  std::cout << "请输入文件名" << std::endl;
  std::cin >> openFile;

  openFile = openFile + ".txt";
  std::ifstream ifs(openFile);
  if (!ifs.is_open()) {
    std::cerr << "Failed to open file for deserialization." << std::endl;
    return;
  }

  // 反序列化对象
  boost::archive::text_iarchive ia(ifs);
  if (flag == 1) {
    ia >> loadInt;
    loadInt->LevelTraversal();
    forest.push_back(loadInt);
    std::cout << "反序列化成功" << std::endl;
    std::cout << "树" << forest.size() << "（int）创建" << std::endl;
  } else if (flag == 2) {
    ia >> loadString;
    loadString->LevelTraversal();
    sforest.push_back(loadString);
    std::cout << "反序列化成功" << std::endl;
    std::cout << "树" << sforest.size() << "（string）创建" << std::endl;
  }

  // 关闭文件
  ifs.close();
}

void thread_entry() {
  BPlusTree<int> treeTwo;
  double pTime1 = 0, pTime2 = 0;
  clock_t start, end;
  std::ofstream ofs;
  std::string s;

  int j = 0;

  s = "performance1.txt";

  ofs.open(s, std::ios::app);
  for (j = 515; j <= 1000; j += 5) {
    treeTwo.SetMaxKey(j - 1);
    treeTwo.SetMinKey(j / 2);

    start = clock();
    for (int i = 1; i <= 10000000; i++) {
      treeTwo.Insert(i, i);
    }
    end = clock();
    pTime1 = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "插入花费时间：" << pTime1 << std::endl;

    start = clock();
    treeTwo.DeleteAll();
    end = clock();
    pTime2 = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "删除花费时间：" << pTime2 << std::endl;

    ofs << "度数" << j << "  insert:" << pTime1 << "  delete:" << pTime2
        << std::endl;
    std::cout << std::endl;
  }
  ofs.close();
}

void Tree(BPlusTree<int>* x) {
  clock_t start, end;

  std::vector<int> randomIns;
  int iKEY, iValue, delKey;

  int num = 0;

  while (1) {
    int choice;
    std::cout << "请选择操作：1插入，2范围查询，3删除，4打印，5清空整颗树,"
                 "6顺序插入1～n,7随即插入1～n，8序列化，100为测试，0退出"
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
          x->Insert(iKEY, iValue);
          std::cout << "插入成功" << std::endl;
          x->LevelTraversal();
          std::cout << "输入键值对,（0,0）退出" << std::endl;
          std::cin >> iKEY >> iValue;
        }
        break;
      case 2:
        int minKey, maxKey;
        std::cout << "请输入查询范围" << std::endl;
        std::cin >> minKey >> maxKey;
        x->RangeQuery(minKey, maxKey);
        break;
      case 3:
        std::cout << "请输入要删除的key(0退出)" << std::endl;
        std::cin >> delKey;
        while (delKey != 0) {
          bool success = x->Delete(delKey);
          if (success == true) {
            std::cout << "删除后的树：" << std::endl;
            x->LevelTraversal();
          } else {
            std::cout << "未找到，删除失败！" << std::endl;
          }
          std::cout << "请输入要删除的key(0退出)" << std::endl;
          std::cin >> delKey;
        }
        break;
      case 4:
        std::cout << "层序遍历" << std::endl;
        x->LevelTraversal();
        break;
      case 5:
        start = clock();
        x->DeleteAll();
        end = clock();
        std::cout << "耗时：" << double(end - start) / CLOCKS_PER_SEC << "s"
                  << std::endl;
        break;
      case 6:
        std::cin >> num;
        start = clock();
        for (int i = 1; i <= num; i++) {
          x->Insert(i, i);
        }
        end = clock();
        std::cout << "耗时：" << double(end - start) / CLOCKS_PER_SEC << "s"
                  << std::endl;
        break;
      case 7:
        int n;
        std::cout << "请输入随机个数" << std::endl;
        std::cin >> n;
        for (int i = 0; i < n; i++) {
          randomIns.push_back(i + 1);
        }
        for (int i = 0; i < n; i++) {
          std::cout << randomIns[i] << " ";
        }
        std::cout << std::endl;
        srand(time(0));
        std::random_shuffle(randomIns.begin(), randomIns.end());
        for (int i = 0; i < n; i++) {
          std::cout << randomIns[i] << " ";
        }
        std::cout << std::endl;
        for (int i = 0; i < n; i++) {
          x->Insert(randomIns[i], 1);
        }
        break;
      case 8:
        Save(x);
        break;

      //性能测试
      case 100:
        thread_entry();
      default:
        break;
    }
  }
}

void TreeS(BPlusTree<std::string>* sx) {
  clock_t start, end;

  std::string skey;
  std::string minKey, maxKey;
  std::string delKey;
  std::vector<int> randomIns;
  int iValue;

  while (1) {
    int choice;
    std::cout << "请选择操作：1插入，2范围查询，3删除，4打印，5清空整颗树,"
                 "6序列化，0退出"
              << std::endl;
    std::cin >> choice;
    if (choice == 0) {
      break;
    }

    switch (choice) {
      case 1:
        std::cout << "输入键值对,（,-1）退出" << std::endl;
        std::cin >> skey;
        std::cin >> iValue;
        while (iValue != -1) {
          sx->Insert(skey, iValue);
          std::cout << "插入成功" << std::endl;
          sx->LevelTraversal();
          std::cout << "输入键值对,（,-1）退出" << std::endl;
          std::cin >> skey >> iValue;
        }
        break;

      case 2:
        std::cout << "请输入查询范围" << std::endl;
        std::cin >> minKey;
        std::cin >> maxKey;
        sx->RangeQuery(minKey, maxKey);
        break;
      case 3:
        std::cout << "请输入要删除的key(0退出)" << std::endl;
        std::cin >> delKey;
        while (delKey != "exit") {
          bool success = sx->Delete(delKey);
          if (success == true) {
            std::cout << "删除后的树：" << std::endl;
            sx->LevelTraversal();
          } else {
            std::cout << "未找到，删除失败！" << std::endl;
          }
          std::cout << "请输入要删除的key(exit退出)" << std::endl;
          std::cin >> delKey;
        }
        break;
      case 4:
        std::cout << "层序遍历" << std::endl;
        sx->LevelTraversal();
        break;
      case 5:
        start = clock();
        sx->DeleteAll();
        end = clock();
        std::cout << "耗时：" << double(end - start) / CLOCKS_PER_SEC << "s"
                  << std::endl;
        break;
      case 6:
        Save(sx);
        break;
      default:
        break;
    }
  }
}
