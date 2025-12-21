#include <future>
#include <iostream>
int main() {
  std::promise<int> p;
  auto f = p.get_future();
  auto s = f.share();
  f.wait();
}