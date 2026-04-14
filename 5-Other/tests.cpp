#include <filesystem>
#include <functional>
#include <iostream>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <map>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

int main() {
  unordered_map<pair<int, int>, int> testCases = {
      {{1, 0}, 0}, {{1, 1}, 0},   {{1, 2}, -1},
      {{2, 1}, 1}, {{1, 10}, -1}, {{10, 1}, 1},
  };
  std::hash<std::string> hash_fn;
  std::cout << hash_fn("100") << std::endl;
}