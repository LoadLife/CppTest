#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <queue>
#include <random>
#include <shared_mutex>
#include <thread>
#include "thread.h"
#include "gtest/gtest.h"
// initial hierarchy_mutex's static value
thread_local uint64_t hierarchical_mutex::this_thread_hierarchy_value(UINT64_MAX);

// test thread_pool
TEST(Thread, pool) {
  fixed_thread_pool pool(3);
  auto main_id = std::this_thread::get_id();
  auto func = [main_id]() {
      ASSERT_NE(main_id, std::this_thread::get_id());
    };

  int local = 0x01;
  auto func_with_param = [&local](int& param, bool param_is_ref) {
      if(param_is_ref)
        ASSERT_EQ(&local, &param);
      else
        ASSERT_NE(&local, &param); 
    };

  auto bind_ref_func = std::bind(func_with_param, std::ref(local), true);
  auto bind_func = std::bind(func_with_param, local, false);

  pool.execute(func);
  pool.execute(func);
  pool.execute(bind_ref_func);
  pool.execute(bind_func);
}

// test class's internal func
struct test_internal {
  test_internal() = default;
  void equal(int param) {
    ASSERT_EQ(param, 1);
  }
};
TEST(Thread, internal_func) {
  test_internal x;
  int num = 0x1;
  std::thread t(&test_internal::equal, &x, num);
  t.join();
}

// std::for_each to invoke class method
void do_work(int i) {
  ASSERT_EQ(i,0x1);
}
TEST(Thread, for_each_invoke) {
  std::vector<std::thread> threads;
  for(size_t i = 0; i != 4 ; i++) {
    threads.emplace_back(do_work, 0x1);
  }
  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

// std::thread::hardware_concurrency()
TEST(Thread, hardware_concurrency) {
  std::vector<uint32_t> src(100);
  std::default_random_engine e;
  std::uniform_int_distribution<unsigned> distrib(0,0xffff);
  for(auto& i : src) {
    i = distrib(e);
  }
  uint32_t src_size = static_cast<uint32_t>(src.size());
  auto thread_num = std::thread::hardware_concurrency();
  uint32_t each_group_eles_num = src_size / thread_num;
  uint32_t extra_num = src_size % thread_num;
  if(src_size < thread_num){
    thread_num = 1;
    each_group_eles_num = src_size;
    extra_num = 0;
  }
  std::vector<uint32_t> results(thread_num);
  auto func = [&results](std::vector<uint32_t>::iterator begin, uint32_t size, uint32_t result_index) {
    results.at(result_index) = std::accumulate(begin, begin + size, results.at(result_index));
  };

  std::vector<std::thread> threads;
  for(uint32_t i = 0; i != thread_num; i++) {
    if(i != thread_num - 1) 
      threads.emplace_back(func, src.begin() + i * each_group_eles_num, each_group_eles_num, i); 
    else 
      threads.emplace_back(func, src.begin() + i * each_group_eles_num, each_group_eles_num + extra_num, i);
  }
  for(auto& i : threads) {
    i.join();
  }
  uint32_t result = 0, threads_result = 0;
  threads_result = std::accumulate(results.begin(), results.end(), threads_result);  
  result = std::accumulate(src.begin(), src.end(), result);
  ASSERT_EQ(threads_result, result);
}

// to avoid dead_lock, use std::lock(...) this case is meaningless, just use the std::lock()
class obj {
 public:
  obj() = default;
  friend void visit(obj& x, obj& y);
 private:
  int32_t value = 1;
  std::mutex lock;
};
void visit(obj& x, obj& y){
  std::lock(x.lock, y.lock);
  //in this pos, using std::adopt_lock to let lock_guard get mutex and manage mutex
  std::lock_guard<std::mutex> xlock(x.lock, std::adopt_lock);
  std::lock_guard<std::mutex> ylock(y.lock, std::adopt_lock);
}
TEST(Thread, lock_and_adopt_lock) {
  obj x,y;
  visit(x,y);
}

// test hierarchy lock
TEST(Thread, hierarchy_lock) {
  hierarchical_mutex high_lock(10000);
  hierarchical_mutex middle_lock(5000);
  hierarchical_mutex low_lock(1000);
  auto func1 = [&](){
    std::lock_guard<hierarchical_mutex> lock1(high_lock);
    std::lock_guard<hierarchical_mutex> lock2(middle_lock);
    std::cout << "lock success" << std::endl;
    };
  auto func2 = [&](){
    std::lock_guard<hierarchical_mutex> lock1(low_lock);
    std::lock_guard<hierarchical_mutex> lock2(high_lock);
    };

  try {
    func1();
    func2();
  } catch(std::logic_error err) {
    std::cout << err.what() << std::endl;
  }
}

// test std::defer_lock
TEST(Thread, defer_lock) {
  std::mutex m_1;
  std::mutex m_2;
  // std::defer_lock to defer lock time
  std::unique_lock<std::mutex> lock_1(m_1, std::defer_lock);
  std::unique_lock<std::mutex> lock_2(m_2, std::defer_lock);
  std::lock(lock_1, lock_2);
  if(lock_1.owns_lock() && lock_2.owns_lock()) {
    lock_1.unlock();
    lock_2.unlock();
  } 
}

/* std::once_flag std::call_once
  object's init may occupy lots of time,
  defer object's init until use it.
*/
std::shared_ptr<int> shared_int;
std::once_flag resource_flag;
void init_resource() {
  shared_int = std::make_shared<int>(10);
}
void do_something() {
  std::call_once(resource_flag, init_resource);
  std::cout << *shared_int <<std::endl;
}
TEST(Thread, call_once) {
  std::vector<std::thread> threads;
  for(uint32_t i = 0; i != 4; i++) {
    threads.emplace_back(do_something);
  }

  for(auto& i : threads){
    i.join();
  }
}

/* std::call_once used in class method. in 
   cpp11 and later, competition in initing 
   a static variable will not happen, we 
   need't use call_once to protect getInst-
   ance()
*/
class singleTon {
 public:
  static singleTon* getInstance() {
    static singleTon t;
    return &t;
  }

  void init() {
    std::call_once(init_flag, &singleTon::initInternal, this);
    ASSERT_EQ(times, 1);
  }
 private:
  void initInternal() {
    resource = std::make_shared<int>(0xff);
    times+=1;
  }
  singleTon() = default;
  std::once_flag init_flag;
  std::shared_ptr<int> resource;
  static unsigned times;
};
unsigned singleTon::times = 0;

void test_singleTon(){
  auto s = singleTon::getInstance();
  s->init();
}
TEST(Thread, singleTon) {
  std::vector<std::thread> threads;
  for(unsigned i = 0; i != 5; i++){
    threads.emplace_back(test_singleTon);
  }

  for(auto& i : threads){
    i.join();
  }
}

// test std::shared_mutex[cpp17]
/* when read/write multi operation in different thread, 
   write only low frequently happen. if we use mutex, it
   seems that we are hyperreactive, so we can use newest
   std::shared_mutex
*/
class lovely_girls {
  public:
    lovely_girls() = default;
    unsigned getGirlAge(std::string name) {
      // std::shared_lock seems a RAII class
      std::shared_lock<std::shared_mutex> lock(mutex);
      auto iter = sexy_girls.find(name);
      if(iter != sexy_girls.end()) {
        std::cout << iter->first << "'s age is " << iter->second << std::endl; 
        return iter->second;
      } else {
        std::cout << "you don't have such a girl" << std::endl;
        return UINT32_MAX;
      } 
    }

    void addNewGirl(std::string name, unsigned age) {
      std::lock_guard<std::shared_mutex> lock(mutex);
      sexy_girls[name] = age;
    }

  private:
    std::map<std::string, unsigned> sexy_girls;
    std::shared_mutex mutex;
};
TEST(Thread, shared_mutex) {
  lovely_girls* my_girls = new lovely_girls();
  my_girls->addNewGirl("yifei.liu", 17);
  my_girls->addNewGirl("xiao.cheng", 20);
  my_girls->addNewGirl("lisa", 19);

  std::vector<std::thread> threads;
  threads.emplace_back(&lovely_girls::getGirlAge, my_girls, std::string("yifei.liu"));
  threads.emplace_back(&lovely_girls::addNewGirl, my_girls, std::string("yin.zhu"), 32);
  threads.emplace_back(&lovely_girls::getGirlAge, my_girls, std::string("xiao.cheng"));
  
  for(auto& i : threads){
    i.join();
  }
}

// test std::recursive_mutex
/* multi lock std::mutex in one thread will cause undefined error,
   so we use std::recursive_mutex instead, if you lock recursive_mutex 
   3 times, you should unlock recursive mutex 3 times. but it is rash, 
   do't advise to use this method
*/
class lazy_boy{
 public:
  lazy_boy() = default;
  void playComputer(){
    std::lock_guard<std::recursive_mutex> lock(mutex);
    eyes.first = 0x2;
    watchMovie(); 
  }
 private:
  void watchMovie(){
    std::lock_guard<std::recursive_mutex> lock(mutex);
    eyes.second = 0x2;
  }
  std::recursive_mutex mutex;
  std::pair<unsigned, unsigned> eyes{1, 1};
};
TEST(Thread, recursive_mutex) {
  lazy_boy boy;
  boy.playComputer();
}

// std::async
struct async_task {
  unsigned getTaskNum(){
    return TaskNum;
  }
 private:
  unsigned TaskNum = 0x1;
};
TEST(Thread, async) {
  async_task task;
  auto future = std::async(&async_task::getTaskNum, &task);
  auto task_num = future.get();
  ASSERT_EQ(task_num, 0x1); 

  auto func = [] {
      std::cout << "thread : "<< std::this_thread::get_id() << " latter launch" << std::endl;
      return std::this_thread::get_id();
    };
  /* std::launch::deferred or std::launch tag means task will be delayed to launch
      when invoke future.get() or future.wait(), the async_task run in main thread*/
  auto future_latter = std::async(std::launch::deferred, func);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  future_latter.wait();
  std::cout << "thread : " << std::this_thread::get_id() << " first launch" << std::endl;
  EXPECT_EQ(std::this_thread::get_id(), future_latter.get());
}

// std::packaged_task return a std::future to get p_task's return value
class packaged_task_test{
 public:
  packaged_task_test() = default;
  template<typename Func>
  std::future<void> post_task(Func func) {
    std::packaged_task<void()> task(func);
    auto result = task.get_future();
    p_mutex_.lock();
    tasks_.push(std::move(task));
    p_mutex_.unlock();
    return result;
  }

  void execute_task() {
    while(tasks_.empty())
      continue;
    p_mutex_.lock();
    auto task = std::move(tasks_.front());
    tasks_.pop();
    p_mutex_.unlock();
    task();
  }
  
 private:
  std::queue<std::packaged_task<void()>> tasks_;
  std::mutex p_mutex_;
};
TEST(Thread, packaged_task) {
  packaged_task_test test;
  auto func = []() {
      std::cout << "hello future" << std::endl;
    };
  auto future = std::async(&packaged_task_test::post_task<void()>, &test, func);
  test.execute_task(); 
  auto inter_future = future.get();
  auto ret = inter_future.wait_for(std::chrono::milliseconds(100));
  ASSERT_EQ(ret, std::future_status::ready);
}

// test std::promise : this is just a promise object, not related to a callable
TEST(Thread, promise) { 
  std::promise<bool> p;
  auto func_set = [&p] {
      p.set_value(true);
    };

  auto func_get = [&p] {
      const std::future<bool>& future = p.get_future();
      future.wait();
      std::cout << "future get" <<std::endl;
    };

  std::thread t_1(func_get);
  std::thread t_2(func_set);
  t_1.join();
  t_2.join();
}

// test std::shared_future, std::future can't be copy, just movable
TEST(Thread, shared_future) {
  std::vector<int> resources;
  auto prepare_func =  [&resources](){
      resources.resize(10);
      return true;
    };  
  auto future = std::async(prepare_func);
  std::shared_future s_f(std::move(future));

  auto func_do_work1 = [&s_f](std::vector<int>& re) {
      auto local_sf = s_f;
      if(s_f.get()) {
        for(unsigned i = 0; i != 5; i++) {
          re.at(2 * i + 1) = 1;
        }
      }
    };

  auto func_do_work2 = [&s_f](std::vector<int>& re) {
      auto local_sf = s_f;
      if(s_f.get()) {
        for(unsigned i = 0; i != 5; i++) {
          re.at(2 * i) = 0;
        }
      }
    };
  std::thread t1(func_do_work1, std::ref(resources));
  std::thread t2(func_do_work2, std::ref(resources));
  t1.join();
  t2.join();
}

// test atomic is_lock_free()
TEST(Thread, how_to_realize_atomic) {
  std::atomic_bool b_value;
  /* this is a lock_free realization, not compiler or library just use lock
     only `std::atomic_flag` can confirm it is lock_free, other type depend on
     its compiler an library define */
  ASSERT_EQ(b_value.is_lock_free(), true);
  std::atomic_llong l_value;
  ASSERT_EQ(l_value.is_lock_free(), true);
}

// test atomic clear() && test_and_set()
TEST(Thread, test_and_set) {
  // atomic_flag must be initialed by this
  std::atomic_flag f = ATOMIC_FLAG_INIT;
  // destory
  //f.clear(std::memory_order_release);
  bool x = f.test_and_set(std::memory_order_acquire);
  ASSERT_FALSE(x);
  // test_and_set return previous value
  x = f.test_and_set(std::memory_order_acquire);
  ASSERT_TRUE(x);
} 

// test spinlock
TEST(Thread, spinlock) {
  std::vector<int> resource(10);
  spinlock_mutex s_mutex;
  auto func_do = [&s_mutex, &resource] {
      s_mutex.lock();
      for(unsigned i = 0; i != 10; i++) {
        resource.at(i) = i;
      }
      s_mutex.unlock();
      return true;
    };
  
  std::packaged_task<bool()> task(func_do);
  auto func_launch = [&task] {
      task();
    };

  auto func_read = [&task, &resource, &s_mutex] {
      auto f = task.get_future();
      ASSERT_TRUE(f.get());
      std::lock_guard<spinlock_mutex> lock(s_mutex);
      for(unsigned i = 0; i != 10; i++){
        ASSERT_EQ(resource.at(i), i); 
      }
      for(auto& i : resource){
        i *= 2;
      }
    };
  std::thread t1(func_launch);
  std::thread t2(func_read);
  t1.join();
  t2.join();
}