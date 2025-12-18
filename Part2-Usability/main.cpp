#include <atomic>
#include <iostream>

int main(int, char **) {
  std::atomic_int a{0};
  a.load(std::memory_order_consume);
}
