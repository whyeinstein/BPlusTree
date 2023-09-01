# 多线程编程

## 1概述

**多线程**：多线程是实现并发（并行）的手段，并发（并行）即多个线程同时执行，一般而言，多线程就是把执行一件事情的完整步骤拆分为多个子步骤，然后使得这多个步骤同时执行。

**C++多线程**：（简单情况下）C++多线程使用多个函数实现各自功能，然后将不同函数生成不同线程，并同时执行这些线程（不同线程可能存在一定程度的执行先后顺序，但总体上可以看做同时执行）。

## 2 基础知识

### 2.1 创建线程

```c++
#include<iostream>
 //该头文件中定义了thread类，创建一个线程即实例化一个该类的对象，实例化对象时候调用的构造函数需要传递一个参数，该参数就是函数名，thread th1(proc1)；
#include<thread>
using namespace std;
void proc(int a)
{
    cout << "我是子线程,传入参数为" << a << endl;
    cout << "子线程中显示子线程id为" << this_thread::get_id()<< endl;
}
int main()
{
    cout << "我是主线程" << endl;
    int a = 9;
    thread th2(proc,a);//第一个参数为函数名，第二个参数为该函数的第一个参数，如果该函数接收多个参数就依次写在后面。此时线程开始执行。
    cout << "主线程中显示子线程id为" << th2.get_id() << endl;
    th2.join()；//此时主线程被阻塞直至子线程执行结束。
    return 0;
}

```

**只要创建了线程对象**（传递“函数名/可调用对象”作为参数的情况下），**线程就开始执行**（std::thread 有一个无参构造函数重载的版本，不会创建底层的线程）。

**join()**, 当前线程暂停, 等待指定的线程执行结束后, 当前线程再继续。th1.join()，即该语句所在的线程（该语句写在main（）函数里面，即主线程内部）暂停，等待指定线程（指定线程为th1）执行结束后，主线程再继续执行。

有两种线程阻塞方法**join()**与detach()，阻塞线程的目的是调节各线程的先后执行顺序，不推荐使用detach()，detach()使用不当会发生引用对象失效的错误。当线程启动后，一定要在和线程相关联的thread对象销毁前，对线程运用join()或者detach()。

### 2.2 互斥量

```c++
#include<iostream>
#include<thread>
#include<mutex>
using namespace std;
mutex m;//实例化m对象，不要理解为定义变量
void proc1(int a)
{
    m.lock();
    cout << "proc1函数正在改写a" << endl;
    cout << "原始a为" << a << endl;
    cout << "现在a为" << a + 2 << endl;
    m.unlock();
}

void proc2(int a)
{
    m.lock();
    cout << "proc2函数正在改写a" << endl;
    cout << "原始a为" << a << endl;
    cout << "现在a为" << a + 1 << endl;
    m.unlock();
}
int main()
{
    int a = 0;
    thread proc1(proc1, a);
    thread proc2(proc2, a);
    proc1.join();
    proc2.join();
    return 0;
}
```

不推荐实直接去调用成员函数**lock()**，因为如果忘记**unlock()**，将导致锁无法释放，使用**lock_guard**或者**unique_lock**能避免忘记解锁这种问题。

**lock_guard()**：声明一个局部的lock_guard对象，在其构造函数中进行加锁，在其析构函数中进行解锁。最终的结果就是：创建即加锁，作用域结束自动解锁。从而使用lock_guard()就可以替代lock()与unlock()。

```c++
#include<iostream>
#include<thread>
#include<mutex>
using namespace std;
mutex m;//实例化m对象，不要理解为定义变量
void proc1(int a)
{
    lock_guard<mutex> g1(m);//用此语句替换了m.lock()；lock_guard传入一个参数时，该参数为互斥量，此时调用了lock_guard的构造函数，申请锁定m
    cout << "proc1函数正在改写a" << endl;
    cout << "原始a为" << a << endl;
    cout << "现在a为" << a + 2 << endl;
}//此时不需要写m.unlock(),g1出了作用域被释放，自动调用析构函数，于是m被解锁

void proc2(int a)
{
    {
        lock_guard<mutex> g2(m);
        cout << "proc2函数正在改写a" << endl;
        cout << "原始a为" << a << endl;
        cout << "现在a为" << a + 1 << endl;
    }//通过使用{}来调整作用域范围，可使得m在合适的地方被解锁
    cout << "作用域外的内容3" << endl;
    cout << "作用域外的内容4" << endl;
    cout << "作用域外的内容5" << endl;
}
int main()
{
    int a = 0;
    thread proc1(proc1, a);
    thread proc2(proc2, a);
    proc1.join();
    proc2.join();
    return 0;
}
```

lock_gurad也可以传入两个参数，第一个参数为adopt_lock标识时，表示不再构造函数中不再进行互斥量锁定，因此**此时需要提前手动锁定**。

**unique_lock**: 使用lock_guard后不能手动lock()与手动unlock();使用unique_lock后可以手动lock()与手动unlock();
unique_lock的第二个参数，除了可以是adopt_lock,还可以是try_to_lock与defer_lock;
try_to_lock: 尝试去锁定，得保证锁处于unlock的状态,然后尝试现在能不能获得锁；尝试用mutx的lock()去锁定这个mutex，但如果没有锁定成功，会立即返回，不会阻塞在那里
defer_lock: 初始化了一个没有加锁的mutex;

```c++
#include<iostream>
#include<thread>
#include<mutex>
using namespace std;
mutex m;
void proc1(int a)
{
    unique_lock<mutex> g1(m, defer_lock);//始化了一个没有加锁的mutex
    cout << "不拉不拉不拉" << endl;
    g1.lock();//手动加锁，注意，不是m.lock();注意，不是m.lock();注意，不是m.lock()
    cout << "proc1函数正在改写a" << endl;
    cout << "原始a为" << a << endl;
    cout << "现在a为" << a + 2 << endl;
    g1.unlock();//临时解锁
    cout << "不拉不拉不拉"  << endl;
    g1.lock();
    cout << "不拉不拉不拉" << endl;
}//自动解锁

void proc2(int a)
{
    unique_lock<mutex> g2(m,try_to_lock);//尝试加锁，但如果没有锁定成功，会立即返回，不会阻塞在那里；
    cout << "proc2函数正在改写a" << endl;
    cout << "原始a为" << a << endl;
    cout << "现在a为" << a + 1 << endl;
}//自动解锁
int main()
{
    int a = 0;
    thread proc1(proc1, a);
    thread proc2(proc2, a);
    proc1.join();
    proc2.join();
    return 0;
}
```

```c++
//unique_lock所有权的转移
mutex m;
{  
    unique_lock<mutex> g2(m,defer_lock);
    unique_lock<mutex> g3(move(g2));//所有权转移，此时由g3来管理互斥量m
    g3.lock();
    g3.unlock();
    g3.lock();
}
```

**#include<condition_variable>**:

notify_all():随机唤醒一个等待的线程
notify_once():唤醒所有等待的线程

## 2.3异步线程

**#include<future>**      **async与future：**

async是一个函数模板，用来启动一个异步任务，它返回一个future类模板对象，future对象起到了**占位**的作用，刚实例化的future是没有储存值的，但在调用future对象的get()成员函数时，主线程会被阻塞直到异步线程执行结束，并把返回结果传递给future，即通过FutureObject.get()获取函数返回值。

相当于你去办政府办业务（主线程），把资料交给了前台，前台安排了人员去给你办理（async创建子线程），前台给了你一个单据（future对象），说你的业务正在给你办（子线程正在运行），等段时间你再过来凭这个单据取结果。过了段时间，你去前台取结果，但是结果还没出来（子线程还没return），你就在前台等着（阻塞），直到你拿到结果（get()）你才离开（不再阻塞）。

```c++
#include <iostream>
#include <thread>
#include <mutex>
#include<future>
#include<Windows.h>
using namespace std;
double t1(const double a, const double b)
{
	double c = a + b;
	Sleep(3000);//假设t1函数是个复杂的计算过程，需要消耗3秒
	return c;
}

int main() 
{
	double a = 2.3;
	double b = 6.7;
	future<double> fu = async(t1, a, b);//创建异步线程线程，并将线程的执行结果用fu占位；
	cout << "正在进行计算" << endl;
	cout << "计算结果马上就准备好，请您耐心等待" << endl;
	cout << "计算结果：" << fu.get() << endl;//阻塞主线程，直至异步线程return
        //cout << "计算结果：" << fu.get() << endl;//取消该语句注释后运行会报错，因为future对象的get()方法只能调用一次。
	return 0;
}
```

**与shared_future比较**

future与shard_future的用途都是为了**占位**，但是两者有些许差别。
future的get()成员函数是转移数据所有权;shared_future的get()成员函数是复制数据。
因此：
**future对象的get()只能调用一次**；无法实现多个线程等待同一个异步线程，一旦其中一个线程获取了异步线程的返回值，其他线程就无法再次获取。
**shared_future对象的get()可以调用多次**；可以实现多个线程等待同一个异步线程，每个线程都可以获取异步线程的返回值。

## 3 实例

生产者-消费者问题

```c++
/*
生产者消费者问题
*/
#include <iostream>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include<Windows.h>
using namespace std;

deque<int> q;
mutex mu;
condition_variable cond;
int c = 0;//缓冲区的产品个数

void producer() { 
	int data1;
	while (1) {//通过外层循环，能保证生成用不停止
		if(c < 3) {//限流
			{
				data1 = rand();
				unique_lock<mutex> locker(mu);//锁
				q.push_front(data1);
				cout << "存了" << data1 << endl;
				cond.notify_one();  // 通知取
				++c;
			}
			Sleep(500);
		}
	}
}

void consumer() {
	int data2;//data用来覆盖存放取的数据
	while (1) {
		{
			unique_lock<mutex> locker(mu);
			while(q.empty())
				cond.wait(locker); //wati()阻塞前先会解锁,解锁后生产者才能获得锁来放产品到缓冲区；生产者notify后，将不再阻塞，且自动又获得了锁。
			data2 = q.back();//取的第一步
			q.pop_back();//取的第二步
			cout << "取了" << data2<<endl;
			--c;
		}
		Sleep(1500);
	}
}
int main() {
	thread t1(producer);
	thread t2(consumer);
	t1.join();
	t2.join();
	return 0;
}
```

## 4 线程池

线程池通常适合下面的几个场合：
(1)单位时间内处理任务频繁而且任务处理时间短
(2)对实时性要求较高。如果接受到任务后在创建线程，可能满足不了实时要求，因此必须采用线程池进行预创建。

日志存储就可以使用线程池

![80](https://raw.githubusercontent.com/whyeinstein/ndssl_training_notes/main/img/202308031014860.png)

**工作队列**

```c++
struct NWORKER{
        pthread_t threadid;        //线程id
        bool terminate;            //是否需要结束该worker的标志
        int isWorking;            //该worker是否在工作
        ThreadPool *pool;        //隶属于的线程池
}
```

**任务队列**

```c++
struct NJOB{
        void (*func)(void *arg);     //任务函数
        void *user_data;             //函数参数
    };
```

**线程池**

```c++
class ThreadPool{
private:
    struct NWORKER{
        pthread_t threadid;
        bool terminate;
        int isWorking;
        ThreadPool *pool;
    } *m_workers;
 
    struct NJOB{
        void (*func)(void *arg);     //任务函数
        void *user_data;
    };
public:
    //线程池初始化
    //numWorkers:线程数量
    ThreadPool(int numWorkers, int max_jobs);
    //销毁线程池
    ~ThreadPool();
    //面向用户的添加任务
    int pushJob(void (*func)(void *data), void *arg, int len);
 
private:
    //向线程池中添加任务
    bool _addJob(NJOB* job);
    //回调函数
    static void* _run(void *arg);
    void _threadLoop(void *arg);
 
private:
    std::list<NJOB*> m_jobs_list;
    int m_max_jobs;                            //任务队列中的最大任务数
    int m_sum_thread;                        //worker总数
    int m_free_thread;                        //空闲worker数
    pthread_cond_t m_jobs_cond;           //线程条件等待
    pthread_mutex_t m_jobs_mutex;         //为任务加锁防止一个任务被两个线程执行等其他情况
};
```

**回调函数**

```c++
void ThreadPool::_threadLoop(void *arg) {
    NWORKER *worker = (NWORKER*)arg;
    while (1){
        //线程只有两个状态：执行\等待
        //查看任务队列前先获取锁
        pthread_mutex_lock(&m_jobs_mutex);
        //当前没有任务
        while (m_jobs_list.size() == 0) {
            //检查worker是否需要结束生命
            if (worker->terminate) break;
            //条件等待直到被唤醒
            pthread_cond_wait(&m_jobs_cond,&m_jobs_mutex);
        }
        //检查worker是否需要结束生命
        if (worker->terminate){
            pthread_mutex_unlock(&m_jobs_mutex);
            break;
        }
        //获取到job后将该job从任务队列移出，免得其他worker过来重复做这个任务
        struct NJOB *job = m_jobs_list.front();
        m_jobs_list.pop_front();
        //对任务队列的操作结束，释放锁
        pthread_mutex_unlock(&m_jobs_mutex);
 
        m_free_thread--;
        worker->isWorking = true;
        //执行job中的func
        job->func(job->user_data);
        worker->isWorking = false;
 
        free(job->user_data);
        free(job);
    }
 
    free(worker);
    pthread_exit(NULL);
}
```

