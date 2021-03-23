#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <numeric>
#include <queue>
#include <random>
#include <thread>
#include "gtest/gtest.h"

// a simple thread realization, copy from zhihu
class fixed_thread_pool {
 public:
  explicit fixed_thread_pool(size_t thread_count)
    : data_(std::make_shared<data>()) {
    for(size_t i = 0; i != thread_count; i++) {
      std::thread([data = data_] {
       std::unique_lock<std::mutex> lk(data->mtx_);
       for(;;){
         if(!data->tasks_.empty()) {
           auto current = std::move(data->tasks_.front());
           data->tasks_.pop();
           lk.unlock();
           current();
           lk.lock();
         } else if(data->is_shutdown_) {
           break;
         } else {
           data->cond_.wait(lk);
         }
       }
      }).detach();
    }
  }

  fixed_thread_pool() = default;
  fixed_thread_pool(fixed_thread_pool&& ) = default;

  ~fixed_thread_pool() {
    if((bool)data_) {
      std::lock_guard<std::mutex> lk(data_->mtx_);
      data_->is_shutdown_ = true;
    }
    data_->cond_.notify_all();
  }

  template <typename F>
  void execute(F&& task) {
    {
      std::lock_guard<std::mutex> lk(data_->mtx_);
      data_->tasks_.emplace(std::forward<F>(task));
    }
    data_->cond_.notify_one();
  }

 private:
  struct data {
    std::mutex mtx_;
    std::condition_variable cond_;
    bool is_shutdown_ = false;
    std::queue<std::function<void()>> tasks_;
  };
  std::shared_ptr<data> data_;
};
// 1.test thread_pool
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
  for(auto& i : src){
    i = distrib(e);
  }
  
  auto thread_num = std::thread::hardware_concurrency();
  std::vector<uint32_t> results(thread_num);
  size_t each_group_eles_num = src.size() / thread_num;
  size_t extra_num = src.size() % thread_num;
  auto func = [&results](std::vector<uint32_t>::iterator begin, size_t size, uint32_t result_index) {
    results.at(result_index) = std::accumulate(begin, begin + size, results.at(result_index));
  };

  std::vector<std::thread> threads;
  for(size_t i = 0; i != thread_num; i++){
    if(i != thread_num - 1) 
      threads.emplace_back(func, src.begin() + i * each_group_eles_num, each_group_eles_num, i); 
    else 
      threads.emplace_back(func, src.begin() + i * each_group_eles_num, each_group_eles_num + extra_num, i);
  }
  for(auto& i : threads)
    i.join();
  uint32_t result = 0, threads_result = 0;
  threads_result = std::accumulate(results.begin(), results.end(), threads_result);  
  result = std::accumulate(src.begin(), src.end(), result);
  ASSERT_EQ(threads_result, result);
}