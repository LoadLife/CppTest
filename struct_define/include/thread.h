#include <atomic>
#include <mutex>
#include <thread>
#include <queue>

// a fixed_thread_pool realization, reference from `zhihu`
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


// a simple hierarchical_mutex, big->small
class hierarchical_mutex {
 public:
  explicit hierarchical_mutex(uint64_t value) :
    hierarchy_value(value), 
    previous_hierarchy_value(0) {}
  
  void lock() {
    check_for_hierarchy_violation();
    internal_mutex.lock();
    update_hierarchy_value();
  }

  void unlock() {
    this_thread_hierarchy_value = previous_hierarchy_value;
    internal_mutex.unlock();
  }

  bool try_lock() {
    check_for_hierarchy_violation();
    if(!internal_mutex.try_lock()) 
      return false;
    update_hierarchy_value();
    return true;
  }
  
 private:
  void check_for_hierarchy_violation() {
    if(this_thread_hierarchy_value <= hierarchy_value) {
      throw std::logic_error("mutex hierarchy violated");
    }
  }

  void update_hierarchy_value() {
    previous_hierarchy_value = this_thread_hierarchy_value;
    this_thread_hierarchy_value = hierarchy_value;
  }

  std::mutex internal_mutex;
  const uint64_t hierarchy_value;
  uint64_t previous_hierarchy_value;
  static thread_local uint64_t this_thread_hierarchy_value;
};

// a simple spinlock_mutex
class spinlock_mutex {
 public:
  spinlock_mutex() = default;
  void lock() {
    while(flag.test_and_set(std::memory_order_acquire));
  }

  void unlock() {
    flag.clear(std::memory_order_release);
  }

  bool try_lock() {
    return !flag.test_and_set(std::memory_order_acquire);
  }
 private:
  std::atomic_flag flag = ATOMIC_FLAG_INIT;
};