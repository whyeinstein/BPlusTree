# B+树并发控制

## B+树并发控制机制的基本要求

B+树并发控制机制需要满足以下几点要求：

- `正确的读操作`：

- - R.1 不会读到一个处于中间状态的键值对：读操作访问中的键值对正在被另一个写操作修改
  - R.2 不会找不到一个存在的键值对：读操作正在访问某个树节点，这个树节点上的键值对同时被另一个写操作（分裂/合并操作）移动到另一个树节点，导致读操作没有找到目标键值对



- `正确的写操作`：

- - W.1 两个写操作不会同时修改同一个键值对



- `无死锁`：

- - D.1 不会出现死锁：两个或多个线程发生永久堵塞（等待），每个线程都在等待被其他线程占用并堵塞了的资源



不管B+树使用的是基于锁的并发机制还是Lock-Free的并发机制，都必须满足上述需求。

- `SL (Shared Lock)`: 共享锁 — 加锁
- `SU (Shared Unlock)`: 共享锁 — 解锁
- `XL (Exclusive Lock)`: 互斥锁 — 加锁
- `XU (Exclusive Unlock)`: 互斥锁 — 解锁
- `SXL (Shared Exclusive Lock)`: 共享互斥锁 — 加锁
- `SXU (Shared Exclusive Unlock)`: 共享互斥锁 — 解锁

- `R.1/R.2/W.1/D.1`: 并发机制需要满足的正确性要求

- `safe nodes`：判断依据为该节点上的当前操作是否会影响祖先节点。以传统B+树为例：

  1.  对于插入操作，当键值对的数量小于M时，插入操作不会触发分裂操作，该节点属于safe node；反之当键值对数量等于M时，该节点属于unsafe node；
  2. 对于删除操作，当键值对的数量大于M/2时，不会触发合并操作，该节点属于safe node；反之当键值对数量等于M/2时，该节点属于unsafe node。当然，对于MySQL而言，一个节点是否是安全节点取决于键值对的大小和页面剩余空间大小等多个因素，详细代码可查询MySQL5.7的`btr_cur_will_modify_tree()`函数

  （https://zhuanlan.zhihu.com/p/414141859）

```c++
/* Algorithm1. 读操作 */ 
//共享锁 
1.   SL(index) 
2.   Travel down to the leaf node
3.   SL(leaf)
4.   SU(index)
5.   Read the leaf node
6.   SU(leaf)
```

```c++
/* Algorithm2. 悲观写操作 */
1.   XL(index) //互斥锁
2.   Travel down to the leaf node
3.   XL(leaf)   /* lock prev/curr/next leaves */
4.   Modify the tree structure 
5.   XU(index)  
6.   Modify the leaf node 
7.   XU(leaf)
```

```c++
/* Algorithm3. 乐观写操作 */
1.   SL(index) //共享锁
2.   Travel down to the leaf node
3.   XL(leaf)
4.   SU(index)
5.   Modify the leaf node
6.   XU(leaf)
```

## **只锁住B+树中被修改的分支**

```c++
/* Algorithm4. 读操作 */
1.   current <= root//从根读取到叶子，不断加锁解锁
2.   SL(current) 
3.   While current is not leaf do {
4.     SL(current->son)
5.     SU(current)
6.     current <= current->son
7.   }
8.   Read the leaf node 
9.   SU(current)
```

```c++
/* Algorithm5. 写操作 */
1.   current <= root
2.   XL(current)
3.   While current is not leaf do {
4.      XL(current->son)
5.      current <= current->son
6.      If current is safe do {
7.         /* Unlocked ancestors on stack. */
8.         XU(locked ancestors)
9.      }     
10.  }
11.  /* Already lock the modified branch. */
12.  Modify the leaf and upper nodes 
13.  XU(current) and XU(locked ancestors) 
```

## **SX锁**(Shared Exclusive Lock/Unlock)共享互斥锁

```c++
/* Algorithm6. 写操作 */
1.   current <= root
2.   SXL(current)
3.   While current is not leaf do {
4.      SXL(current->son)
5.      current <= current->son
6.      If current is safe do {
7.          /* Unocked ancestors on stack. */
8.          SXU(locked ancestors)
9.      }     
10. }
11. XL(modified nodes) /* SX->X, top-to-down*/
12. /* Already lock the modified branch. */
13. Modify the leaf and upper nodes 
13. XU(current) and XU(locked ancestors) 
```

算法5与6类似，但算法6使用了共享互斥锁，在到达叶子节点后再将SX锁升级为X锁。

**Blink树**

- 假设树节点的读写操作是原子性的

- 写操作之间修改同一份数据时会出现冲突

```c++
/* Algorithm7. 读操作 */
1.   current <= root
2.   While current is not leaf do {
3.      current <= scanNode(current, v)
4.      current <= current->son
5.   }
6.   /* Keep move right if necessary. */
7.   /* Deal with the leaf node. */
/*  scanNode函数 */
8.  Func scanNode(node* t, int v) {
9.     If t->next->key[0] <= v do 
10.       t <= scanNode(t->next, v)
11.   return t;
12. }
```

```c++
/* Algorithm8. 写操作 */
1.   current <= root                                  
2.   While current is not leaf do {             
3.      current <= scannode(current, v)     
4.      stack <= current                           
5.      current <= current->son                 
6.   }                                                          
7.   XL(current)   /* lock the current leaf */ 
8.   moveRight(current)                             
9.   DoInsertion:                                        
10.  If current is safe do                                       
11.    insert(current) and XU(current)              
12.  else {
13.    allocate(next)
14.    shift(next) + link(next)
15.    modify(current)
16.    oldnode <= current
17.    current <= pop(stack)
18.    XL(current)
19.    moveRight(current) 
20.    XU(oldnode)
21.    goto DoInsertion; 
22. }
```

对于删除操作，年代久远的Blink树采用一种比较粗暴的方式。它采用index粒度的X锁，堵塞其它读/写操作

`moveRight`与`scanNode`相似，主要的区别在于前者是在加锁状态下向右走，拿到右节点的锁后可释放当前结点的锁。

自左向右，自底向上

## OLFIT树

```c++
//版本号的具体操作
/* Algorithm9. 树节点的写操作 */
1.   XL(current)
2.   Update the node content
3.   INCREASE(version)
4.   XU(current)
/* Algorithm10. 树节点的读操作 */
1.   RECORD(version)
2.   Read the node content
3.   If node is lock, go to step1
4.   If version differs, go to step1
```

## Mysql5.7的B+树并发控制机制

```c++
/* Algorithm12. 读操作 */
1.   SL(index)
2.   While current is not leaf do {
3.      SL(non-leaf)
4.   }
5.   SL(leaf)
6.   SU(non-leaf)
7.   SU(index)
8.   Read the leaf node
9.   SU(leaf)
```

```c++
/* Algorithm13. 乐观写操作 */
1.   SL(index)
2.   While current is not leaf do {
3.      SL(non-leaf)
4.   }
5.   XL(leaf)
6.   SU(non-leaf)
7.   SU(index)
8.   Modify the leaf node
9.   XU(leaf)
```

```c++
/* Algorithm14. 悲观写操作 */
1.   SX(index) 
2.   While current is not leaf do {
3.      XL(modified non-leaf)
4.   }
5.   XL(leaf)      /* lock prev/curr/next leaf */
6.   Modify the tree structure 
7.   XU(non-leaf)
8.   SXU(index) 
9.   Modify the leaf node 
10.  XU(leaf)
```

