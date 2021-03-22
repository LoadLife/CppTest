#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <iostream>
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