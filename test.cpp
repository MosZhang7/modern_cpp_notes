#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <map>
#include <new>
#include <queue>
#include <set>
#include <stack>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
};

class Solution {
public:
  int maxSubArray(vector<int> &nums) {
    // 定义dp[i]表示以第i位置为结尾的子数组的最大和
    // dp[0] = nums[0];
    // dp[1] = max(dp[0] + nums[1], num[1]);
    // dp[i] = max(dp[i - 1] + nums[i], nums[i])

    int n = nums.size();
    int p = INT32_MIN;
    vector<int> dp(n, 0);
    dp[0] = nums[0];
    for (int i = 1; i < n; i++) {
      dp[i] = max(dp[i - 1] + nums[i], nums[i]);
    }
    return *max_element(dp.begin(), dp.end());
  }
};

int main() {
  vector<int> give_me_a_name{-2, -1};
  Solution s;
  cout << s.maxSubArray(give_me_a_name) << endl;
  return 0;
};
