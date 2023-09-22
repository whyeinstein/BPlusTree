#include <pthread.h>

#include <mutex>
#include <shared_mutex>
#include <thread>

#include "B+tree.h"
using namespace std;

int lockC = 0;
int unLock = 0;
int lockCs = 0;
int unLocks = 0;

void Reader(int id, BPlusTree<int>*& ccTree) {
  shared_lock<shared_mutex> lock(ccTree->treeMutex);

  // 当前时间
  auto currentTimePoint = chrono::system_clock::now();
  time_t currentTime = chrono::system_clock::to_time_t(currentTimePoint);
  cout << "rid:" << id << "Current time: " << ctime(&currentTime);

  cout << "Reader " << id << " reads data: " << ccTree->GetMaxKey() << endl;
  this_thread::sleep_for(chrono::milliseconds(10000));
}

void Writer(int id, BPlusTree<int>*& ccTree) {
  unique_lock<shared_mutex> lock(ccTree->treeMutex);

  // 当前时间
  auto currentTimePoint = chrono::system_clock::now();
  time_t currentTime = chrono::system_clock::to_time_t(currentTimePoint);
  cout << "wid:" << id << "Current time: " << ctime(&currentTime);

  ccTree->SetMaxKey(ccTree->GetMaxKey() + 1);
  cout << "Writer " << id << " writes data: " << ccTree->GetMaxKey() << endl;
  this_thread::sleep_for(chrono::milliseconds(5000));
}

void ThreadReg() {
  //清屏
  std::cout << "\x1B[2J\x1B[H";

  BPlusTree<int>* ccTree = new BPlusTree<int>;
  ccTree->SetMaxKey(4);
  ccTree->SetMinKey(2);
  thread Reader1(Reader, 1, ref(ccTree));
  thread writer1(Writer, 1, ref(ccTree));
  std::this_thread::sleep_for(chrono::milliseconds(2000));

  thread Reader2(Reader, 2, ref(ccTree));

  Reader1.join();
  writer1.join();
  Reader2.join();
}
