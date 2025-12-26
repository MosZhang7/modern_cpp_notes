#include <future>
#include <iostream>
int main() {
  std::async([] {
    std::cout << "Hello from async!" << std::endl;
  }).get(); // Ensure the async task completes before main exits

  std::future<int> result;
  {
    std::packaged_task<int()> task([] {
      std::cout << "Hello from packaged_task!" << std::endl;
      return 42;
    });
    result = task.get_future();
  }
  try {
    result.get();

  } catch (const std::exception &e) {
    std::cerr << "Caught exception: " << e.what() << std::endl;
  }
  std::this_thread::sleep_for(std::chrono::seconds(100));
  // task(); // Execute the packaged task
}