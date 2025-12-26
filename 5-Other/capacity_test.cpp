#include <cstddef>
#include <iostream>
#include <vector>

int main() {
  std::vector<int> v;
  size_t prev_cap = v.capacity();
  auto prev_data = v.data();
  std::cout << "initial =" << v.size() << " cap =" << prev_cap
            << " data =" << prev_data << "\n";
  for (size_t i = 1; i <= 100; ++i) {
    v.push_back(static_cast<int>(i));
    if (v.capacity() != prev_cap) {
      std::cout << "i=" << i << " size =" << v.size() << " cap =" << prev_cap
                << "->" << v.capacity();
      if (v.data() != prev_data)
        std::cout << " (realloc)";
      std::cout << "\n";
      prev_cap = v.capacity();
      prev_data = v.data();
    }
  }
  std::cout << "final size=" << v.size() << " cap=" << v.capacity() << "\n\n";
  return 0;
}
