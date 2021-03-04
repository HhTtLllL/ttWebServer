#### 编译 && 运行
~~~
编译: cmake CMakeLists.txt && make 
运行: ./tt_server [thread_nums]	port[8000]

~~~

项目介绍: 使用c++语言编写，采用多reactor线程模型,　实现了基本的GET, POST请求。　


项目技术: 

 - 使用GCC内置原子操作__sync__　和 volatile 实现原子类 
 - 采用 RAII 机制封装MutexLockGuard类,条件变量等线程同步互斥机制。
 - 使用eventfd实现线程高效间唤醒
 - 使用智能指针管理内存分配，防止内存泄露 
 - 定时器使用timerfd_*实现　
 - 给每一个TcpConnection准备两个buffer 
 
开发环境: 4.15.0-30deepin-generic
开发工具: vim 编辑器, GCC编译器
