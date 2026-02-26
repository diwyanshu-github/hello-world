#include <iostream>
using namespace std;

#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
/*
======================
DYNAMIC PROGRAMMING (DP)
======================

When to think DP:
- Problem asks for MIN / MAX / COUNT / BEST way
- Overlapping subproblems + optimal substructure
- Choices at each step (pick / not pick, take / skip, left / right)

Core idea:
- Store answers of subproblems so we don’t recompute them.

--------------------------------
DP BLUEPRINT (Top-down or Bottom-up)
--------------------------------

1) Define the state
   dp[state] = answer for that state
   Examples:
   - dp[i] → answer using first i elements
   - dp[i][j] → answer for i, j indices
   - dp[i][w] → answer using items till i with capacity w

2) Base cases
   Smallest subproblems that are trivially solved
   Examples:
   - dp[0] = 0
   - dp[i][0] = 1
   - dp[n] = 1

3) Transition (the heart of DP)
   How current state depends on smaller states
   Example:
   dp[i] = min(dp[i-1], dp[i-2]) + cost[i]

4) Order of computation
   - Top-down (recursion + memo)
   - Bottom-up (loops)

5) Final answer
   Usually dp[n] or dp[0][0]

--------------------------------
TOP-DOWN (Memoization) TEMPLATE
--------------------------------
*/

int dp[/*state size*/];

int solve(int i)
{
    // base case
    if (i <= 0)
        return 0;

    // memoized result
    if (dp[i] != -1)
        return dp[i];

    // transition
    return dp[i] = min(solve(i - 1), solve(i - 2));
}

/*
--------------------------------
BOTTOM-UP (Tabulation) TEMPLATE
--------------------------------
*/

void solveDP(int n)
{
    vector<int> dp(n + 1, 0);

    // base cases
    dp[0] = 0;
    dp[1] = 1;

    // fill table
    for (int i = 2; i <= n; i++)
    {
        dp[i] = dp[i - 1] + dp[i - 2]; // transition
    }

    // answer -> dp[n]
}

/*
--------------------------------
COMMON DP PATTERNS
--------------------------------
1) 1D DP
   dp[i] depends on dp[i-1], dp[i-2]

2) 2D DP
   dp[i][j] depends on dp[i-1][j], dp[i][j-1]

3) Pick / Not Pick
   dp[i] = max(pick + dp[i-2], notPick + dp[i-1])

4) Knapsack-style
   dp[i][w] = max(dp[i-1][w], val[i] + dp[i-1][w-wt[i]])

--------------------------------
OPTIMIZATION
--------------------------------
- If dp[i] depends only on previous row → space optimize
- Use rolling variables when possible

--------------------------------
INTERVIEW GOLD RULES
--------------------------------
- Always define state clearly in words first
- Transition must reduce problem size
- DP index meaning > code itself
*/

int fibHelper(int n, vector<int> &memo)
{
    if (n == 0 || n == 1)
        return n;

    if (memo[n] != -1)
        return memo[n];
    return memo[n] = fibHelper(n - 1, memo) + fibHelper(n - 2, memo);
}
int fib(int n)
{
    // vector<int>memo(n+1,-1);
    // // return fibHelper(n,memo);
    // for(int i=0;i<=n;i++){
    //     if(i==0 || i==1) memo[i]=i;
    //     else
    //         memo[i] = memo[i-1]+memo[i-2];
    // }
    // return memo[n];
    if (n < 2)
        return n;
    int prev1 = 1;
    int prev2 = 0;
    for (int i = 2; i <= n; i++)
    {
        int now = prev1 + prev2;
        prev2 = prev1;
        prev1 = now;
    }
    return prev1;
}

int stairsHelper(int n, vector<int> &dp)
{
    if (dp[n] != -1)
        return dp[n];
    if (n == 1 || n == 0)
        return dp[n] = 1;
    else
        return dp[n] = stairsHelper(n - 1, dp) + stairsHelper(n - 2, dp);
}
int climbStairs(int n)
{
    if (n == 1 || n == 0)
        return 1;
    // return climbStairs(n-1)+climbStairs(n-2); //Time: O(2ⁿ) (repeated subproblems)

    vector<int> dp(n + 1, -1);
    // return stairsHelper(n,dp);

    // for(int i=0;i<=n;i++){
    //     if(i==0 || i==1) dp[i]=1;
    //     else dp[i]=dp[i-1]+dp[i-2];
    // }
    // return dp[n];

    int p1 = 1;
    int p2 = 1;
    for (int i = 2; i <= n; i++)
    {
        int steps = p1 + p2;
        p2 = p1;
        p1 = steps;
    }
    return p1;
}

// ! be careful of base case
int frogHelper(int n, vector<int> &h, vector<int> &dp)
{
    if (n == 0)
        return dp[n] = 0;
    if (n == 1)
        return dp[n] = abs(h[n] - h[n - 1]);
    if (dp[n] != -1)
        return dp[n];
    return dp[n] = min(frogHelper(n - 1, h, dp) + abs(h[n] - h[n - 1]),
                       frogHelper(n - 2, h, dp) + abs(h[n] - h[n - 2]));
}

int frogJumps(vector<int> &height)
{
    int n = height.size();
    if (n == 0 || n == 1)
        return 0;
    vector<int> dp(n, -1);
    // return frogHelper(n-1,height,dp);

    // for(int i=0;i<n;i++){
    //     if(i==0) dp[i]=0;
    //     else if(i==1) dp[i] = abs(height[i]-height[i-1]);
    //     else
    //         dp[i]=min(dp[i-2]+abs(height[i]-height[i-2]),
    //                     dp[i-1]+abs(height[i]-height[i-1]));
    // }
    // return dp[n-1];

    int p1 = abs(height[1] - height[0]);
    int p2 = 0;
    for (int i = 2; i < n; i++)
    {
        int cur = min(p1 + abs(height[i] - height[i - 1]), p2 + abs(height[i - 2] - height[i]));
        p2 = p1;
        p1 = cur;
    }
    return p1;
}

// -------------------- TOP-DOWN (Memoization) --------------------
// TC: O(n * k)
// Explanation:
// - There are n states (0 to n-1)
// - For each state, we try up to k jumps
// - Each state is computed once due to memoization
//
// SC: O(n)
// Explanation:
// - dp array of size n
// - Recursion stack depth up to n in worst case

int frogHelper(int n, vector<int> &h, vector<int> &dp, int k)
{
    if (n == 0)
        return dp[n] = 0; // Base case → O(1)
    if (dp[n] != -1)
        return dp[n]; // Memoized result → O(1)

    int mn = INT_MAX;

    // Loop runs k times → O(k)
    for (int i = 1; i <= k; i++)
    {
        if (n - i >= 0)
        {
            mn = min(mn,
                     frogHelper(n - i, h, dp, k) // Each state computed once
                         + abs(h[n] - h[n - i]));
        }
    }
    return dp[n] = mn;
}

// -------------------- SPACE-OPTIMIZED TABULATION --------------------
// TC: O(n * k)
// Explanation:
// - Outer loop runs n times
// - Inner loop runs k times
//
// SC: O(k)
// Explanation:
// - Only last k DP states are stored using circular buffer

int frogWithkJumps(vector<int> &height)
{
    int n = height.size();
    if (n == 0 || n == 1)
        return 0;

    int k = 2;
    // for(int i=0;i<n;i++){
    //     if(i==0){
    //         dp[i]=0;
    //         continue;
    //     }
    //     int mn=1e9;
    //     for(int j=1;j<=k;j++){
    //         if(i-j>=0){
    //             mn=min(mn,dp[i-j] + abs(height[i]-height[i-j]));
    //         }
    //     }
    //     dp[i]=mn;
    // }

    // return dp[n-1];

    // Stores last k dp values (space optimization)
    vector<int> prev(k, 1e9);
    prev[0] = 0; // dp[0] = 0

    // Outer loop → O(n)
    for (int i = 1; i < n; i++)
    {
        int mn = 1e9;

        // Inner loop → O(k)
        for (int j = 1; j <= k; j++)
        {
            if (i - j >= 0)
            {
                mn = min(mn,
                         prev[(i - j) % k] + abs(height[i] - height[i - j]));
            }
        }
        prev[i % k] = mn; // Overwrite in circular manner
    }

    return prev[(n - 1) % k];
}

// ! house robber-1 can not rob adjacent houses
int houseRobberHelper(int i, vector<int> &nums, vector<int> &dp)
{
    if (i >= nums.size())
        return 0;
    if (dp[i] != -1)
        return dp[i];
    // rob
    int rob = nums[i] + houseRobberHelper(i + 2, nums, dp);
    // not rob
    int notRob = houseRobberHelper(i + 1, nums, dp);
    return dp[i] = max(rob, notRob);
}
int rob(vector<int> &nums)
{
    // vector<int>dp(nums.size(),-1);
    // return houseRobberHelper(0,nums,dp);
    int n = nums.size();
    if (n == 1)
        return nums[0];
    // dp[0]=nums[0];
    // dp[1]=max(nums[0],nums[1]);
    // for(int i=2;i<n;i++){
    //     int notRob = dp[i-1];
    //     int rob = nums[i]+dp[i-2];
    //     dp[i]=max(rob,notRob);
    // }
    // return dp[n-1];

    int p2 = nums[0];
    int p1 = max(nums[0], nums[1]);
    for (int i = 2; i < n; i++)
    {
        int notRob = p1;
        int rob = nums[i] + p2;
        int curr = max(rob, notRob);
        p2 = p1;
        p1 = curr;
    }
    return p1;
}
// ! house robber 2
int houseRobberHelper(int i, int end, vector<int> &nums, vector<int> &dp)
{
    if (i > end)
        return 0;
    if (dp[i] != -1)
        return dp[i];
    // rob
    int rob = nums[i] + houseRobberHelper(i + 2, end, nums, dp);
    // not rob
    int notRob = houseRobberHelper(i + 1, end, nums, dp);
    return dp[i] = max(rob, notRob);
}
int rob(vector<int> &nums)
{
    int n = nums.size();
    if (n == 1)
        return nums[0];
    vector<int> dp(n, -1);
    vector<int> dp2(n, -1);
    // int case1 = houseRobberHelper(0, n - 2, nums, dp);
    // int case2 = houseRobberHelper(1, n - 1, nums, dp2);

    // dp[0] = nums[0];
    // dp[1] = max(nums[0], nums[1]);
    // for (int i = 2; i <= n - 2; i++)
    // {
    //     dp[i] = max(dp[i - 1], dp[i - 2] + nums[i]);
    // }
    // int case1 = dp[n - 2];
    // dp2[1] = nums[1];
    // if (n >= 3)
    //     dp2[2] = max(nums[1], nums[2]);
    // for (int i = 3; i <= n - 1; i++)
    // {
    //     dp2[i] = max(dp2[i - 1], dp2[i - 2] + nums[i]);
    // }
    // int case2 = dp2[n - 1];

    int p2 = nums[0];
    int p1 = max(nums[0], nums[1]);
    for (int i = 2; i <= n - 2; i++)
    {
        int curr = max(p1, p2 + nums[i]);
        p2 = p1;
        p1 = curr;
    }
    int case1 = p1;
    p2 = nums[1];
    if (n >= 3)
        p1 = max(nums[1], nums[2]);
    for (int i = 3; i <= n - 1; i++)
    {
        int curr = max(p1, p2 + nums[i]);
        p2 = p1;
        p1 = curr;
    }
    int case2 = p1;
    return max(case1, case2);
}
int main()
{

    return 0;
}