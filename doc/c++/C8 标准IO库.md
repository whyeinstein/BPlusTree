# C8 标准I\O库

##### IO标准库类型和头文件，IO继承层次

![36](https://raw.githubusercontent.com/whyeinstein/ndssl_training_notes/main/img/202307041104274.png)

![37](https://raw.githubusercontent.com/whyeinstein/ndssl_training_notes/main/img/202307041106963.png)

io库使用继承有一个主要的用处在于，如果函数有基类类型的引用形参时，可以给函数传递其派生类型的对象。如对istream&进行操作的对象，可以使用ifstream或者istringstream对象来调用。

##### IO对象不可复制

io对象的不可复制，产生了几个问题

- 由于流对象不能复制，因此不能存储在 vector（或其他）容器中（即不存在存储流对象的 vector 或其他容器）。
- 形参或返回类型也不能为流类型。如果需要传递或返回 IO对象，则必须传递或返回指向该对象的指针或引用。

##### 流的条件

![38](https://raw.githubusercontent.com/whyeinstein/ndssl_training_notes/main/img/202307041149359.png)

```c++
auto old_state = cin.rdstate(); // 记住cin的当前状态
cin.clear();					// 使cin有效
process_input(cin);				// 使用cin
cin.setstate(old_state);		// 将cin置为原有状态
```

##### 输出缓冲区的管理 

系统将字符串字面值存储在与流 os 关联的缓冲区中。下面几种情况将导致 缓冲区的内容被刷新，即写入到真实的输出设备或者文件：

1. 程序正常结束。作为 main 返回工作的一部分，将清空所有输出缓冲区。 
2. 在一些不确定的时候，缓冲区可能已经满了，在这种情况下，缓冲区将会 在写下一个值之前刷新。
3.  用操纵符（第 1.2.2 节）显式地刷新缓冲区，例如行结束符 endl。 
4. 在每次输出操作执行完后，用 unitbuf 操作符设置流的内部状态，从而 清空缓冲区。 
5. 可将输出流与输入流关联（tie）起来。在这种情况下，在读输入流时将刷新其关联的输出缓冲区。 

如果需要刷新所有输出，最好使用 unitbuf 操纵符。

##### 文件输入输出

![39](https://raw.githubusercontent.com/whyeinstein/ndssl_training_notes/main/img/202307041153250.png)

```c++
std::ifstream in(filename); // 创建一个ifstream对象，并自动调用.open打开filename
std::ofstream out;          // 创建一个ofstream对象
out.open(filename);
if (out){
    //如果out成功打开，则做一些事情
}
in.close();               // 此时if (in)条件位false
```

##### 字符串流

标准库定义了三种类型的字符串流：

-  istringstream，由 istream 派生而来，提供读 string 的功能。

-  ostringstream，由 ostream 派生而来，提供写 string 的功能。

-  stringstream，由 iostream 派生而来，提供读写 string 的功能。

![40](https://raw.githubusercontent.com/whyeinstein/ndssl_training_notes/main/img/202307041156240.png)
