#ifndef HELLO_H
#define HELLO_H

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <map>
#include <ostream>
#include <queue>
#include <string>
#include <vector>

/*-------------------------Node类定义---------------------------*/
template <typename T>
class Node {
 public:
  Node();
  virtual ~Node(){};  // virtual ???

  //获取修改有效数据个数
  int GetCount() { return m_Count; }
  void SetCount(int i) { m_Count = i; }

  //获取设置父亲节点
  Node* GetFather() { return m_Father; }
  void SetFather(Node* father) { m_Father = father; }

  //设置中间节点或叶子节点
  int GetNodeType() { return nodeType; }
  void SetNodeType(int i) { nodeType = i; }

  //根据位置找到键值
  T GetKeyValue(int i) const { return m_KeyValues[i]; }
  //插入键值
  void SetKeyValue(int i, T key);
  //替换键值
  void ReplaceKeyValue(int i, T key);

  //删除节点时寻找可以借的兄弟
  Node<T>* GetBro(int& leftOrRight, int ORDER);

  virtual void SetChild(int i, Node* pointer) {}
  virtual Node<T>* GetChild(int i) { return NULL; }
  virtual Node<T>* GetRightBro() { return NULL; }
  virtual Node<T>* GetLeftBro() { return NULL; }

  //虚函数：用于叶子节点获取value
  virtual T GetValues(int i) { return 0; }

 protected:
  int m_Count;  //有效数据个数（中间节点：键个数，叶子结点：数据个数）
  int nodeType;  //值为0代表根节点，值为1代表中间节点，2代表叶子节点
  Node* m_Father;              //指向当前节点的父亲节点
  std::vector<T> m_KeyValues;  //键值
};

/*----------------------InterNode类定义------------------------*/
//内节点
template <typename T = int>
class InterNode : public Node<T> {
 public:
  InterNode() {}
  ~InterNode() {}

  Node<T>* GetChild(int i) { return m_Childs[i]; }  //获取孩子指针

  //中间节点插入函数（参数列表为插入的值和待插入节点指针）
  bool Insert(T value, Node<T>* pNode);

  //设置孩子指针
  void SetChild(int i, Node<T>* pointer);

  // 分裂结点 绑定继承类的基类指针会不会找不到该函数？？？
  T Split(InterNode<T>* parentNode, T key, int MAX_KEYNUM, int ORDER,
          int& flag);

  //删除中间节点
  void Delete(T key);

  //中间节点合并
  void Combine(Node<T>* pNode);

  //移动
  bool BorrowInterBro(InterNode<T>* pNode);

 private:
  std::vector<Node<T>*> m_Childs;
};

/*----------------------LeafNode类定义------------------------*/
//叶子节点
template <typename T = int>
class LeafNode : public Node<T> {
 public:
  LeafNode();
  ~LeafNode() {}

  //获取、设置右兄弟指针
  LeafNode<T>* GetRightBro() { return m_RightBro; }
  void SetRightBro(LeafNode<T>* rightBro) { m_RightBro = rightBro; }

  //获取设置左兄弟指针
  LeafNode<T>* GetLeftBro() { return m_LeftBro; }
  void SetLeftBro(LeafNode<T>* leftBro) { m_LeftBro = leftBro; }

  //获取、设置值
  T GetValues(int i) { return m_Values[i]; }
  void SetValues(int i, T value) { m_Values[i] = value; }

  //叶子节点插入函数
  bool Insert(T key, uint64_t);
  //叶子节点删除函数
  bool Delete(T value);
  //叶子节点合并
  void Combine(Node<T>* pNode);

  T Split(LeafNode<T>* pNode);  // 分裂结点

 private:
  LeafNode<T>* m_RightBro;         //右兄弟指针
  LeafNode<T>* m_LeftBro;          //左兄弟指针
  std::vector<uint64_t> m_Values;  //值（指针数据）
};

/*-----------------------BPlusTree类定义-------------------------*/
template <typename foo>
class BPlusTree {
 public:
  BPlusTree();
  ~BPlusTree();

  // 查找指定的数据
  bool Search(foo data, char* sPath);
  LeafNode<foo>* SearchLeafNode(foo key);         //用于插入查找
  LeafNode<foo>* SearchDelNode(foo key, int& i);  //用于删除查找

  // 插入指定的数据
  bool Insert(foo key, uint64_t value);
  //插入中间节点
  bool InsertInterNode(InterNode<foo>* pNode, foo key, Node<foo>* rightChild);

  // 删除指定的数据
  bool Delete(foo data);
  //递归删除中间节点
  bool DelInterNode(InterNode<foo>* pNode, foo key);
  //更新父亲键值
  void UpdateKey(foo key, Node<foo>* oldNode, int x);
  //删除时叶子节点借兄弟
  bool BorrowLeafBro(LeafNode<foo>* oldNode, LeafNode<foo>* pBro, foo key,
                     int leftOrRight);
  //删除所有
  void DeleteAll();

  //层序遍历
  void LevelTraversal(Node<foo>* pNode);

  //范围查询
  void RangeQuery(foo minKey, foo maxKey);

  // 获取和设置根结点
  Node<foo>* GetRoot() { return m_Root; }
  void SetRoot(Node<foo>* root) { m_Root = root; }

  //获取设置最大最小值
  void SetMaxKey(int i) { MAX_KEYNUM = i; }
  int GetMaxKey() { return MAX_KEYNUM; }
  void SetMinKey(int i) { ORDER = i; }
  int GetMinKey() { return ORDER; }

 private:
  LeafNode<foo>* m_LeafHead;  //叶子节点链表表头
  LeafNode<foo>* m_LeafTail;  //叶子节点链表表尾
  Node<foo>* m_Root;          //根节点
  int MAX_KEYNUM;             //最大键值数
  int ORDER;                  //最小数目
};

/*-------------------------Node函数定义---------------------------*/
template <typename T>
Node<T>::Node() {
  m_Count = 0;
  nodeType = 0;
  m_Father = NULL;
}
template <typename T>
void Node<T>::SetKeyValue(int i, T key) {
  m_KeyValues.insert(m_KeyValues.begin() + i, key);
}

template <typename T>
void Node<T>::ReplaceKeyValue(int i, T key) {
  m_KeyValues[i] = key;
}

/*----------------------InterNode函数定义------------------------*/
//设置孩子指针
template <typename T>
void InterNode<T>::SetChild(int i, Node<T>* pointer) {
  m_Childs.insert(m_Childs.begin() + i, pointer);
}

//中间节点插入操作
template <typename T>
bool InterNode<T>::Insert(T key, Node<T>* pNode) {
  //插入
  int i = 0, j = 0;
  //找到在当前节点应该插入的位置
  i = BinALG(this, this->GetCount(), key);
  for (i = 0; (i < (Node<T>::GetCount())) && (key > Node<T>::GetKeyValue(i));
       i++)
    ;

  Node<T>::SetKeyValue(i, key);
  SetChild(i + 1, pNode);  //插入指针(在键值位置下一个)
  pNode->SetFather(this);  //将插入关键字所在节点的父亲指针设置为当前指针

  //关键字数目加1,返回真
  Node<T>::SetCount(Node<T>::GetCount() + 1);
  return true;
}

/* 分裂中间结点
分裂中间结点和分裂叶子结点完全不同，因为中间结点不仅有2V键，还有2V+1指针，如果单纯地一分为2，指针将无法分
配。 因此根据http://www.seanster.com/BplusTree/BplusTree.html ，分裂中
间结点的算法是： 根据要插入的键key判断：
(1)如果key小于第V个键，则把第V个键提出来,其左右的键分别分到两个结点中
(2) 如果key大于第V+1个键，则把第V+1个键提出来,其左右的键分别分到两个结点中
(3)如果key介于第V和V+1个键之间，则把key作为
要提出的键，原来的键各分一半到两个结点中
提出来的RetKey作用是便于后续插入到祖先结点
*/
template <typename T>
T InterNode<T>::Split(InterNode<T>* pNode, T key, int MAX_KEYNUM, int ORDER,
                      int& flag)  // key是新插入的值，pNode是分裂结点
{
  int i = 0, j = 0, mm = 0;

  // 如果要插入的键值在第V和V+1个键值中间，需要翻转一下，因此先处理此情况
  if ((key > this->GetKeyValue(ORDER - 1)) &&
      (key < this->GetKeyValue(ORDER))) {
    int x;
    for (x = ORDER, i = ORDER; i < MAX_KEYNUM; i++) {
      //用m找到在当前节点应该插入的位置,x保存删除位置
      int m = BinALG(pNode, pNode->GetCount(), this->GetKeyValue(i));
      for (mm = 0; (mm < (pNode->GetCount())) &&
                   (this->GetKeyValue(i) > pNode->GetKeyValue(mm));
           mm++)
        ;
      std::cout << "xxx" << mm - m << std::endl;
      //插入
      pNode->SetKeyValue(m, this->GetKeyValue(x));
      pNode->SetChild(m, this->GetChild(x + 1));

      //将插入关键字所在节点的父亲指针设置为当前指针
      this->GetChild(x + 1)->SetFather(pNode);

      //关键字数目加1
      pNode->SetCount(pNode->GetCount() + 1);

      this->m_KeyValues.erase(this->m_KeyValues.begin() + x);
      this->m_Childs.erase(this->m_Childs.begin() + x + 1);
      this->SetCount(this->GetCount() - 1);
    }

    // 设置好Count个数
    this->SetCount(ORDER);
    pNode->SetCount(ORDER);

    // 把原键值返回
    return key;
  }

  // 以下处理key小于第V个键值或key大于第V+1个键值的情况

  // 判断是提取第V还是V+1个键
  int position = 0;
  if (key < this->GetKeyValue(ORDER - 1))  //？？？位置确定
  {
    position = ORDER - 1;
  } else {
    position = ORDER;
  }

  // 把第position个键提出来，作为新的键值返回
  T retKey = this->GetKeyValue(position);

  int x;
  for (x = position + 1, i = position + 1; i < MAX_KEYNUM; i++) {
    int m = 0;
    // m找到在当前节点应该插入的位置 ,x保存删除位置
    for (m = 0; (m < (pNode->GetCount())) &&
                (this->GetKeyValue(x) > pNode->GetKeyValue(m));
         m++)
      ;

    //插入
    pNode->SetKeyValue(m, this->GetKeyValue(x));  //插入键值
    pNode->SetChild(m, this->GetChild(x));  //插入指针(在键值位置下一个)
    this->GetChild(x)->SetFather(
        pNode);  //将插入关键字所在节点的父亲指针设置为当前指针

    //关键字数目加1
    pNode->SetCount(pNode->GetCount() + 1);
    this->SetCount(this->GetCount() - 1);

    this->m_KeyValues.erase(this->m_KeyValues.begin() + x);
    this->m_Childs.erase(this->m_Childs.begin() + x);
  }

  //补上新节点中最后的指针并重新设置孩子的指针
  pNode->SetChild(MAX_KEYNUM - (position + 1),
                  this->GetChild(this->GetCount()));
  this->GetChild(this->GetCount())->SetFather(pNode);
  // 把第position+1 -- 2V+1个指针移到指定的结点中(注意指针比键多一个)

  // 清除提取出的位置
  this->m_Childs.erase(this->m_Childs.begin() + position + 1);
  this->m_KeyValues.erase(this->m_KeyValues.begin() + position);

  // 设置好Count个数
  this->SetCount(position);
  pNode->SetCount(MAX_KEYNUM - (position + 1));
  if (this->GetCount() == pNode->GetCount()) {
    flag = 1;
  }
  return retKey;

  // // 以下处理key小于第V个键值或key大于第V+1个键值的情况

  // // 判断是提取第V还是V+1个键
  // int position = 0;
  // if (key < this->GetKeyValue(ORDER - 1))  //？？？位置确定
  // {
  //   position = ORDER - 1;
  // } else {
  //   position = ORDER;
  // }

  // // 把第position个键提出来，作为新的键值返回
  // T retKey = this->GetKeyValue(position);

  // int x;
  // for (x = position, i = position; i < MAX_KEYNUM; i++) {
  //   int m = 0;
  //   // m找到在当前节点应该插入的位置 ,x保存删除位置
  //   for (m = 0; (m < (pNode->GetCount())) &&
  //               (this->GetKeyValue(i) > pNode->GetKeyValue(m));
  //        m++)
  //     ;

  //   //插入
  //   pNode->SetKeyValue(m, this->GetKeyValue(i));  //插入键值
  //   pNode->SetChild(m, this->GetChild(i));  //插入指针(在键值位置下一个)
  //   this->GetChild(i)->SetFather(
  //       pNode);  //将插入关键字所在节点的父亲指针设置为当前指针

  //   //关键字数目加1
  //   pNode->SetCount(pNode->GetCount() + 1);

  //   this->m_KeyValues.erase(this->m_KeyValues.begin() + x);
  //   // this->m_Childs.erase(this->m_Childs.begin() + x + 1);
  //   this->m_Childs.erase(this->m_Childs.begin() + x);
  //   this->SetCount(this->GetCount() - 1);
  // }

  // //补上新节点中最后的指针并重新设置孩子的指针
  // pNode->SetChild(MAX_KEYNUM - (position + 1),
  //                 this->GetChild(this->GetCount()));
  // this->GetChild(this->GetCount())->SetFather(pNode);
  // // 把第position+1 -- 2V+1个指针移到指定的结点中(注意指针比键多一个)
  // j = 0;

  // // 清除提取出的位置
  // this->m_Childs.erase(this->m_Childs.begin() + this->GetCount());
  // // this->m_KeyValues.erase(this->m_KeyValues.begin() + position);

  // // 设置好Count个数
  // this->SetCount(position);
  // pNode->SetCount(MAX_KEYNUM - (position));
  // return retKey;
}

//移动中间节点键值
template <typename T>
bool InterNode<T>::BorrowInterBro(
    InterNode<T>* pNode) {  // 定义成InterNode而非Node？？？
  // 参数检查
  int i, j;

  // 兄弟结点在本结点左边
  if (pNode->GetKeyValue(0) < this->GetKeyValue(0)) {
    // 赋值
    // 第一个键值不是兄弟结点的最后一个键值，而是本结点第一个子结点的叶子节点中的最小值
    Node<T>* lNode = this->GetChild(0);
    while (lNode->GetNodeType() != 2) {
      lNode = lNode->GetChild(0);
    }
    this->SetKeyValue(0, lNode->GetKeyValue(0));

    // 第一个子结点为兄弟结点的最后一个子结点
    this->SetChild(0, pNode->GetChild(pNode->GetCount()));

    //修改父亲节点
    pNode->GetChild(pNode->GetCount())->SetFather(this);

    //更新本节点父亲节点键值
    LeafNode<T>* newLNode = (LeafNode<T>*)lNode;
    newLNode = newLNode->GetLeftBro();
    T newKey = newLNode->GetKeyValue(0);
    InterNode<T>* father = (InterNode<T>*)this->GetFather();
    while (father != NULL) {
      for (int i = 0; i < father->GetCount(); i++) {
        if (father->GetKeyValue(i) == lNode->GetKeyValue(0)) {
          father->ReplaceKeyValue(i, newKey);
          break;
        }
      }
      father = (InterNode<T>*)father->GetFather();
    }

    // 修改兄弟结点
    pNode->m_Childs.erase(pNode->m_Childs.begin() + pNode->GetCount());
    pNode->m_KeyValues.erase(pNode->m_KeyValues.begin() + pNode->GetCount() -
                             1);
  } else  // 兄弟结点在本结点右边
  {
    // 赋值
    // 最后一个键值不是兄弟结点的第一个键值，而是兄弟结点第一个子结点的叶子节点中的最小值
    Node<T>* lNode = pNode->GetChild(0);
    while (lNode->GetNodeType() != 2) {
      lNode = lNode->GetChild(0);
    }
    this->SetKeyValue(this->GetCount(), lNode->GetKeyValue(0));

    // 最后一个子结点为兄弟结点的第一个子结点
    this->SetChild(this->GetCount() + 1, pNode->GetChild(0));

    //修改父亲节点
    pNode->GetChild(0)->SetFather(this);

    //更新兄弟节点父亲键值
    LeafNode<T>* newLNode = (LeafNode<T>*)lNode;
    newLNode = newLNode->GetRightBro();
    T newKey = newLNode->GetKeyValue(0);
    InterNode<T>* father = (InterNode<T>*)pNode->GetFather();
    while (father != NULL) {
      for (int i = 0; i < father->GetCount(); i++) {
        if (father->GetKeyValue(i) == lNode->GetKeyValue(0)) {
          father->ReplaceKeyValue(i, newKey);
          break;
        }
      }
      father = (InterNode<T>*)father->GetFather();
    }

    // 修改兄弟结点
    pNode->m_Childs.erase(pNode->m_Childs.begin());
    pNode->m_KeyValues.erase(pNode->m_KeyValues.begin());
  }

  // 设置数目
  this->SetCount(this->GetCount() + 1);
  pNode->SetCount(pNode->GetCount() - 1);

  return true;
}

//中间节点删除
template <typename T>
void InterNode<T>::Delete(T key) {
  int i;
  //找到比当前key值大的位置i
  for (i = 0; (i < this->GetCount()) && (key >= this->GetKeyValue(i)); i++)
    ;
  this->m_KeyValues.erase(this->m_KeyValues.begin() + i - 1);
  this->m_Childs.erase(this->m_Childs.begin() + i);
  this->SetCount(this->GetCount() - 1);
  // return true;
}

template <typename T>
void InterNode<T>::Combine(Node<T>* pNode) {
  Node<T>* lNode = pNode->GetChild(0);
  while (lNode->GetNodeType() != 2) {
    lNode = lNode->GetChild(0);
  }
  // 取待合并结点的第一个孩子的第一个元素作为新键值
  T newKey = lNode->GetKeyValue(0);
  this->Insert(newKey, pNode->GetChild(0));

  for (int i = 0; i < pNode->GetCount(); i++) {
    this->Insert(pNode->GetKeyValue(i), pNode->GetChild(i + 1));
  }
}

/*----------------------LeafNode函数定义------------------------*/
//构造函数
template <typename T>
LeafNode<T>::LeafNode() {
  m_RightBro = NULL;
  LeafNode<T>* m_LeftBro = NULL;
  Node<T>::SetNodeType(2);  //设置节点类型
}

//获取兄弟节点
template <typename T>
Node<T>* Node<T>::GetBro(int& leftOrRight, int ORDER) {
  Node<T>* pFather = this->GetFather();  //获取其父结点指针
  Node<T>* pBro = NULL;                  //记录获取到的兄弟指针

  if (pFather == NULL) {
    return NULL;
  }

  for (int i = 0; i <= pFather->GetCount();
       i++)  // GetCount()表示获取数据或关键字数，要比指针数小1。
  {
    // 找到本结点的位置
    if (pFather->GetChild(i) == this) {
      if (i == 0) {                       //本身为第一个指针
        pBro = pFather->GetChild(i + 1);  // 优先找后一个指针
        leftOrRight = 2;
      } else if (i == (pFather->GetCount()))  //表示其为父结点的最右边孩子。
      {
        pBro =
            pFather->GetChild(i - 1);  // 本身是最后一个指针，只能找前一个指针
        leftOrRight = 1;
      } else if ((pFather->GetChild(i + 1)->GetCount()) >
                 ORDER)  //后一个兄弟能借
      {
        pBro = pFather->GetChild(i + 1);  // 优先找后一个指针
        leftOrRight = 2;
      } else {  //后一个兄弟不能借，只能找前一个
        pBro = pFather->GetChild(i - 1);
        leftOrRight = 1;
      }
    }
  }

  return pBro;
}

//叶子节点插入
template <typename T>
bool LeafNode<T>::Insert(T key, uint64_t value) {
  // i为寻找插入位置，j为保存值的数目
  int i, j = Node<T>::GetCount();

  // 找到要插入数据的位置
  for (i = 0; (i < j) && (key > this->GetKeyValue(i)); i++)
    ;

  // 把数据存入当前位置
  m_Values.insert(m_Values.begin() + i, value);
  //插入键值到当前位置
  Node<T>::m_KeyValues.insert(Node<T>::m_KeyValues.begin() + i, key);

  //值数目加1
  Node<T>::SetCount(j + 1);

  // 返回成功
  return true;
}

//叶子分裂
template <typename T>
T LeafNode<T>::Split(LeafNode<T>* pNode) {
  // 把本叶子结点的后一半数据移到指定的结点中(从后往前)
  int j = 0;
  for (int i = this->GetCount() - 1; i >= (this->GetCount() + 1) / 2;
       i--)  // 注意从哪开始分裂？？？
  {
    j++;
    //将原i位置上的元素删除并添加到新节点中
    pNode->Insert(this->GetKeyValue(i), this->GetValues(i));
    this->m_KeyValues
        .pop_back();  // m_Keyvalues.erase(this->m_KeyValues.begin()+i);
    this->m_Values.pop_back();
  }
  // 设置好Count个数
  this->SetCount(this->GetCount() - j);
  // pNode->SetCount(pNode->GetCount() + j);

  // 返回新结点的第一个元素作为键
  return pNode->GetKeyValue(0);
}

//叶子节点删除
template <typename T>
bool LeafNode<T>::Delete(T key) {
  int i;
  bool found = false;
  for (i = 0; i < this->GetCount(); i++) {
    if (key == this->GetKeyValue(i)) {
      found = true;
      break;
    }
  }
  // 如果没有找到，返回失败
  if (false == found) {
    return false;
  }

  // 删除键、值数据
  this->m_KeyValues.erase(this->m_KeyValues.begin() + i);
  this->m_Values.erase(this->m_Values.begin() + i);

  //键值数减1
  this->SetCount(this->GetCount() - 1);

  // 返回成功
  return true;
}

//叶子节点合并
template <typename T>
void LeafNode<T>::Combine(Node<T>* pNode) {
  for (int i = 0; i < pNode->GetCount(); i++) {
    this->Insert(pNode->GetKeyValue(i), pNode->GetValues(i));
  }
}
/*----------------------BPulsTree函数定义------------------------*/
template <typename foo>
BPlusTree<foo>::BPlusTree() {
  m_LeafHead = NULL;
  m_LeafTail = NULL;
  m_Root = NULL;
  MAX_KEYNUM = 0;
  ORDER = 0;
}  //记得拿到外面去
template <typename foo>
BPlusTree<foo>::~BPlusTree() {
  DeleteAll();
}
//树的查找操作
template <typename foo>
bool BPlusTree<foo>::Search(foo data, char* sPath) {
  int i = 0;
  int offset = 0;
  Node<foo>* pNode = GetRoot();

  // 循环查找对应的叶子结点
  while (pNode != NULL) {
    // 结点为叶子结点，循环结束
    if (pNode->GetNodeType() == 2) {
      break;
    }

    // 找到第一个键值大于等于key的位置
    for (i = 1; (data >= pNode->m_KeyValues[i]) && (i <= pNode->GetCount());
         i++)
      ;
    pNode = pNode->GetChild(i);
  }

  // 没找到
  if (pNode == NULL) {
    return false;
  }

  // 在叶子结点中继续找
  bool found = false;
  for (i = 1; i <= pNode->GetCount(); i++) {
    if (data == pNode->GetValues(i)) {
      found = true;
    }
  }
  return found;
}

//插入找叶子节点
template <typename foo>  //
LeafNode<foo>* BPlusTree<foo>::SearchLeafNode(foo key) {
  int i = 0;

  //获取根节点
  Node<foo>* pNode = GetRoot();
  while (pNode != NULL) {
    //找到叶子节点就退出循环
    if (pNode->GetNodeType() == 2) {
      break;
    }

    //寻找大于key的第一个关键字
    for (i = 0; (i < pNode->GetCount()) && (key >= pNode->GetKeyValue(i)); i++)
      ;

    pNode = pNode->GetChild(i);
  }
  return (LeafNode<foo>*)pNode;
}

//删除查找叶子节点
template <typename foo>
LeafNode<foo>* BPlusTree<foo>::SearchDelNode(foo key, int& i) {
  i = 0;
  //获取根节点
  Node<foo>* pNode = GetRoot();
  while (pNode != NULL) {
    //找到叶子节点就退出循环
    if (pNode->GetNodeType() == 2) {
      break;
    }

    //寻找大于value的第一个关键字
    for (i = 0; (key >= pNode->GetKeyValue(i)) && (i < pNode->GetCount()); i++)
      ;  //？？？i=1？？？
    pNode = pNode->GetChild(i);
  }
  if (pNode != NULL) {
    for (i = 0; key != pNode->GetKeyValue(i); i++)
      ;
    return (LeafNode<foo>*)pNode;  //返回当前叶子节点
  }
  i = 0;
  return (LeafNode<foo>*)pNode;  //向下强转？？？
}

//树的插入操作
/* 在B+树中插入数据
插入数据首先要找到理论上要插入的叶子结点，然后分三种情况：
(1) 叶子结点未满。直接在该结点中插入即可；
(2)
叶子结点已满，且无父结点(即根结点是叶子结点)，需要首先把叶子结点分裂，然后选择插入原结点或新结点，然后新生成根节点；
(3)
叶子结点已满，但其父结点未满。需要首先把叶子结点分裂，然后选择插入原结点或新结点，再修改父结点的指针；
(4)
叶子结点已满，且其父结点已满。需要首先把叶子结点分裂，然后选择插入原结点或新结点，接着把父结点分裂，再修改祖父结点的指针。
    因为祖父结点也可能满，所以可能需要一直递归到未满的祖先结点为止。
*/

template <typename foo>
bool BPlusTree<foo>::Insert(foo key, uint64_t value) {
  LeafNode<foo>* oldNode = SearchLeafNode(key);
  //查找合适的叶子节点插入（为空则表示树是空的,新创建一个根节点）
  if (oldNode == NULL) {
    oldNode = new LeafNode<foo>;
    m_LeafHead = oldNode;
    m_LeafTail = oldNode;
    SetRoot(oldNode);
  }

  //插入
  oldNode->Insert(key, value);

  // 1.叶子节点未超
  if (oldNode->GetCount() <= MAX_KEYNUM) {
    return true;
  }

  // 2.叶子节点已满先分裂
  LeafNode<foo>* newNode = new LeafNode<foo>;

  //分裂
  foo leafSplitKey = oldNode->Split(newNode);

  // 在叶子节点双向链中插入结点
  LeafNode<foo>* oldNext = oldNode->GetRightBro();
  oldNode->SetRightBro(newNode);
  newNode->SetRightBro(oldNext);
  newNode->SetLeftBro(oldNode);
  if (oldNext == NULL) {
    m_LeafTail = newNode;
  } else {
    oldNext->SetLeftBro(newNode);
  }

  // 获取父结点
  InterNode<foo>* pFather = (InterNode<foo>*)(oldNode->GetFather());

  // 如果原结点是根节点，对应情况2(pFather是新的根节点)
  if (pFather == NULL) {
    pFather = new InterNode<foo>;  //为什么参考要用Node而不是InterNode？？？
    pFather->SetChild(0, oldNode);          // 指针1指向原结点
    pFather->SetKeyValue(0, leafSplitKey);  // 设置键
    pFather->SetChild(1, newNode);          // 指针2指向新结点
    oldNode->SetFather(pFather);            // 指定父结点
    newNode->SetFather(pFather);            // 指定父结点
    pFather->SetCount(1);

    SetRoot(pFather);  // 指定新的根结点
    return true;
  }
  return InsertInterNode(pFather, leafSplitKey, newNode);
}

template <typename foo>
bool BPlusTree<foo>::InsertInterNode(InterNode<foo>* pNode, foo key,
                                     Node<foo>* rightChild) {
  // 结点未满则直接插入
  if (pNode->GetCount() < MAX_KEYNUM) {
    return pNode->Insert(key, rightChild);  //注意这里是InterNode的插入
  }

  //当前中间节点满了，新创建一个
  InterNode<foo>* pBro = new InterNode<foo>;
  // 分裂当前中间结点(那个节点键少给哪个插)
  int flag = 0;
  foo newKey = pNode->Split(pBro, key, MAX_KEYNUM, ORDER, flag);

  if (pNode->GetCount() < pBro->GetCount()) {
    pNode->Insert(key, rightChild);  //注意这里是InterNode的插入
  } else if (pNode->GetCount() > pBro->GetCount()) {
    pBro->Insert(key, rightChild);  //注意这里是InterNode的插入 ？？？
  } else  // 两者相等，即键值在第V和V+1个键值中间的情况，把字节点挂到新结点的第一个指针上
  {
    if (flag == 1) {
      pNode->Insert(key, rightChild);  //注意这里是InterNode的插入
    } else {
      pBro->SetChild(0, rightChild);
      rightChild->SetFather(pBro);
      if (ORDER * 2 != MAX_KEYNUM) {
        pBro->SetCount(pBro->GetCount() - 1);
      }
    }
  }

  //寻找当前中间节点的父亲节点（循环）
  InterNode<foo>* pFather = (InterNode<foo>*)(pNode->GetFather());
  // 直到根结点都满了，新生成根结点
  if (NULL == pFather) {
    pFather = new InterNode<foo>;
    pFather->SetChild(0, pNode);      // 指向原结点
    pFather->SetKeyValue(0, newKey);  // 设置键
    pFather->SetChild(1, pBro);       // 指向新结点
    pNode->SetFather(pFather);        // 指定父结点
    pBro->SetFather(pFather);         // 指定父结点
    pFather->SetCount(1);

    SetRoot(pFather);  // 指定新的根结点
    return true;
  }

  // 递归
  return InsertInterNode(pFather, newKey, pBro);
}

//树的删除操作
/*
(1) 删除后叶子结点能维持，只修改叶子结点，若是父节点中的键，父结点也要修改；
(2) 删除后叶子结点不能维持，先找到一个最近的兄弟结点(左右)：
    1）如果兄弟借出后能维持，把兄弟的最近一个数据移动过来，父结点的键值也要修改。
    2）兄弟结点临界维持，则把两个结点合并，父结点键也相应合并。(如果合并后父结点不能维持，则递归)
*/
template <typename foo>
bool BPlusTree<foo>::Delete(foo key) {
  // 查找理想的叶子结点
  LeafNode<foo>* oldNode = SearchLeafNode(key);

  // 如果没有找到，返回失败
  if (oldNode == NULL) {
    return false;
  }

  // 保存oldNode最小值信息，更新父节点键值用
  foo saveKey = oldNode->GetKeyValue(0);

  // 删除数据
  if ((oldNode->Delete(key)) == false) {
    return false;
  }

  // 获取父结点
  InterNode<foo>* pFather = (InterNode<foo>*)(oldNode->GetFather());
  if (NULL == pFather) {
    // 此时是根节点且其所有数据均被删除
    if (oldNode->GetCount() == 0) {
      delete oldNode;
      m_LeafHead = NULL;
      m_LeafTail = NULL;
      SetRoot(NULL);
    }

    return true;
  }

  // 情况（1）叶子节点自身能维持
  if (oldNode->GetCount() >= ORDER) {
    //更新父亲节点
    UpdateKey(key, oldNode, 0);
    return true;
  }

  // 找到一个最近的兄弟结点
  int leftOrRight = 1;  // 1代表当前为左兄弟
  LeafNode<foo>* pBro = (LeafNode<foo>*)(oldNode->GetBro(leftOrRight, ORDER));

  // 兄弟借出后能维持 对应情况1）
  if (BorrowLeafBro(oldNode, pBro, saveKey, leftOrRight) == true) {
    return true;
  }

  // 情况2)兄弟结点临界维持
  // 父结点中要删除的键
  foo delKey = key;

  // 把本结点与兄弟结点合并，大的往小的合并，父结点就无需修改指针（因此前面pBro尽量获取的都是右兄弟）
  // 找到的是左兄弟
  if (leftOrRight == 1) {
    pBro->Combine(oldNode);
    delKey = oldNode->GetKeyValue(0);  //获取要删除的key,方便父亲杰迪纳中的删除

    //更新父亲节点
    UpdateKey(key, pBro, 0);

    LeafNode<foo>* oldNext = oldNode->GetRightBro();
    pBro->SetRightBro(
        oldNext);  //原节点的后一个节点设置为要删除节点的下一个节点
    // 在双向链表中删除结点
    if (oldNext == NULL) {
      m_LeafTail = pBro;
    } else {
      oldNext->SetLeftBro(pBro);
    }
    // 删除本结点
    delete oldNode;
  } else {
    oldNode->Combine(pBro);
    delKey = pBro->GetKeyValue(0);  //获取要删除的key，方便父亲杰迪纳中的删除

    UpdateKey(key, oldNode, 0);

    LeafNode<foo>* oldNext = pBro->GetRightBro();
    oldNode->SetRightBro(
        oldNext);  //原节点的后一个节点设置为要删除节点的下一个节点
    // 在双向链表中删除结点
    if (oldNext == NULL) {
      m_LeafTail = oldNode;
    } else {
      oldNext->SetLeftBro(oldNode);
    }
    // 删除本结点
    delete pBro;
  }
  return DelInterNode(pFather, delKey);
}

//递归删除合并中间节点
template <typename foo>
bool BPlusTree<foo>::DelInterNode(InterNode<foo>* pNode, foo key) {
  // 删除键，如果失败一定是没有找到，直接返回失败
  pNode->Delete(key);

  // 获取父结点
  InterNode<foo>* pFather = (InterNode<foo>*)(pNode->GetFather());
  if (pFather == NULL) {
    // 一个数据都没有了，把根结点的第一个结点作为根结点(此时树只剩一个节点)
    if (pNode->GetCount() == 0) {
      SetRoot(pNode->GetChild(0));
      pNode->GetChild(0)->SetFather(NULL);
      delete pNode;
    }
    return true;
  }

  // 删除后结点仍能维持
  if (pNode->GetCount() >= ORDER) {
    for (int i = 0;
         (i < pFather->GetCount()) && (key >= pFather->GetKeyValue(i));
         i++)  //？？？=还是<=
    {
      // 如果删除的是父结点的键值，需要更改该键
      if (pFather->GetKeyValue(i) == key) {
        pFather->ReplaceKeyValue(
            i, pNode->GetKeyValue(0));  // 更改为孩子结点新的第一个元素
      }
    }
    return true;
  }

  //找到一个最近的兄弟结点(根据B+树的定义，除了根结点，总是能找到的)
  int leftOrRight = 1;
  InterNode<foo>* pBro = (InterNode<foo>*)(pNode->GetBro(leftOrRight, ORDER));

  // 兄弟借出后能维持
  if (pBro->GetCount() > ORDER) {
    pNode->BorrowInterBro(pBro);
    return true;
  }

  // 兄弟结点维持不了，合并结点，父结点需要删除键
  foo delKey = key;
  // 把本结点与兄弟结点合并，无论如何合并到数据较小的结点，这样父结点就无需修改指针
  if (leftOrRight == 1) {
    (void)pBro->Combine(pNode);
    delKey = pNode->GetKeyValue(0);
    delete pNode;
  } else {
    (void)pNode->Combine(pBro);
    delKey = pBro->GetKeyValue(0);
    delete pBro;
  }

  // 递归
  return DelInterNode(pFather, delKey);
}

template <typename foo>
bool BPlusTree<foo>::BorrowLeafBro(LeafNode<foo>* oldNode, LeafNode<foo>* pBro,
                                   foo key, int leftOrRight) {
  // 兄弟借出后能维持 对应情况1）
  foo newkey = key;
  uint64_t newValue = 0;

  if (pBro->GetCount() > ORDER) {
    if (leftOrRight == 1)  // 兄弟在左边，移最后一个数据
    {
      newkey = pBro->GetKeyValue(pBro->GetCount() - 1);
      newValue = pBro->GetValues(pBro->GetCount() - 1);
    } else  // 兄弟在右边，移第一个数据过来
    {
      newkey = pBro->GetKeyValue(0);
      newValue = pBro->GetValues(0);
    }

    oldNode->Insert(newkey, newValue);
    pBro->Delete(newkey);

    // 修改父结点的键值
    if (leftOrRight == 1) {
      UpdateKey(key, oldNode, 0);
    } else if (leftOrRight == 2) {
      //原节点更新父亲键值
      UpdateKey(key, oldNode, 0);

      //兄弟仅更新当前父亲键值
      InterNode<foo>* father = (InterNode<foo>*)pBro->GetFather();
      for (int i = 0; i < father->GetCount(); i++) {
        if (father->GetKeyValue(i) == newkey) {
          father->ReplaceKeyValue(i, pBro->GetKeyValue(0));
          break;
        }
      }
    }
    return true;
  }
  return false;
}

//更新父亲键值
template <typename foo>
void BPlusTree<foo>::UpdateKey(foo key, Node<foo>* oldNode, int x) {
  InterNode<foo>* father = (InterNode<foo>*)oldNode->GetFather();
  while (father != NULL) {
    for (int i = 0; i < father->GetCount(); i++) {
      if (father->GetKeyValue(i) == key) {
        father->ReplaceKeyValue(i, oldNode->GetKeyValue(x));
        // father->SetKeyValue(i, father->GetChild(i+1)->GetKeyValue(0));
        break;
      }
    }
    father = (InterNode<foo>*)father->GetFather();
  }
}

//范围查询
template <typename foo>
void BPlusTree<foo>::RangeQuery(foo minKey, foo maxKey) {
  LeafNode<foo>* pNode = m_LeafHead;
  int t = 0;
  // for(int i=0;pNode->GetKeyValue(int i))
  Node<foo>* sNode = GetRoot();
  while (sNode->GetNodeType() != 2) {
    int i = BinALG(sNode, sNode->GetCount(), minKey);
    sNode = sNode->GetChild(i);
  }
  int i = BinALG(sNode, sNode->GetCount(), minKey);
  int j = i;
  if ((sNode->GetKeyValue(i) != minKey) && (maxKey < sNode->GetKeyValue(i))) {
    std::cout << "范围内无数据" << std::endl;
  } else {
    while (sNode != NULL) {
      for (; j < sNode->GetCount(); j++) {
        if (sNode->GetKeyValue(j) >= minKey) {
          if (sNode->GetKeyValue(j) <= maxKey) {
            std::cout << sNode->GetKeyValue(j) << " ";
            t++;
          } else {
            break;
          }
        }
      }
      j = 0;
      sNode = sNode->GetRightBro();
    }
  }
  std::cout << std::endl;
  std::cout << "找到了" << t << "个数据" << std::endl;
}

//删除整颗树
template <typename foo>
void BPlusTree<foo>::DeleteAll() {
  int i = 0;
  // M，N用于记录当前行和下一行的节点数，实现换行
  int m = 0, n = 0;
  Node<foo>* pNode = this->GetRoot();
  if (pNode == NULL) {
    return;
  }
  Node<foo>* node;
  std::queue<Node<foo>*> q;
  q.push(pNode);
  m++;
  while (!q.empty()) {
    node = q.front();
    m--;
    q.pop();
    //非叶子节点才将指针入队
    if (node->GetNodeType() != 2) {
      for (int i = 0; i <= node->GetCount(); i++) {
        q.push(node->GetChild(i));
        n++;
      }
    }
    delete node;
    i++;
    if (m == 0) {
      m = n;
      n = 0;
    }
  }
  this->SetRoot(NULL);
}

//层序遍历树
template <typename foo>
void BPlusTree<foo>::LevelTraversal(Node<foo>* pNode) {
  std::cout << std::endl;
  // M，N用于记录当前行和下一行的节点数，实现换行
  if (pNode == NULL) {
    return;
  }
  int m = 0, n = 0;
  Node<foo>* node;
  std::queue<Node<foo>*> q;
  q.push(pNode);
  m++;
  while (!q.empty()) {
    node = q.front();
    m--;
    q.pop();
    //非叶子节点才将指针入队
    if (node->GetNodeType() != 2) {
      for (int i = 0; i <= node->GetCount(); i++) {
        q.push(node->GetChild(i));
        n++;
      }
    }

    std::cout << "[";
    for (int i = 0; i < node->GetCount(); i++) {
      std::cout << "" << node->GetKeyValue(i) << " ";
      std::cout << " ";
    }
    std::cout << "]";

    if (m == 0) {
      std::cout << std::endl;
      m = n;
      n = 0;
    }
  }
}

//二分查找
template <typename T>
int BinALG(Node<T>* pNode, int size, T target) {
  int left = 0;
  int right = size - 1;
  while (left <= right) {
    int middle = left + (right - left) / 2;
    if (target < pNode->GetKeyValue(middle)) {
      right = middle - 1;
    } else if (target > pNode->GetKeyValue(middle)) {
      left = middle + 1;
    } else {
      return middle;
    }
  }
  return left;
}

#endif  // HELLO_H