
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
class ThreadPool {
public:
  ThreadPool(std::size_t num_threads) : running(true) {
    workers.reserve(num_threads);
    for (int i = 0; i < num_threads; i++) {
      workers.emplace_back([this]() {
        while (true) {
          std::packaged_task<void()> task;
          {
            std::unique_lock<std::mutex> lock(queue_mtx);
            cv.wait(lock,
                    [this]() { return !tasks_queue.empty() || !running; });
            if (!running && tasks_queue.empty()) {
              return;
            }
            task = std::move(tasks_queue.front());
            tasks_queue.pop_front();
          }
          task();
        }
      });
    }
  }

  template <typename F, typename... Args>
  auto submit(F &&func, Args &&...args) {
    using R = std::invoke_result_t<F, Args...>;
    std::future<R> ret;

    std::packaged_task<R()> job([func = std::forward<F>(func),
                                 ... as = std::forward<Args>(args)]() mutable {
      return std::invoke(func, as...);
    });
    ret = job.get_future();
    {
      std::lock_guard<std::mutex> lock(queue_mtx);
      tasks_queue.emplace_back(
          std::packaged_task<void()>([t = std::move(job)]() mutable { t(); }));
    }
    cv.notify_one();
    return ret;
  }

  ~ThreadPool() {
    tasks_queue.clear();
    running = false;
    cv.notify_all();
    for (auto &&t : workers) {
      t.join();
    }
  }

private:
  bool running = false;

  std::vector<std::thread> workers;
  std::deque<std::packaged_task<void()>> tasks_queue;

  std::condition_variable cv;
  std::mutex queue_mtx;
};

int main() {
  ThreadPool pool(4);
  auto fut = pool.submit([] {
    std::cout << "Hello, World!" << std::endl;
    throw std::runtime_error("Test exception");
    return 42;
  });
  try {
    auto p = fut.get();
    std::cout << "Result: " << p << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
  }
  return 0;
}