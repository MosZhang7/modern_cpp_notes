#include <future>
#include <iostream>
int main() {
  std::async([] {
    std::cout << "Hello from async!" << std::endl;
  }).get(); // Ensure the async task completes before main exits

  std::packaged_task<int()> task([] {
    std::cout << "Hello from packaged_task!" << std::endl;
    return 42;
  });

  std::future<int> result = task.get_future();
  result.get();
  task(); // Execute the packaged task
}