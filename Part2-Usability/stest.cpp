#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  explicit ThreadPool(std::size_t thread_count) : stop_(false) {
    for (std::size_t i = 0; i < thread_count; ++i) {
      workers_.emplace_back([this] { worker_loop(); });
    }
  }

  ~ThreadPool() {
    {
      std::lock_guard<std::mutex> lk(mtx_);
      stop_ = true;
    }
    cv_.notify_all();

    for (auto& t : workers_) {
      if (t.joinable()) t.join();
    }
  }

  template <typename F, typename... Args>
  auto submit(F&& f, Args&&... args)
      -> std::future<std::invoke_result_t<F, Args...>> {
    using R = std::invoke_result_t<F, Args...>;

    // 1. 把函数和参数绑定成一个无参 callable
    auto bound = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

    // 2. 包装成 packaged_task<R()>
    auto task = std::make_shared<std::packaged_task<R()>>(std::move(bound));

    // 3. 拿到 future
    std::future<R> fut = task->get_future();

    {
      std::lock_guard<std::mutex> lk(mtx_);
      if (stop_) {
        throw std::runtime_error("ThreadPool is stopped");
      }

      // 4. 放入任务队列（type-erased 为 void()）
      tasks_.emplace([task] {
        (*task)();  // 调用 packaged_task
      });
    }

    cv_.notify_one();  // 唤醒一个 worker
    return fut;
  }

 private:
  void worker_loop() {
    while (true) {
      std::function<void()> job;

      {
        std::unique_lock<std::mutex> lk(mtx_);
        cv_.wait(lk, [this] { return stop_ || !tasks_.empty(); });

        if (stop_ && tasks_.empty()) {
          return;  // 优雅退出
        }

        job = std::move(tasks_.front());
        tasks_.pop();
      }

      job();  // 真正执行任务
    }
  }

 private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;

  std::mutex mtx_;
  std::condition_variable cv_;
  bool stop_;
};
