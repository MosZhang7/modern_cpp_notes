#include <iostream>
#include <vector>
struct Bar {
  Bar(int x) { std::cout << "default\n"; }
  Bar(const Bar &) { std::cout << "copy\n"; };
  Bar(Bar &&) { std::cout << "move\n"; };
};

int main() {
  std::vector<Bar> v;
  v.reserve(100);
  Bar b(2);
  std::cout << "----\n";
  v.push_back(b);
  std::cout << "*\n";
  v.push_back(Bar(1));
  std::cout << "*\n";
  v.push_back(3);
  std::cout << "----\n";
  v.emplace_back(b);
  std::cout << "*\n";
  v.emplace_back(Bar(1));
  std::cout << "*\n";
  v.emplace_back(3);
  std::cout << "----\n";
  return 0;
}
