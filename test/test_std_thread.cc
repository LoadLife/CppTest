#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <numeric>
#include <queue>
#include <random>
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
