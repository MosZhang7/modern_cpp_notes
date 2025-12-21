#include <algorithm>
#include <set>
#include <vector>

#include <iostream>

using namespace std;
class Solution {
public:
  int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2,
                  int by2) {
    set<pair<int, int>> as;
    set<pair<int, int>> bs;
    for (int a = ax1; a <= ax2; a++) {
      for (int c = ay1; c <= ay2; c++) {
        as.insert({a, c});
      }
    }
    for (int a = bx1; a <= bx2; a++) {
      for (int c = by1; c <= by2; c++) {
        bs.insert({a, c});
      }
    }
    vector<pair<int, int>> c;
    set_intersection(as.begin(), as.end(), bs.begin(), bs.end(),
                     back_inserter(c));
    int s = c.size();
    int intersec = 0;
    if (s >= 1) {
      sort(c.begin(), c.end());
      auto &[r1, u1] = c[s - 1];
      auto &[r2, u2] = c[0];
      intersec = (r1 - r2) * (u1 - u2);
    }

    return (ay2 - ay1) * (ax2 - ax1) + (by2 - by1) * (bx2 - bx1) - intersec;
  }
};

int main() {
  Solution s;
  cout << s.computeArea(0, 0, 0, 0, -1, -1, 1, 1) << endl;
  return 0;
}