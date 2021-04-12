
---
**琐碎点**
 - `std::forward<Type>(arg)` : 返回Tpye&& 类型引用，右值引用用于表达式时为左值，此时需要forward转发保持类型  

 - `static` : 成员函数只能访问static成员变量与其它static成员函数(没有this指针)，但是可以访问构造函数

 -  类内包含不支持拷贝操作的成员时候，类的拷贝构造函数，拷贝赋值运算符默认标记为`delete`

 -  `noexcept`：默认不使用,大部分情况下,你都很难避免bad_alloc的异常,即使这个函数不直接allocate,有可能编译器执行代码时还是需要allocate。比如最简单的a = b，如果a和b是一个自定义的type，有可能这个type有类似vector、string这些需要allocate的member,那这个赋值语句就可能报错。而且即使这个type现在没有这样的member,以后说不定代码改来改去就加了一个这样的成员,不使用noexcept是最future-proof的。move constructor/assignment operator 如果不会抛出异常,一定用noexcept。如果destructor抛出异常，程序99%会挂掉,用noexcept基本没任何坏处。简单的leaf function,像是int，pointer这类的getter，setter用noexcept,因为不可能出错

<br>

---

 - 并行：同一时刻真的有多个任务在运行    
 - 并发：同一时刻只有一个任务在运行，快速切换感觉像在同时运行  

**why并发?**      
 - 分离关注点：相关代码与无关代码分离  
 - 性能

**避免死锁**  
 - 避免嵌套锁 
 - 使用固定顺序获取多个锁
 - 定义遍历容器的顺序（比如链表删除操作）
 - 使用层次锁来避免 

**atomic memory order param**
 - Store操作： `memory_order_relaxed, memory_order_release, memory_order_seq_cst`
 - Load操作：`memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_seq_cst`
 - Read-modify-write操作：`memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_release, memory_order_acq_rel, memory_order_seq_cst`

**内存序列选项**
 - 顺序一致序列：`memory_order_seq_cst`
 - 获取-释放序列：`memory_order_consume, memory_order_acquire, memory_order_acq_rel, memory_order_acq_rel`
 - 自由序列：`memory_order_relaxed`

**std::atomic\<UDT\>**
 - 必须有拷贝赋值运算符（编译器创建的）
 - 不能有虚函数或者虚基类
 - 所有成员必须支持拷贝赋值运算符
 - 位可比较的(保证CAS操作)

 <br>

![Alt pic](./pictures/atomic.png)

**自旋锁**  
***一个线程在获取锁的时候，如果锁已经被其它线程获取，那么这个线程将循环等待，知道锁被获取才退出***  
 - 优点 : 不会使线程状态发生切换进入阻塞，一直处于用户态，减少了不必要的上下文切换  
 - 缺点 : 当一个线程持有锁时间过长，其它想要获取锁的线程过多时，会导致cpu占用率极高


<br>

![Alt pic](./pictures/进程.png)
![Alt pic](./pictures/线程.png)
![Alt pic](./pictures/纤程.png)
![Alt pic](./pictures/管程.png)
![Alt pic](./pictures/协程.png)

---