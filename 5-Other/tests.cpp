#include <string>
#include <string_view>
#include <vector>
using namespace std;

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> result;
  std::string current;

  for (char c : s) {
    if (c == delim) {
      result.push_back(current);
      current.clear();
    } else {
      current.push_back(c);
    }
  }
  result.push_back(current); // 最后一个字段

  return result;
}

class Solution {
public:
  int compareVersion(string version1, string version2) {
    string_view v1s(version1);
    // split('.')形成vector
    // 每个vector去掉前导0
    // 将vector给join成一个新的string
    // 逐位比较，直到到末尾
    vector<string> v1, v2;

    auto n1 = version1.size();
    int lp = 0, rp = 0;
    for (int i = 0; i < n1; i++) {
      if (version1[i] == '.') {
        rp = i;
        auto tmp = version1.substr(lp, rp);
        while (tmp.starts_with('0')) {
          tmp = tmp.substr(0, 1);
        }
        v1.push_back(tmp);
        lp = rp;
      }
    }

    auto n2 = version2.size();
    lp = rp = 0;
    for (int i = 0; i < n2; i++) {
      if (version2[i] == '.') {
        rp = i;
        auto tmp = version2.substr(lp, rp);
        while (tmp.starts_with('0')) {
          tmp = tmp.substr(0, 1);
        }
        v2.push_back(tmp);
        lp = rp;
      }
    }

    string vv1 = "", vv2 = "";
    for (auto &v : v1) {
      vv1 += v;
    }
    for (auto &v : v2) {
      vv2 += v;
    }
    auto length = min(vv1.size(), vv2.size());
    for (int i = 0; i < length; i++) {
      if (vv1[i] < vv2[i]) {
        return -1;
      }
      if (vv1[i] > vv2[i]) {
        return 1;
      }
    }
  }
};

int main() {
  Solution s;
  s.compareVersion("1.2", "1.10");
}