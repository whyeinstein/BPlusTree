

# 协程

子程序，或者称为函数，在所有语言中都是层级调用，比如A调用B，B在执行过程中又调用了C，C执行完毕返回，B执行完毕返回，最后是A执行完毕。子程序调用是通过栈实现的，一个线程就是执行一个子程序。子程序调用总是一个入口，一次返回，调用顺序是明确的。而协程的调用和子程序不同。

协程看上去也是子程序，但执行过程中，在子程序内部可中断，然后转而执行别的子程序，在适当的时候再返回来接着执行。一个子程序中中断，去执行其他子程序，不是函数调用，有点类似CPU的中断。

```
def A():
    print '1'
    print '2'
    print '3'
 
def B():
    print 'x'
    print 'y'
    print 'z'
```

假设由协程执行，在执行A的过程中，可以随时中断，去执行B，B也可能在执行过程中中断再去执行A，结果可能是：

```
1
2
x
y
3
z
```

看起来A、B的执行有点像多线程，但**协程的特点在于是一个线程执行**

协程最大的优势就是极高的执行效率。因为子程序切换不是线程切换，而是由程序自身控制，因此，没有线程切换的开销，和多线程比，线程数量越多，协程的性能优势就越明显。

第二大优势就是不需要多线程的锁机制，因为只有一个线程，也不存在同时写变量冲突，在协程中控制共享资源不加锁，只需要判断状态就好了，所以执行效率比多线程高很多。

因为协程利用多核CPU呢最简单的方法是多进程+协程，既充分利用多核，又充分发挥协程的高效率，可获得极高的性能。



现有的 C++ 协程库均基于两种方案：**利用汇编代码控制协程上下文的切换，以及利用操作系统提供的 API 来实现协程上下文切换**。

- libco，Boost.context：基于汇编代码的上下文切换
- phxrpc：基于 ucontext/Boost.context 的上下文切换
- libmill：基于 setjump/longjump 的协程切换

一般基于汇编的上下文切换要比采用系统调用的切换更加高效

**libco 协程：**

一个 coroutine 的数据结构

```c++
//co_routine_inner.h 
struct stCoRoutine_t
{
    stCoRoutineEnv_t *env;  // 协程运行环境
    pfn_co_routine_t pfn;   // 协程执行的逻辑函数
    void *arg;              // 函数参数
    coctx_t ctx;            // 保存协程的下文环境 
    ...
    char cEnableSysHook;    // 是否运行系统 hook，即非侵入式逻辑
    char cIsShareStack;     // 是否在共享栈模式
    void *pvEnv;
    stStackMem_t* stack_mem;  // 协程运行时的栈空间
    char* stack_sp;           // 用来保存协程运行时的栈空间
    unsigned int save_size;
    char* save_buffer;
};
```

由于多个协程运行于**一个线程**内部的，因此当创建线程中的第一个协程时，需要初始化该协程所在的环境 `stCoRoutineEnv_t`

```c++
struct stCoRoutineEnv_t
{
    stCoRoutine_t *pCallStack[ 128 ];  // 记录当前创建的协程
    int iCallStackSize;                // 记录当前一共创建了多少个协程
    stCoEpoll_t *pEpoll;               // 该线程的协程调度器
 
    // 在使用共享栈模式拷贝栈内存时记录相应的 coroutine
    stCoRoutine_t* pending_co;
    stCoRoutine_t* occupy_co;
};
```

初始化 `stCoRoutineEnv_t` 时主要完成以下几步：

1. 为 `stCoRoutineEnv_t` 申请空间并且进行初始化，设置协程调度器 `pEpoll`。
2. 创建一个空的 coroutine，初始化其上下文环境( 有关 `coctx` 在后文详细介绍 )，将其加入到该线程的协程环境中进行管理，并且设置其为 main coroutine。这个 main coroutine 用来运行该线程主逻辑。

**初始化 stCoRoutineEnv_t 	：**

```c++
struct stStackMem_t
{<!-- -->
stCoRoutine_t* occupy_co; // 使用该栈的协程
int stack_size; // 栈大小
char* stack_bp; // 栈的指针，栈从高地址向低地址增长
char* stack_buffer; // 栈底
};
```

**使用 co_create 创建完一个协程之后，将调用 co_resume 来将该协程激活运行：**

```c++
void co_resume( stCoRoutine_t *co )
{<!-- -->
stCoRoutineEnv_t *env = co->env;
// 获取当前正在运行的协程的结构
stCoRoutine_t *lpCurrRoutine = env->pCallStack[ env->iCallStackSize - 1 ];
if( !co->cStart )
{<!-- -->
// 为将要运行的 co 布置上下文环境
coctx_make( &co->ctx,(coctx_pfn_t)CoRoutineFunc,co,0 );
co->cStart = 1;
}
env->pCallStack[ env->iCallStackSize++ ] = co; // 设置co为运行的线程
co_swap( lpCurrRoutine, co );
}
```

