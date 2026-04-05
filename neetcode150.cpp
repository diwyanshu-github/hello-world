#include <iostream>
using namespace std;

#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm> // reverse
#include <numeric>
#include <string>
#include <sstream>
#include <cmath>

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    // Member Initialization Avoids default construction + reassignment. Directly initializes in memory.
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    // Always use initializer lists for constructors instead of assignment inside constructor body
};

// ! Time Based Key-Value Store
/*
Time Based Key Value Store (Short Explanation)

Goal:
Store key-value pairs with timestamps.
For get(key, t), return the value set at the largest timestamp <= t.

Key Idea:
For each key, maintain a sorted list of {timestamp, value}.
Use binary search to find the latest timestamp <= t.

Approach:
- set(key, val, t): append {t, val} to vector (timestamps increasing)
- get(key, t): binary search to find rightmost timestamp <= t
*/
// ! try upper bound
class TimeMap
{
    // key -> (timestamp, value), timestamps strictly increasing
    unordered_map<string, vector<pair<int, string>>> mp;

public:
    TimeMap() {}

    // Time: O(1) amortized
    // Space: O(1) per call
    void set(string key, string value, int timestamp)
    {
        mp[key].push_back({timestamp, value});
    }

    // Time: O(log T)
    // Space: O(1)
    string get(string key, int timestamp)
    {
        if (!mp.count(key))
            return "";

        auto &v = mp[key];
        int l = 0, r = v.size() - 1;
        string ans = "";

        // Binary search for largest timestamp <= given timestamp
        while (l <= r)
        {
            int mid = l + (r - l) / 2;
            if (v[mid].first <= timestamp)
            {
                ans = v[mid].second;
                l = mid + 1;
            }
            else
            {
                r = mid - 1;
            }
        }
        return ans;
    }
};
// !K Closest Points to Origin
/*
Goal:
Find k points with smallest distance from origin (0,0).
Test Case:
points = {{1,3}, {-2,2}, {2,-2}}
k = 2

Output:
{{-2,2}, {2,-2}}   // order can vary

Explanation:
Distance from origin = x^2 + y^2

(1,3)  -> 1^2 + 3^2 = 10
(-2,2) -> (-2)^2 + 2^2 = 8
(2,-2) -> 2^2 + (-2)^2 = 8

The 2 closest points have smallest distances: 8 and 8.
So return {-2,2} and {2,-2}.
*/

/*
K Closest Points to Origin (Short Explanation)


Key Idea:
Distance comparison only needs squared distance:
dist = x*x + y*y  (no sqrt needed)

Approach:
Use a max-heap of size k.
- Push each point with its distance
- If heap size > k, remove farthest point
- Remaining k points are the closest
*/
// ! try quick select
// Time Complexity:
// - Building heap: O(n log n)
//   (n pushes into min-heap)
// - Extract k elements: O(k log n)
// - Total: O((n + k) log n) ≈ O(n log n)
//
// Space Complexity:
// - Heap stores up to n elements: O(n)
// - Result stores k points: O(k)
// - Total auxiliary space: O(n)
// ! maintain a max heap of size k and
// !discard farther points to achieve TC: O(n log k) and SC: O(k)
#define pip pair<int, pair<int, int>>
vector<vector<int>> kClosest(vector<vector<int>> &points, int k)
{
    // priority_queue<pip, vector<pip>, greater<pip>> q; // min heap
    priority_queue<pip, vector<pip>> q; // max heap
    for (auto &v : points)
    {
        int x = v[0];
        int y = v[1];
        // compare squared distances to avoid floating-point precision issues
        int dist = x * x + y * y;
        q.push({dist, {x, y}});

        if (q.size() > k)
            q.pop();
    }
    vector<vector<int>> res;
    while (k--)
    {
        auto p = q.top();
        q.pop();
        auto [x, y] = p.second;
        res.push_back({x, y});
    }
    return res;
}

// ! Target sum
/*
Test Case:
nums   = {1,1,1,1,1}
target = 3

Output:
5

Explanation:
We assign '+' or '-' to each number to get sum = 3.

Possible ways:
+1 +1 +1 +1 -1 = 3
+1 +1 +1 -1 +1 = 3
+1 +1 -1 +1 +1 = 3
+1 -1 +1 +1 +1 = 3
-1 +1 +1 +1 +1 = 3

Total ways = 5


Target Sum (Short Explanation)

Goal:
Assign '+' or '-' to each number so the final sum equals target.

Key Idea:
Convert to subset sum:
Find subset with sum = (totalSum + target) / 2

Reason:
+subset - remaining = target
⇒ subset - (totalSum - subset) = target
⇒ 2 * subset = totalSum + target

If (totalSum + target) is odd or negative → return 0.

Then count subsets with that sum using DP.
*/
// ! convert to subset sum
int targetSum(vector<int> &nums, int n, int curr, int target, vector<vector<int>> &dp, int offset)
{
    if (n == nums.size())
        return curr == target;
    if (dp[n][curr + offset] != -1)
        return dp[n][curr + offset];
    int take = targetSum(nums, n + 1, curr + nums[n], target, dp, offset);
    int notTake = targetSum(nums, n + 1, curr - nums[n], target, dp, offset);
    return dp[n][curr + offset] = take + notTake;
}
int findTargetSumWays(vector<int> &nums, int target)
{
    // vector<int>dp(nums.size()+1,-1);
    int n = nums.size();
    int offset = accumulate(nums.begin(), nums.end(), 0);
    // Since the running sum ranges from −total to +total,
    // we use an offset and allocate 2×total+1 states
    // sum = -total  → index 0
    // sum = 0       → index total
    // sum = +total  → index 2*total
    vector<vector<int>> dp(n + 1, vector<int>(2 * offset + 1, -1));
    return targetSum(nums, 0, 0, target, dp, offset);
}
// ! merge triplets
/*
Merge Triplets to Form Target Triplet

Goal:
Given triplets[i] = [ai, bi, ci] and target = [x, y, z],
we can merge any number of triplets by taking element-wise max.
Return true if we can form exactly the target triplet.

Key Idea:
We only care about triplets that do not exceed the target.
Among valid triplets, we check if we can independently match
each dimension of the target.

Example:
triplets = {{2,5,3}, {1,8,4}, {1,7,5}}
target   = {2,7,5}

Step-by-step:

Triplet [2,5,3]
Valid? yes (2<=2, 5<=7, 3<=5)
Matches target[0] = 2 → good for first position

Triplet [1,8,4]
Invalid because 8 > 7 → discard (cannot use)

Triplet [1,7,5]
Valid (1<=2, 7<=7, 5<=5)
Matches target[1] = 7 and target[2] = 5

Now we have:
- target[0] satisfied by [2,5,3]
- target[1] satisfied by [1,7,5]
- target[2] satisfied by [1,7,5]

So merging gives:
max([2,5,3], [1,7,5]) = [2,7,5] = target

Answer: true
*/
// TC: O(n) SC: O(1)
// We greedily ignore triplets that exceed target.
bool mergeTriplets(vector<vector<int>> &triplets, vector<int> &target)
{
    bool x = false, y = false, z = false;
    for (auto &v : triplets)
    {
        if (v[0] <= target[0] && v[1] <= target[1] && v[2] <= target[2])
        {
            if (v[0] == target[0])
                x = true;
            if (v[1] == target[1])
                y = true;
            if (v[2] == target[2])
                z = true;
        }
        if (x && y && z)
            return true;
    }
    return false;
}

// !Minimum Interval to Include Each Query
/*
Minimum Interval to Include Each Query

Goal:
For every query q, return the size of the smallest interval [l, r]
such that l <= q <= r. If no interval contains q, return -1.

Key Idea:
For each query, many intervals may cover it, but we only care about
the one with minimum length (r - l + 1).

Example:
intervals = {{1,4}, {2,4}, {3,6}, {4,4}, {7,10}}
queries   = {2,3,4,5,8,11}

Step-by-step Explanation:

Query = 2
Valid intervals covering 2:
  [1,4] → size = 4
  [2,4] → size = 3
Choose smallest → 3

Query = 3
Valid intervals:
  [1,4] → 4
  [2,4] → 3
  [3,6] → 4
Choose smallest → 3

Query = 4
Valid intervals:
  [1,4] → 4
  [2,4] → 3
  [3,6] → 4
  [4,4] → 1   (best possible since exact match)
Choose smallest → 1

Query = 5
Valid intervals:
  [3,6] → 4
Answer → 4

Query = 8
Valid intervals:
  [7,10] → 4
Answer → 4

Query = 11
No interval satisfies l <= 11 <= r
Answer → -1

Final Output:
{3, 3, 1, 4, 4, -1}
*/
vector<int> minInterval(vector<vector<int>> &intervals, vector<int> &queries)
{
    // !brute O(N*Q)
    // sort(intervals.begin(), intervals.end(), // O(N log N)
    //      [](const auto& a, const auto& b) {
    //          return a[1] - a[0] < b[1] - b[0];
    //      });

    // vector<int> res;
    // for (auto q : queries) { // O(Q),
    //     bool found=false;
    //     for (auto& e : intervals) { // O(N)
    //         int x = e[0];
    //         int y = e[1];
    //         if (q >= x && q <= y) {
    //             found = true;
    //             res.push_back(y - x + 1);
    //             break;
    //         }
    //     }
    //     if(!found) res.push_back(-1);
    // }
    // return res;

    // !optimised
    // Sort intervals by start
    // Sort queries (but keep original index!)
    // For each query (in sorted order):
    //  Add intervals with start <= q
    //  Remove intervals with end < q
    //  Top of heap = smallest valid interval
    //  Save answer using original index

    // Step 1: Sort intervals by start
    sort(intervals.begin(), intervals.end(),
         [](const auto &a, const auto &b)
         {
             return a[0] < b[0];
         });

    // Step 2: Store queries with original index
    vector<pair<int, int>> q; // {query_value, original_index}
    for (int i = 0; i < queries.size(); i++)
    {
        q.push_back({queries[i], i});
    }

    sort(q.begin(), q.end());

    // Min-heap: {interval_length, interval_end}
    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>>
        pq;

    vector<int> ans(queries.size(), -1);

    int i = 0; // pointer for intervals

    // Step 3: Process queries in sorted order
    for (auto &[query, idx] : q)
    {

        // Add intervals whose start <= query
        while (i < intervals.size() && intervals[i][0] <= query)
        {
            int start = intervals[i][0];
            int end = intervals[i][1];
            int length = end - start + 1;

            pq.push({length, end});
            i++;
        }

        // Remove intervals that cannot cover query
        while (!pq.empty() && pq.top().second < query)
        {
            pq.pop();
        }

        // Smallest valid interval
        if (!pq.empty())
        {
            ans[idx] = pq.top().first;
        }
    }

    return ans;
    /*
   Time Complexity:
   - Sort intervals: O(N log N)
   - Sort queries:   O(Q log Q)
   - Each interval pushed once into heap: O(N log N)
   - Each interval popped at most once:   O(N log N)
   - Each query does heap operations:     O(Q log N)

   Overall:
   O((N + Q) log N)

   Space Complexity:
   - Heap stores at most N intervals → O(N)
   - Query with index array → O(Q)
   - Answer array → O(Q)

   Overall:
   O(N + Q)
   */
}

// ! minimum erases for non overlapping intervals
/*
Test Case: Erase Overlapping Intervals (Non-overlapping after removals)

intervals = {{1,2}, {2,3}, {3,4}, {1,3}}

Expected Output:
1

Explanation:
- Optimal non-overlapping set: [1,2], [2,3], [3,4]
- Remove [1,3] because it overlaps and ends later.
*/
int eraseOverlapIntervals(vector<vector<int>> &intervals)
{
    /*
    -> Key idea:
       If two intervals overlap, keep the one that ends earlier
       because it leaves more room for future intervals.

    Steps:
    1. Sort intervals by ending time (earliest end first).
       Greedy idea: keep the interval that finishes earliest so it leaves
       maximum room for future intervals.
    2. Track the end of the last non-overlapping interval.
    3. For each next interval:
       - If its start < current end → overlap → remove this interval (count++)
       - Else → no overlap → update current end to this interval's end
    4. Return the number of removed intervals.

    Time Complexity:
    - Sorting intervals: O(N log N)
    - Single pass scan: O(N)
    Overall: O(N log N)

    Space Complexity:
    - O(1) extra space (ignoring sort's internal recursion stack)
    */

    sort(intervals.begin(), intervals.end(),
         [](const auto &a, const auto &b)
         {
             return a[1] < b[1]; // sort by end time
         });

    int end = intervals[0][1]; // end of last chosen non-overlapping interval
    int count = 0;             // number of intervals removed

    for (int i = 1; i < intervals.size(); i++)
    {
        int x = intervals[i][0];
        int y = intervals[i][1];

        if (x < end)
        { // overlap → remove current interval
            count++;
            // keep the interval with smaller end (already ensured by sorting)
            end = min(end, y);
        }
        else
        { // no overlap → take this interval
            end = y;
        }
    }

    return count;
}

// ? Longest Repeating Character Replacement
/* Sliding Window template
    expand window
    update freq / state

    while (window invalid)
        shrink window

    update answer

*/
/*
Input: s = "AABABBA", k = 1
Output: 4
Explanation: Replace the one 'A' in the middle with 'B' and form "AABBBBA".
Approach (Sliding Window):

1. Use a window [start ... i] to represent current substring.
2. Maintain freq[26] to count characters in the window.
3. Track mx = highest frequency of any char in the window.
   → We try to make all chars equal to this most frequent char.
4. Replacements needed = window_size - mx.
   If replacements > k → shrink window from left.
5. Update answer with the largest valid window size.

Key Insight:
We don't decrease mx while shrinking.
An outdated mx may allow a slightly larger window,
but it never makes the final answer incorrect.
*/

int characterReplacement(string s, int k)
{
    vector<int> freq(26, 0);
    int start = 0, mx = 0, ans = 0;

    for (int i = 0; i < s.size(); i++)
    {
        freq[s[i] - 'A']++;             // add current char
        mx = max(mx, freq[s[i] - 'A']); // update max frequency

        while ((i - start + 1) - mx > k)
        {                           // if window invalid
            freq[s[start] - 'A']--; // remove left char
            start++;
        }

        ans = max(ans, i - start + 1); // update answer
    }
    return ans;
}

// !Pacific Atlantic Water Flow
/*
Problem:
Return cells from which water can flow to both Pacific (top/left edges)
and Atlantic (bottom/right edges). Water flows to equal/lower heights.

Idea:
Reverse flow → start DFS from ocean borders and move to higher/equal cells.
Cells reachable from both traversals are the answer.

TC: O(m*n) - Each cell is visited at most once in Pacific DFS and once in Atlantic DFS.
SC: O(m*n)
- Two visited matrices of size m*n.
- Recursion stack worst case O(m * n) for DFS.

Example:
heights = [[1,2,2],
           [3,2,3],
           [2,4,5]]
Output: [[0,2],[1,0],[1,1],[1,2],[2,0],[2,1],[2,2]]
*/

// bool isValid(int r, int c, int nr, int nc, vector<vector<int>>&m){
//     if(m[nr][nc] >= m[r][c]) return true;
//     return false;
// }
vector<vector<int>> dir = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
void dfs(vector<vector<int>> &m, int r, int c, vector<vector<bool>> &vis)
{
    vis[r][c] = true;
    for (auto &d : dir)
    {
        int nr = r + d[0];
        int nc = c + d[1];

        if (nc < 0 || nr < 0 || nc >= m[0].size() || nr >= m.size())
            continue;
        if (!vis[nr][nc] && m[nr][nc] >= m[r][c])
            dfs(m, nr, nc, vis);
    }
    // //up
    // if(r-1 >= 0 && !vis[r-1][c] && isValid(r,c,r-1,c,m)) dfs(m,r-1,c,vis);
    // //down
    // if(r+1 < m.size() && !vis[r+1][c] && isValid(r,c,r+1,c,m)) dfs(m,r+1,c,vis);
    // //left
    // if(c-1 >= 0 && !vis[r][c-1] && isValid(r,c,r,c-1,m)) dfs(m,r,c-1,vis);
    // //right
    // if(c+1 < m[0].size() && !vis[r][c+1] && isValid(r,c,r,c+1,m)) dfs(m,r,c+1,vis);
}
vector<vector<int>> pacificAtlantic(vector<vector<int>> &heights)
{
    // start from all top and left and mark -1 (visited by pacific)
    // start from all bottom and right, if already -1 means overlap, save ans
    int m = heights.size();
    int n = heights[0].size();
    vector<vector<bool>> vis(m, vector<bool>(n, false));
    vector<vector<int>> res;

    for (int i = 0; i < n; i++)
    {
        dfs(heights, 0, i, vis);
    }
    for (int j = 0; j < m; j++)
    {
        dfs(heights, j, 0, vis);
    }

    vector<vector<bool>> vis2(m, vector<bool>(n, false));
    for (int i = 0; i < n; i++)
    {
        dfs(heights, m - 1, i, vis2);
    }
    for (int j = 0; j < m; j++)
    {
        dfs(heights, j, n - 1, vis2);
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (vis[i][j] && vis2[i][j])
                res.push_back({i, j});
        }
    }

    return res;
}
// !Pacific Atlatic BFS
vector<vector<int>> pacificAtlanticBFS(vector<vector<int>> &heights)
{
    // start from all top and left and mark -1 (visited by pacific)
    // start from all bottom and right, if already -1 means overlap, save
    // ans
    int m = heights.size();
    int n = heights[0].size();
    vector<vector<bool>> vis(m, vector<bool>(n, false));
    vector<vector<bool>> vis2(m, vector<bool>(n, false));
    vector<vector<int>> res;

    queue<pair<int, int>> q; // row col
    // top
    for (int i = 0; i < n; i++)
    {
        vis[0][i] = true;
        q.push({0, i});
    }
    // left
    for (int i = 1; i < m; i++)
    {
        vis[i][0] = 1;
        q.push({i, 0});
    }

    while (!q.empty())
    {
        auto [r, c] = q.front();
        q.pop();

        for (auto &d : dir)
        {
            int nr = r + d[0];
            int nc = c + d[1];
            if (nr < 0 || nc < 0 || nr >= m || nc >= n)
                continue;

            if (!vis[nr][nc] && heights[nr][nc] >= heights[r][c])
            {
                vis[nr][nc] = 1;
                q.push({nr, nc});
            }
        }
    }

    queue<pair<int, int>> empty;
    swap(q, empty);

    // right
    for (int i = 0; i < m; i++)
    {
        vis2[i][n - 1] = 1;
        q.push({i, n - 1});
    }
    // down
    for (int i = 0; i < n; i++)
    {
        vis2[m - 1][i] = 1;
        q.push({m - 1, i});
    }
    while (!q.empty())
    {
        auto [r, c] = q.front();
        q.pop();

        for (auto &d : dir)
        {
            int nr = r + d[0];
            int nc = c + d[1];
            if (nr < 0 || nc < 0 || nr >= m || nc >= n)
                continue;

            if (!vis2[nr][nc] && heights[nr][nc] >= heights[r][c])
            {
                vis2[nr][nc] = 1;
                q.push({nr, nc});
            }
        }
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (vis[i][j] && vis2[i][j])
                res.push_back({i, j});
        }
    }

    return res;
}

// ! Partittion equal subsets

/*
!Koko Eating Bananas
Find minimum eating speed k so Koko finishes all piles within h hours.

Example:
piles = [3,6,7,11], h = 8 → Output: 4

TC: O(n log maxPile)
SC: O(1)
*/

bool isValid(vector<int> &piles, int k, int hours)
{
    int h = 0;
    for (int p : piles)
    {
        // h += (p + k - 1) / k; // ceil division
        h += ceil((double)p / k);
        if (h > hours)
            return false;
    }
    return true;
}

int minEatingSpeed(vector<int> &piles, int h)
{
    int l = 1;
    int hi = *max_element(piles.begin(), piles.end());

    while (l < hi)
    {
        int mid = l + (hi - l) / 2;
        if (isValid(piles, mid, h))
            hi = mid; // if valid eat slower
        else
            l = mid + 1; // need faster speed
    }
    return l;
}

/*
LC 853 — Car Fleet

Problem:
Given target distance and cars with position & speed,
return number of fleets that reach the target.
Cars cannot pass; faster cars may catch slower ones and form fleets.

Intuition:
Sort cars by position. Process from closest to target → farthest.
If a car takes longer than the fleet ahead → new fleet.
Else → it joins the existing fleet.

Example:
target = 12
position = [10,8,0,5,3]
speed    = [ 2,4,1,1,3]
Output: 3

TC: O(n log n)   // sorting
SC: O(n)         // storing cars
*/

int carFleet(int target, vector<int> &position, vector<int> &speed)
{
    int n = position.size();
    vector<pair<int, int>> cars; // {position, speed}

    for (int i = 0; i < n; i++)
        cars.push_back({position[i], speed[i]});

    sort(cars.begin(), cars.end()); // sort by position

    int fleets = 1;
    double prev_time = (target - cars[n - 1].first) * 1.0 / cars[n - 1].second;
    // if the lead fleet drives away, then count it and continue.
    // Otherwise, merge the fleets and continue.
    for (int i = n - 2; i >= 0; i--)
    {
        double curr_time = (target - cars[i].first) * 1.0 / cars[i].second;
        // if this can not catch up, new fleet else join same fleet
        if (curr_time > prev_time)
        { // new fleet
            fleets++;
            prev_time = curr_time;
        }
        // else joins fleet, dont update prev_time we will compare with slower in fleet
        // already curr_time < prev_time in else case
    }

    return fleets;
}

// ! sum of two integers without + and -
int getSum(int a, int b)
{
    // sum   = a ^ b        // add bits
    // carry = (a & b) << 1 // carry to next column
    // repeat until carry = 0
    while (b != 0)
    {
        int carry = (a & b) << 1;
        a = a ^ b;
        b = carry;
    }
    return a;
}

// ! Hand of straights
/*
Given an integer array hand where hand[i] is the value written on the ith
card and an integer groupSize, return true if she can rearrange the cards
into sets of groupSize consecutive numbers.

Input: hand = [1,2,3,6,2,3,4,7,8], groupSize = 3
Output: true
Explanation: Alice's hand can be rearranged as [1,2,3],[2,3,4],[6,7,8]

TC: O(n log n + n*groupSize)
SC: O(n)
Steps
    Sort the hand.
    Use frequency map to track counts.
    Start from smallest card and try to build groupSize consecutive cards.
    Decrement counts for each formed group.
 */
bool isNStraightHand(vector<int> &hand, int groupSize)
{
    int n = hand.size();
    if (n % groupSize != 0)
        return false;

    unordered_map<int, int> m; // card, freq
    for (int i = 0; i < n; i++)
    {
        m[hand[i]]++;
    }
    sort(hand.begin(), hand.end());
    for (int i = 0; i < n; i++)
    {
        if (m[hand[i]] > 0)
        {
            m[hand[i]]--;
            // cout<<hand[i]<<"->"<<m[hand[i]]<<" ";
            for (int j = 1; j < groupSize; j++)
            {
                if (m[hand[i] + j] != 0)
                {
                    m[hand[i] + j]--;
                    // cout<<hand[i]+j<<"->"<<m[hand[i]+j]<<" ";
                }
                else
                {
                    return false;
                }
            }
            // cout<<endl;
        }
    }
    return true;
}
/*
TC: O(n log n + n)
SC: O(n)
Idea:
Track how many consecutive groups are currently open.
Each card must extend existing groups before starting new ones.
Queue helps close groups after groupSize steps.
*/

bool isNStraightHand(vector<int> &hand, int k)
{
    if (hand.size() % k != 0)
        return false;
    map<int, int> m;
    queue<int> q;
    for (auto i : hand)
        m[i]++;

    int open = 0;
    int prev = -1;
    for (auto &c : m)
    {
        int card = c.first;
        int count = c.second;

        // next card is not consecutive and open group expects consecutive
        if (open > 0 && card != prev + 1)
            return false;

        // next card can not be put in all open groups
        if (open > 0 && count < open)
            return false;

        // after putting into open groups, if some cards remain, these will
        // be used by upcoming groups
        int newGroups = count - open;
        q.push(newGroups); // queue tracks count of groups starting from a card

        open = count;
        // current open groups: groups in which i put this card + new opened
        // groups thats why count
        prev = card;

        // Close groups after k steps
        if (q.size() == k)
        {
            // When queue.size() == k, we have processed k consecutive card values,
            // so any group that started k steps ago must now be complete.
            // front card will tell how many groups completed
            // q.front() can be 0 when no new groups started at that value.
            // Example: [1,1,2,2,3,3,4,4,5,5,6,6], k=3
            // groups start at 1 → 2 groups
            // at 2 → 0 new groups
            // at 3 → 0 new groups
            // queue timeline: [2,0,0] → pop → [0,0] → push 2 → [0,0,2]

            open -= q.front();
            q.pop();
        }
    }
    return open == 0;
}
// !Design twitter
/*
Twitter Feed (Heap Merge Approach) — Summary

Idea:
- Feed must show 10 most recent tweets from user + followees.
- Each user's tweets are stored in time order (latest last).

Algorithm:
1. Collect sources = {user} ∪ followees.
2. Push latest tweet from each source into a max-heap.
3. Repeat up to 10 times:
      - Pop most recent tweet → add to feed.
      - Push next older tweet from the same source (if exists).
4. Heap size ≈ number of sources (F + 1), not total tweets.

Why it works:
- Heap always holds the next possible most recent tweet from each source.
- Reveals older tweets only when newer ones are used.
- Equivalent to merging k sorted lists by timestamp.

Complexity:
- postTweet: O(1)
- getNewsFeed: build heap, pop from heap O(F log F + k log F)
- Space: O(F)

Key Insight:
- We never scan all tweets; we only explore tweets that can appear in top 10.
*/

/*
!brute
class Twitter {
    // unordered_map<int,vector<int>>posts; //userId, tweetIds
    vector<pair<int,int>>posts;     // userId, tweetId
    unordered_map<int, set<int>> followers; // userId, follows id
public:
    Twitter() {}

    void postTweet(int userId, int tweetId) {
        // posts[userId].push_back(tweetId);
        posts.push_back({userId, tweetId});
    }

    vector<int> getNewsFeed(int userId) {
        vector<int> feed;

        for (auto it = posts.rbegin(); it != posts.rend(); ++it) {
            int user = it->first;
            // vector<int>posts=it.second;
            int post = it->second;
            if (user == userId || followers[userId].count(user)) {
                // feed.insert(feed.end(),posts.rbegin(),posts.rend());
                feed.push_back(post);
                if (feed.size() == 10)
                    return feed;
            }
        }
        return feed;
    }

    void follow(int followerId, int followeeId) {
        followers[followerId].insert(followeeId);
    }

    void unfollow(int followerId, int followeeId) {
        followers[followerId].erase(followeeId);
    }
};
*/
/*
======================== LC 355: Time Complexity Comparison ========================

BRUTE FORCE APPROACH (global posts list)
----------------------------------------
Data structure:
    vector<pair<int,int>> posts;  // stores all tweets globally

postTweet:
    Time: O(1)
    -> push_back into vector

getNewsFeed:
    Time: O(N)
    where N = total tweets in system

    Why?
    -> Iterate from latest to oldest over ALL tweets
    -> Check if tweet belongs to user or followees
    -> Stop after collecting 10 tweets

    Worst case:
        User follows few people → must scan entire list
        => O(N)

follow/unfollow:
    Time: O(log F)
    where F = number of followees (set insert/erase)

Space:
    O(N + U + F)


------------------------------------------------------------------------------------

OPTIMAL HEAP APPROACH (k-way merge)
-----------------------------------
Data structure:
    unordered_map<int, vector<pair<int,int>>> posts; // per-user tweets
    priority_queue (max heap)

postTweet:
    Time: O(1)

getNewsFeed:
    Time: O(10 log k)
    where k = number of followees + self

    Why?
    -> Push latest tweet from each followee (k entries)
    -> Extract at most 10 tweets from heap
    -> Each pop/push costs log k

    Total ≈ O(10 log k) ≈ O(log k)

follow/unfollow:
    Time: O(log F)

Space:
    O(total tweets) + O(k) heap


------------------------------------------------------------------------------------

WHEN DOES BRUTE FORCE FAIL?
---------------------------
Example:
    Total tweets N = 10,000,000
    Followees k = 50

Brute:
    getNewsFeed = O(N) = 10M operations ❌

Heap:
    getNewsFeed = O(10 log 50) ≈ O(60) ✅


------------------------------------------------------------------------------------

INTERVIEW SUMMARY
-----------------
Brute force:
    ✔ Simple
    ❌ Not scalable
    ❌ O(N) per feed request

Heap merge (optimal):
    ✔ Scales with followees, not total tweets
    ✔ O(log k)
    ✔ Expected interview solution

====================================================================================
*/

class Twitter
{

    // user and tweets+timestamp
    unordered_map<int, vector<pair<int, int>>> posts;
    // user and followers
    unordered_map<int, set<int>> followers;
    int time = 0;

public:
    Twitter() {}

    void postTweet(int userId, int tweetId)
    {
        posts[userId].push_back({time, tweetId});
        time++;
    }

    vector<int> getNewsFeed(int userId)
    {
        // merge k sorted lists and stop at top 10 max
        // sources: user's posts, user->follow's posts
        // max heap for latest timestamp on top
        struct Node
        {
            int time, tweetId, userId, idx;
        };
        struct Compare
        {
            bool operator()(const Node &a, const Node &b)
            {
                return a.time < b.time;
            }
        };
        priority_queue<Node, vector<Node>, Compare> q;

        // sources: user, followers
        // push last post of users and followers in max heap
        int n = posts[userId].size();
        if (n > 0)
            q.push({posts[userId][n - 1].first, posts[userId][n - 1].second,
                    userId, n - 1});

        for (auto f : followers[userId])
        {
            int n = posts[f].size();
            if (n > 0)
                q.push(
                    {posts[f][n - 1].first, posts[f][n - 1].second, f, n - 1});
        }
        vector<int> feed;
        for (int i = 0; i < 10; i++)
        {
            if (q.empty())
                break;
            auto top = q.top();
            q.pop();
            feed.push_back(top.tweetId);
            // push next
            if (top.idx > 0)
            {
                q.push({posts[top.userId][top.idx - 1].first,
                        posts[top.userId][top.idx - 1].second, top.userId,
                        top.idx - 1});
            }
        }
        return feed;
    }

    void follow(int followerId, int followeeId)
    {
        followers[followerId].insert(followeeId);
    }

    void unfollow(int followerId, int followeeId)
    {
        followers[followerId].erase(followeeId);
    }
};

/**
 * Your Twitter object will be instantiated and called as such:
 * Twitter* obj = new Twitter();
 * obj->postTweet(userId,tweetId);
 * vector<int> param_2 = obj->getNewsFeed(userId);
 * obj->follow(followerId,followeeId);
 * obj->unfollow(followerId,followeeId);
 */

// !Top K frequent elements

/*
==================== 332. Reconstruct Itinerary ====================

Problem
-------
Given flight tickets [from, to], reconstruct the itinerary:
- Start from "JFK"
- Use ALL tickets exactly once
- Return lexicographically smallest valid itinerary

This is a graph problem:
    nodes  = airports
    edges  = tickets (directed)

--------------------------------------------------------------------

Intuition
---------
We must use every ticket exactly once → think "use all edges once".

Each airport has a list of outgoing flights.
We always choose the smallest lexical destination first.

Key idea:
    Use DFS and remove edges as we use them.
    Add airport to path only AFTER all outgoing edges are used
    (postorder DFS).

This builds the itinerary in reverse → reverse at the end.

Why postorder?
    It ensures we don't finalize an airport until all its tickets are used.

This is equivalent to finding an Eulerian path in a directed graph.

--------------------------------------------------------------------

Algorithm
---------
1. Build adjacency list: airport -> list of destinations
2. Reverse sort each list so smallest lexical choice is at the back
3. DFS from "JFK":
       while edges exist:
           go to next destination
           remove ticket
       add airport to path
4. Reverse path

--------------------------------------------------------------------

Complexity
----------
Let E = number of tickets

Time:
    Build graph:        O(E)
    Sorting edges:      O(E log E)
    DFS traversal:      O(E)
    ------------------------------
    Total:              O(E log E)

Space:
    Adjacency list:     O(E)
    Recursion stack:    O(E)
    Path storage:       O(E)
    ------------------------------
    Total:              O(E)
====================================================================
*/
void dfs(string node, unordered_map<string, vector<string>> &adj,
         vector<string> &path)
{

    while (!adj[node].empty())
    {
        string nei = adj[node].back();
        adj[node].pop_back();
        dfs(nei, adj, path);
    }
    // when no more paths from this node
    path.push_back(node);
}
vector<string> findItinerary(vector<vector<string>> &tickets)
{
    // we dont have n so we use map for adj, to create nodes dynamically
    // vector<vector<string>>adj(n)
    unordered_map<string, vector<string>> adj;
    for (auto &e : tickets)
    {
        adj[e[0]].push_back(e[1]);
    };
    for (auto &list : adj)
    {
        sort(list.second.rbegin(), list.second.rend());
    }

    // do dfs starting from jfk, also in lexiographical order
    // start from jfk, keep doing dfs and removing edge till nommore edges
    // push a node once no more edges left
    // eg JFK → KUL
    // JFK → NRT
    // NRT → JFK
    // if we push pre order jfk -> kul then again jfk -> nrt -> jfk
    // post order: [KUL, JFK, NRT, JFK]

    vector<string> path;
    dfs("JFK", adj, path);
    reverse(path.begin(), path.end());
    return path;
}

// !329. Longest Increasing Path in a Matrix
/*
Description
Given an m x n matrix, find the length of the longest strictly increasing path.
You can move in 4 directions (up, down, left, right).
You may not move diagonally or go out of bounds.

Key rules:
- Move only to a strictly larger value.
- Path length = number of cells visited.

The matrix forms a DAG because edges only go from smaller → larger values.

Simple Test Case
Input:
    matrix = [[9,9,4],
              [6,6,8],
              [2,1,1]]

Valid increasing path:
    1 → 2 → 6 → 9

Output:
    4

Approach Options
1) DFS + Memoization (Top-down DP)
   - Each cell computes longest path starting from itself.

2) Topological Sort (BFS)
   - Treat cells as DAG nodes and count BFS layers.

Both yield optimal solution.

Complexity
DFS + Memo:
    Time  : O(m * n)     // each cell computed once
    Space : O(m * n)     // memo + recursion stack

Topological Sort (BFS):
    Time  : O(m * n)     // process each edge once
    Space : O(m * n)     // indegree + queue

Key Insight
This is a longest path problem in a DAG formed by increasing edges.
Memoization or topo layering ensures efficient computation.
*/
int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
int dfs(int x, int y, vector<vector<int>> &matrix, vector<vector<int>> &vis, int m, int n)
{
    if (vis[x][y] != -1)
        return vis[x][y];
    int mx = 1;
    for (auto &d : dir)
    {
        int nr = x + d[0];
        int nc = y + d[1];
        if (nr < 0 || nr >= m || nc < 0 || nc >= n)
            continue;
        if (matrix[nr][nc] <= matrix[x][y])
            continue;
        mx = max(mx, 1 + dfs(nr, nc, matrix, vis, m, n));
    }
    return vis[x][y] = mx;
}
int longestIncreasingPath(vector<vector<int>> &matrix)
{
    // dfs from each cell, in visited stored length of paths

    int m = matrix.size();
    int n = matrix[0].size();
    vector<vector<int>> vis(m, vector<int>(n, -1));
    int res = 0;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (vis[i][j] == -1)
                res = max(res, dfs(i, j, matrix, vis, m, n));
        }
    }
    return res;
}

// ! number of connected componets (count no of dfs of graph)
// ! coin change (pick not pick dp)

// ! burst balloon
// ! interval dp
// Time:  O(n³)
// Space: O(n²)
int solve(int i, int j, vector<int> &nums, vector<vector<int>> &dp)
{
    if (i > j)
        return 0;
    if (dp[i][j] != -1)
        return dp[i][j];

    int ans = -1e9; // for this range
    for (int k = i; k <= j; k++)
    {
        // if kth bursts last
        int curr = nums[i - 1] * nums[k] * nums[j + 1];
        // burst this, but this is bursting last
        // so all in range i to j are burst, for cost use left and right outside this range
        int tempans = curr + solve(i, k - 1, nums, dp) + solve(k + 1, j, nums, dp);
        ans = max(ans, tempans);
    }
    return dp[i][j] = ans;
}
int maxCoins(vector<int> &nums)
{
    nums.insert(nums.begin(), 1);
    nums.push_back(1);
    int n = nums.size();
    vector<vector<int>> dp(n, vector<int>(n, -1));
    return solve(1, n - 2, nums, dp);
}

// ! buy sell stock with cooldown
// States:  n * 2
// Time:  O(n)
// Space: O(n)
int solve(int i, vector<int> &prices, int holding, vector<vector<int>> &dp)
{
    if (i >= prices.size())
        return 0;
    if (dp[i][holding] != -1)
        return dp[i][holding];
    int op1, op2;
    if (holding)
    { // sell, not sell
        op1 = prices[i] + solve(i + 2, prices, 0, dp);
        op2 = solve(i + 1, prices, 1, dp);
    }
    else // buy not buy
    {
        op1 = -prices[i] + solve(i + 1, prices, 1, dp);
        op2 = solve(i + 1, prices, 0, dp);
    }
    return dp[i][holding] = max(op1, op2);
}
int maxProfit(vector<int> &prices)
{
    int n = prices.size();
    vector<vector<int>> dp(n, vector<int>(2, -1));
    return solve(0, prices, 0, dp);
}

// ! Longest increasing subsequence
int solve(int i, int prev, vector<int> &nums, vector<vector<int>> &dp)
{
    if (i >= nums.size())
        return 0;
    if (dp[i][prev + 1] != -1)
        return dp[i][prev + 1];
    // pick, not pick
    int notPick = solve(i + 1, prev, nums, dp);
    int pick = 0;
    if (prev == -1 || nums[prev] < nums[i])
    {
        pick = 1 + solve(i + 1, i, nums, dp);
    }
    return dp[i][prev + 1] = max(pick, notPick);
}
int lengthOfLIS(vector<int> &nums)
{
    // vector<vector<int>>dp(nums.size(),vector<int>(nums.size()+1,-1));
    // return solve(0,-1,nums,dp);
    vector<int> temp;
    temp.push_back(nums[0]);
    for (int i = 1; i < nums.size(); i++)
    {
        if (nums[i] > temp.back())
        {
            temp.push_back(nums[i]);
        }
        else
        {
            int idx = lower_bound(temp.begin(), temp.end(), nums[i]) - temp.begin();
            temp[idx] = nums[i];
        }
    }
    return temp.size();
}

// ! serialize desrialize binary tree
// ! MedianFinder (Two Heaps) — Key Ideas
// left  = max heap → smaller half
// right = min heap → larger half
// Invariants:
//   1) max(left) ≤ min(right)
//   2) size(left) == size(right) OR size(left) = size(right)+1
//
// addNum(x):
//   push into appropriate heap
//   rebalance sizes (avoid size() subtraction → use comparisons)
//
// findMedian():
//   if sizes equal → avg(left.top(), right.top())
//   else → left.top()
//
// Time: addNum O(log n), findMedian O(1)
class MedianFinder
{
    // vector<int>nums;
    // int n;
    priority_queue<int> left;                             // max heap, all smaller than median
    priority_queue<int, vector<int>, greater<int>> right; // min heap, all bigger
public:
    MedianFinder()
    {
        // n=0;
    }

    void addNum(int num)
    {
        // nums.push_back(num);
        // n++;
        if (left.size() == 0 || num <= left.top())
            left.push(num);
        else
            right.push(num);

        // re balance
        while (left.size() > right.size() + 1)
        {
            int ltop = left.top();
            left.pop();
            right.push(ltop);
        }
        while (right.size() > left.size())
        {
            int rtop = right.top();
            right.pop();
            left.push(rtop);
        }
    }

    double findMedian()
    {
        // sort(nums.begin(),nums.end());
        // if(n%2 != 0) return 1.0 * nums[n/2];
        // else{
        //     double x=nums[n/2];
        //     double y=nums[(n/2)-1];
        //     return y - (y-x)/2;
        // }
        if (left.size() == right.size())
        {
            return (1.0 * left.top() + right.top()) / 2;
        }
        else
            return left.top();
    }
};
// !LC 287 — Find Duplicate Number (Floyd’s Cycle Detection)
//
// Treat array as linked list: i → nums[i]
// Duplicate value ⇒ cycle. Cycle entry = duplicate.
//
// Distances:
// a = start → cycle entry
// b = entry → meeting point
// c = meeting → entry
// cycle length = b + c
//
// Phase 1 meeting condition:
//   dist(slow) = a + b
//   dist(fast) = 2 * dist(slow)
//
// Fast also travels full cycles:
//   2(a + b) = a + b + (b + c)
// ⇒ a = c
//
// Meaning:
// distance(start → entry) == distance(meeting → entry)
//
// Phase 2:
//   slow = start
//   fast = meeting point
//   move both 1 step → meet at cycle entry (duplicate)
//
// After reset:
// slow travels a steps → entry
// fast travels c steps → entry
// ⇒ they meet at duplicate.
int findDuplicate(vector<int> &nums)
{
    // slow fast
    int slow = nums[0];
    int fast = nums[nums[0]];
    while (slow != fast)
    {
        slow = nums[slow];
        fast = nums[nums[fast]];
    }
    // return nums[slow];
    slow = 0;
    while (fast != slow)
    {
        slow = nums[slow];
        fast = nums[fast];
    }
    return slow;
}

// !Cheapest flight within k stops
/*
int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        vector<vector<pair<int, int>>> adj(n);
        for (auto& f : flights) {
            adj[f[0]].push_back({f[1], f[2]});
        }
        // cost node stops
        priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>>q;

        vector<int> cost(n, 1e8);
        cost[src] = 0;
        q.push({0, src, 0});

        while (!q.empty()) {
            auto t = q.top();
            q.pop();
            int currCost = t[0];
            int node = t[1];
            int edges = t[2];


            for (auto& v : adj[node]) {
                int nei = v.first;
                int neiCost = v.second;
                // k stops means k+1 edges can be used
                if (edges + 1 <= k + 1 && cost[nei] > currCost + neiCost) {

                    current code ensures path till nei is <= k jumps but if dest
                    is say 10 jumps away from this nei and i updated cost of nei
                    when it was k-1, so updated nei cost is unusable but i still
                    updated it as it was within k jumps.
                    ✅ Path to nei ≤ k edges
                    ❌ Remaining edges may be insufficient
                    ❌ Yet cost overwrite blocks valid paths
                    updating invalid path's cost is the issue
                    state doesn't depend on cost alone
                    the real state is: node + remaining edges
                    so track this combination

                    cost[nei] = currCost + neiCost;
                    q.push({cost[nei], nei, edges + 1});
                }
            }
        }

        return cost[dst] == 1e8 ? -1 : cost[dst];
    }
*/
// Time:  O(K * E)
// Space: O(V)
int findCheapestPrice(int n, vector<vector<int>> &flights, int src, int dst, int k)
{
    // use normal queue bfs with k limit
    // update whenever better cost (already in k limit)

    vector<vector<pair<int, int>>> adj(n);
    // node->{nei,cost}

    for (auto &f : flights)
    {
        adj[f[0]].push_back({f[1], f[2]});
    }
    vector<int> cost(n, INT_MAX);
    queue<vector<int>> q;
    // cost, node, stops
    q.push({0, src, 0});
    while (!q.empty())
    {
        auto t = q.front();
        q.pop();
        int costTillNode = t[0];
        int node = t[1];
        int stops = t[2];

        if (stops > k) // can not go further now
            continue;

        for (auto &nei : adj[node])
        {
            int next = nei.first;
            int nextCost = nei.second;
            // update if better cost
            if (cost[next] > costTillNode + nextCost)
            {
                cost[next] = costTillNode + nextCost;
                q.push({cost[next], next, stops + 1});
            }
        }
    }
    return cost[dst] == INT_MAX ? -1 : cost[dst];
}

// ! using bellman ford
/*
Example (why temp is needed): distance updates may cascade within
the same iteration

Graph:
0 -> 1 -> 2 -> 3   (cost = 1 each)
k = 1  → max edges allowed = 2

Without temp (WRONG):
Round 1:
0->1 updates cost[1]=1
1->2 uses updated cost[1] → cost[2]=2
2->3 uses updated cost[2] → cost[3]=3  ❌ (used 3 edges in one round)

With temp (CORRECT):
Round 1:
only 0->1 updates → cost[1]=1
Round 2:
1->2 updates → cost[2]=2
(stop here, edge limit respected)

temp ensures each round adds only ONE edge.
*/
int findCheapestPrice(int n, vector<vector<int>> &flights, int src, int dst, int k)
{
    // use bellman ford, relax k+1 edges (k stops will use k+1 edges)
    // if dst reached return cost else -1
    vector<int> cost(n, 1e9);
    cost[src] = 0;

    for (int i = 0; i < k + 1; i++)
    {
        vector<int> temp = cost;
        for (auto &e : flights)
        {
            int node = e[0];
            int nei = e[1];
            int neiCost = e[2];
            if (temp[nei] > cost[node] + neiCost)
            {
                temp[nei] = cost[node] + neiCost;
            }
        }
        cost = temp;
    }
    return cost[dst] == 1e9 ? -1 : cost[dst];
}
// ! walls and gates
/*
Problem
You are given a 2D grid:
0 → Gate
-1 → Wall
INF (2147483647) → Empty room
 Fill each empty room with distance to its nearest gate.
 If unreachable → keep INF.
[
  [INF, -1,  0, INF],
  [INF, INF, INF, -1],
  [INF, -1, INF, -1],
  [  0, -1, INF, INF]
]
output:
[
  [3, -1, 0, 1],
  [2,  2, 1, -1],
  [1, -1, 2, -1],
  [0, -1, 3, 4]
]
Key Idea: Multi-source BFS from all gates
Time:  O(m * n)
Space: O(m * n)
*/
vector<vector<int>> dir = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
const int INF = 1e8;
vector<vector<int>> gates(vector<vector<int>> grid)
{
    int m = grid.size();
    int n = grid[0].size();
    queue<pair<int, int>> q; // row, col
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 0)
                q.push({i, j});
        }
    }

    int steps = 0;
    while (!q.empty())
    {
        steps++;
        int sz = q.size();
        for (int i = 0; i < sz; i++)
        {
            auto t = q.front();
            q.pop();
            int r = t.first;
            int c = t.second;
            for (auto d : dir)
            {
                int nr = r + d[0];
                int nc = c + d[1];
                if (nr >= m || nr < 0 || nc >= n || nc < 0)
                    continue;
                if (grid[nr][nc] == INF)
                {
                    grid[nr][nc] = steps;
                    q.push({nr, nc});
                }
            }
        }
    }
    return grid;
}
/*
// !LeetCode 778. Swim in Rising Water

Problem:
Given an n x n grid where grid[r][c] is elevation, find the minimum time t
such that you can move from (0,0) to (n-1,n-1) when water level = t.
Time of a path = maximum elevation on that path.

Test Case:
grid = {
  {0, 2},
  {1, 3}
}
Answer = 3  // path: 0 → 1 → 3, max elevation = 3

Intuition:
We need the path that minimizes the maximum elevation encountered.
Use a min-heap to always expand the lowest reachable cell (Dijkstra-like),
and track the max height seen so far as the answer.

Approach:
Use a min-heap to always expand the lowest-height reachable cell,
ensuring we explore globally optimal elevations first.

Track mn = max(mn, h) because the path cost is the maximum height encountered, not steps taken.

Stop when destination is popped — the first time we reach it guarantees the minimum possible maximum height.

Time Complexity:  O(n^2 log n)
Time Complexity: O(n^2 log(n^2))
Since heap size can grow up to n^2.
log(n^2) = 2 log n  → drop constant → O(n^2 log n)
Space Complexity: O(n^2)
- Visited/grid marking stores n^2 cells.
- Heap can contain up to n^2 cells in worst case.
*/
using T = tuple<int, int, int>;
// height row col
vector<vector<int>> dir = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
int swimInWater(vector<vector<int>> &grid)
{
    // find minimum max in my path to dest
    // find a path to dest in which max value of path is minimum among all paths
    // ans depends on max value in a path, not on path length
    int n = grid.size();
    vector<vector<int>> vis(n, vector<int>(n, 0));
    // at a point i want minimum next height, i can use min heap with {heights, row, col}
    priority_queue<T, vector<T>, greater<T>> q; // min heap
    // for visited mark 1e8 in grid
    int mn = grid[0][0];
    q.push({grid[0][0], 0, 0});
    grid[0][0] = 1e8;
    while (!q.empty())
    {
        auto [h, r, c] = q.top();
        q.pop();
        // if i popped this, obv height is min across all paths
        // now i want max val of this path as ans
        mn = max(mn, h);
        if (r == n - 1 && c == n - 1)
            break;
        for (auto &d : dir)
        {
            int nr = r + d[0];
            int nc = c + d[1];
            if (nr < 0 || nc < 0 || nr >= n || nc >= n)
                continue;
            if (grid[nr][nc] == 1e8)
                continue;
            q.push({grid[nr][nc], nr, nc});
            grid[nr][nc] = 1e8;
        }
    }
    return mn;
}

// !encode decode string
/*
Design an algorithm to encode a list of strings into a single string
and decode it back to the original list.

Sample:
Input: ["lint","code","love",""]

Encoded:
"4#lint4#code4#love0#"

Decoded:
["lint","code","love",""]

Time Complexity: O(n)
Space Complexity: O(1) extra (excluding output)
*/
class Codec
{
public:
    string encode(vector<string> &strs)
    {
        string e;
        for (string &s : strs)
        {
            int l = s.size();
            e += to_string(l) + "#";
            e += s;
        }
        return e;
    }
    vector<string> decode(string s)
    {
        vector<string> res;
        int i = 0;
        int sz = s.size();
        while (i < sz)
        {
            int len = 0;
            string curr = "";
            while (s[i] != '#')
            {
                len = len * 10 + (s[i] - '0');
                i++;
            }
            i++; // skip #
            curr = s.substr(i, len);
            res.push_back(curr);
            i += len;
        }
        return res;
    }
};

// !Partition Lables
// two ways O(n) greedy, O(nlogn) intervals
vector<int> partitionLabels(string s)
{
    vector<int> res;
    map<char, pair<int, int>> m; // d {start,end};
    // at last iterate map and merge overlapping intervals
    for (int i = 0; i < s.size(); i++)
    {
        if (m.count(s[i]))
        {
            // auto [start, end] = m[s[i]];
            // m[s[i]] = {start, i};
            auto &p = m[s[i]];
            p.second = i;
        }
        else
        {
            m[s[i]] = {i, i};
        }
    }
    // merge intervals
    // need sorted intervals
    vector<vector<int>> intervals;
    for (auto &[c, interval] : m)
    {
        auto [start, end] = interval;
        intervals.push_back({start, end});
    }
    sort(intervals.begin(), intervals.end());
    int start = intervals[0][0];
    int end = intervals[0][1];

    for (int i = 1; i < intervals.size(); i++)
    {
        int currS = intervals[i][0];
        int currE = intervals[i][1];
        if (currS < end)
        { // merge
            end = max(end, currE);
        }
        else
        {
            res.push_back(end - start + 1);
            start = currS;
            end = currE;
        }
    }
    res.push_back(end - start + 1);
    return res;
}
// TC O(n) SC O(1) 26 characters
vector<int> partitionLabels(string s)
{
    vector<int> last(26, 0);
    for (int i = 0; i < s.size(); i++)
    {
        last[s[i] - 'a'] = i;
    }
    /*
    eg: defeghhijh
    d → 0
    e → 3
    f → 2
    g → 4
    h → 9
    i → 7
    j → 8

    now on iterating
    d e f e g h h i j h
    0 3 2 3 4 9 9 7 8 9

    each char tells limit till which it can extend its interval
    so at every char update max limit
    if curr index is the max limit so partition
*/

    int start = 0;
    int end = 0;
    vector<int> res;
    for (int i = 0; i < s.size(); i++)
    {
        end = max(end, last[s[i] - 'a']);
        if (i == end)
        {
            res.push_back(end - start + 1);
            start = i + 1;
        }
    }
    return res;
}

// ! alien dictionary
/*
LeetCode 269 / Alien Dictionary

Problem:
Given words sorted in an alien language, determine the character order.

Sample:
words = ["wrt","wrf","er","ett","rftt"]
Output: "wertf"

Idea:
1. Compare adjacent words to find first differing character → create edge (u → v).
2. Build graph + indegree.
3. Apply Kahn's Topological Sort (BFS).
4. If result size < unique chars → cycle → invalid.

Time Complexity: O(total characters in words + edges)
Space Complexity: O(unique characters + edges)
*/
string findOrder(vector<string> &words)
{
    unordered_map<char, vector<char>> adj;
    unordered_map<char, int> indegree;

    // collect all characters
    for (auto &w : words)
        for (char c : w)
            indegree[c] = 0;

    // build graph
    for (int i = 0; i < words.size() - 1; i++)
    {
        string &w1 = words[i];
        string &w2 = words[i + 1];

        if (w1.size() > w2.size() && w1.substr(0, w2.size()) == w2)
            return "";

        int j = 0;
        while (j < min(w1.size(), w2.size()) && w1[j] == w2[j])
            j++;

        if (j < min(w1.size(), w2.size()))
        {
            adj[w1[j]].push_back(w2[j]);
        }
    }

    // compute indegree
    for (auto &[u, nodes] : adj)
        for (char v : nodes)
            indegree[v]++;

    queue<char> q;
    for (auto &[c, deg] : indegree)
        if (deg == 0)
            q.push(c);

    string res;

    while (!q.empty())
    {
        char u = q.front();
        q.pop();
        res += u;

        for (char v : adj[u])
        {
            if (--indegree[v] == 0)
                q.push(v);
        }
    }

    if (res.size() != indegree.size())
        return "";
    return res;
}
/*
// !Check if graph is a valid tree.

Conditions:
1. Number of edges must be n-1.
2. Graph must be connected.
3. Graph must have no cycle.

Approach:
Run DFS from node 0 with parent tracking.
If we see a visited neighbor that is not parent → cycle.
After DFS check if all nodes were visited.

Time:  O(V + E)
Space: O(V)
*/

bool dfs(int node, int parent, vector<vector<int>> &adj, vector<bool> &vis)
{
    vis[node] = true;
    for (int nei : adj[node])
    {
        if (!vis[nei])
        {
            if (dfs(nei, node, adj, vis))
            {
                return true;
            }
        }
        else if (nei != parent)
        {
            return true;
        }
    }
    return false;
}
int isTree(int n, int m, vector<vector<int>> &edges)
{
    if (m != n - 1)
        return 0;
    vector<vector<int>> adj(n);

    // build adjacency list
    for (auto &e : edges)
    {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }
    vector<bool> vis(n, false);
    bool cycle = dfs(0, -1, adj, vis);
    if (cycle)
        return 0;
    for (int i = 0; i < n; i++)
    {
        if (vis[i] == false)
            return 0;
    }
    return 1;
}
// ! meeting rooms
/*
LC 252 - Meeting Rooms
Sort intervals by start time and check overlap.
If curr.start < prev.end → overlap → return false.
Else update prev.end.

Time: O(n log n)
Space: O(1)
*/
bool canAttend(vector<vector<int>> &arr)
{
    // Code Here
    sort(arr.begin(), arr.end());
    int prevEnd = arr[0][1];
    for (int i = 1; i < arr.size(); i++)
    {
        int nextStart = arr[i][0];
        int nextEnd = arr[i][1];

        if (nextStart < prevEnd)
            return false;
        prevEnd = nextEnd;
    }
    return true;
}

// !meeting rooms 2
/*
LC 253 - Meeting Rooms II

Given meeting intervals [start,end], return minimum rooms needed
so that no meetings overlap in the same room.

Example:
intervals = {{0,30},{5,10},{15,20}}

0-30 uses room1
5-10 overlaps → room2
15-20 uses freed room2

Answer = 2

Sort meetings by start time.
Use a min-heap storing end times of active meetings.

For each meeting:
- Pop all meetings that ended before current start (room freed).
- Push current meeting's end time (occupy a room).
- Track maximum heap size.

Heap size = rooms currently used
Max heap size = minimum rooms required

Time: O(n log n)
Space: O(n)
*/
int minMeetingRooms(vector<vector<int>> &arr)
{
    if (arr.size() <= 1)
        return arr.size();
    sort(arr.begin(), arr.end());
    int end = arr[0][1];
    priority_queue<int, vector<int>, greater<int>> q;
    q.push(end);

    int ans = 1;
    for (int i = 1; i < arr.size(); i++)
    {
        int currStart = arr[i][0];
        int currEnd = arr[i][1];

        /*
        CASE-1
            curr meeting starts after prev meet ends
            will remove that from room, and take its place

        CASE-2
            curr meeting starts before quickest prev meetings end
            will take another room, push in heap
        */
        while (!q.empty() && currStart >= q.top())
            q.pop(); // free finished rooms

        q.push(currEnd);
        ans = max(ans, (int)q.size());
    }
    return ans;
}

/*
// !LC 239 - Sliding Window Maximum

Problem:
Given an array nums and window size k, return the maximum element in every
contiguous subarray of size k.

Sample:
nums = [1,3,-1,-3,5,3,6,7], k = 3
windows → max
[1,3,-1] → 3
[3,-1,-3] → 3
[-1,-3,5] → 5
[-3,5,3] → 5
[5,3,6] → 6
[3,6,7] → 7

Intuition:
Key Idea (LC 239):
If a number is smaller than a new incoming number,
it can never become the maximum later.
So you can remove such smaller elements.

Use a deque (double-ended queue) to store indices of useful elements
in decreasing order of values.

Discard smaller elements and keep only "useful candidates"
for maximum in a decreasing deque.

Thus the deque stores only elements that still have a chance
to be the maximum of the current or future windows.

Use a monotonic decreasing deque storing indices.
Front always stores index of the largest element in the current window.
Remove elements:
1) From front → if they move outside window
2) From back → if smaller than current element (can't be future max)

Sliding Window Blueprint:
for each i:
    remove elements outside window
    remove smaller elements from back
    add current index
    if window formed → record answer

Time Complexity:  O(n)
(each element pushed once and popped at most once)

Space Complexity: O(k)
(deque holds at most k indices)
*/

vector<int> maxSlidingWindow(vector<int> &nums, int k)
{
    deque<int> q;
    vector<int> res;

    for (int i = 0; i < nums.size(); i++)
    {

        // remove indices outside window
        if (!q.empty() && q.front() <= i - k)
            q.pop_front();

        // maintain decreasing deque
        while (!q.empty() && nums[q.back()] < nums[i])
            q.pop_back();

        q.push_back(i);

        // record answer when window formed
        if (i >= k - 1)
            res.push_back(nums[q.front()]);
    }

    return res;
}
/*
// !LC 238 - Product of Array Except Self

Build prefix product in result array:
res[i] = product of elements before i.

Traverse from right maintaining suffix product
and multiply with res[i].

Each element processed twice.

Time: O(n)
Space: O(1) extra
*/
vector<int> productExceptSelf(vector<int> &nums)
{
    int n = nums.size();
    vector<int> res(n, 1);

    // prefix product
    for (int i = 1; i < n; i++)
    {
        res[i] = nums[i - 1] * res[i - 1];
    }

    // suffix product
    int suffix = nums[n - 1];
    for (int i = n - 2; i >= 0; i--)
    {
        res[i] *= suffix;
        suffix *= nums[i];
    }

    return res;
}

/*
// !LC 1143 - Longest Common Subsequence

If characters match → include them and move both pointers.
If not → skip one character from either string.

dp[i][j] = LCS length using s1[0..i] and s2[0..j]

Time: O(n*m)
Space: O(n*m)
*/
int solve(int i, int j, string &s1, string &s2, vector<vector<int>> &dp)
{
    if (i < 0 || j < 0)
        return 0;
    if (dp[i][j] != -1)
        return dp[i][j];
    // match then common subsq len increase
    if (s1[i] == s2[j])
        return dp[i][j] = 1 + solve(i - 1, j - 1, s1, s2, dp);

    //              i     j
    // case 1 :  abcd abcde (do j-1)

    // case 2 :  pqrst pqrs (do i-1)
    //               i    j

    return dp[i][j] = max(solve(i - 1, j, s1, s2, dp), solve(i, j - 1, s1, s2, dp));
}
int longestCommonSubsequence(string text1, string text2)
{
    int i = text1.size() - 1;
    int j = text2.size() - 1;
    vector<vector<int>> dp(i + 1, vector<int>(j + 1, -1));
    return solve(i, j, text1, text2, dp);
}

/*
// !LC 739 - Daily Temperatures

Problem:
Given an array temperatures where temperatures[i] is the temperature on day i,
return an array answer such that answer[i] is the number of days until a warmer
temperature. If no future day exists, answer[i] = 0.

Sample:
temps = [73,74,75,71,69,72,76,73]

Output:
[1,1,4,2,1,1,0,0]


Idea:
Use a monotonic decreasing stack storing indices of days.
Stack keeps days whose next warmer temperature is not found yet.

When a warmer temperature appears:
    pop all smaller temps from stack
    answer = current_index - popped_index

Each index pushed once and popped once.
Time Complexity:  O(n)
Space Complexity: O(n)
*/
vector<int> dailyTemperatures(vector<int> &temp)
{
    // next greater after what index
    stack<int> st;
    vector<int> res(temp.size(), 0);
    for (int i = 0; i < temp.size(); i++)
    {
        while (!st.empty() && temp[i] > temp[st.top()])
        {
            int idx = st.top();
            st.pop();
            res[idx] = i - idx;
        }
        st.push(i);
    }
    return res;
}
/*
// !LC 79 - Word Search

Problem:
Given a grid of characters and a word, check if the word can be formed
by sequentially adjacent cells (up, down, left, right).
A cell can be used only once in a path.

Sample:
board = [
["A","B","C","E"],
["S","F","C","S"],
["A","D","E","E"]
]

word = "ABCCED"

Path:
(0,0)A → (0,1)B → (0,2)C → (1,2)C → (2,2)E → (2,1)D

----------------------------------------------------

Approach (DFS + Backtracking):

1. Iterate over every cell in the board.
   If board[i][j] == word[0], start DFS from that cell.

2. DFS(r, c, k):
   r,c = current cell
   k   = index of next character to match in word

3. Base case:
   If k == word.size() → all characters matched → return true.

4. From the current cell explore 4 directions:
      up, down, left, right

5. For each neighbor:
   - must be inside board
   - must match word[k]
   - must not be visited

6. Mark the current cell as visited (e.g. replace with '#'),
   explore neighbors recursively.

7. After recursion, restore the cell (backtracking).

8. If any DFS path succeeds → return true.

----------------------------------------------------

Key Idea:
Treat the grid like a graph and try building the word character by
character using DFS while preventing reuse of the same cell.

----------------------------------------------------

Time Complexity:
O(m * n * 4^L)

m*n → possible starting cells
4^L → DFS branching (L = word length)

Space Complexity:
O(L) recursion stack
*/
int dir[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
bool solve(vector<vector<char>> &board, string &word, int r, int c, int k, int m, int n)
{
    // cout<<curr<<" ";
    // if(curr == word) return true;
    if (k == word.size())
        return true;
    for (auto &d : dir)
    {
        int nr = r + d[0];
        int nc = c + d[1];
        if (nr < m && nc < n && nr >= 0 && nc >= 0 && board[nr][nc] != '#' && board[nr][nc] == word[k])
        {
            // curr.push_back(board[nr][nc]);
            char temp = board[nr][nc];
            board[nr][nc] = '#';
            if (solve(board, word, nr, nc, k + 1, m, n))
                return true;
            board[nr][nc] = temp;
            // curr.pop_back();
        }
    }
    return false;
}
bool exist(vector<vector<char>> &board, string word)
{
    int m = board.size();
    int n = board[0].size();
    if (word.size() > m * n)
        return false;
    // vector<vector<bool>>vis(m,vector<bool>(n,false));
    int k = 0;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (board[i][j] != word[k])
                continue;
            // string curr = "";
            // curr += board[i][j];
            char temp = board[i][j];
            board[i][j] = '#';
            if (solve(board, word, i, j, k + 1, m, n))
                return true;
            board[i][j] = temp;
        }
    }
    return false;
}

/*
// !Trie (Prefix Tree)

Each node stores 26 children and end-of-word flag.

Insert:
create nodes if missing while traversing characters.

Search:
follow path and check end flag.

StartsWith:
check if prefix path exists.

Insert      : O(L)
Search      : O(L)
StartsWith  : O(L)

Space: O(total characters inserted) when inserting
*/

class Trie
{
    struct TrieNode
    {
        TrieNode *links[26] = {nullptr}; // array of pointers to TrieNode objects
        bool isEnd = false;

        bool contains(char c)
        {
            return links[c - 'a'] != nullptr;
        }
        void put(char c)
        {
            links[c - 'a'] = new TrieNode();
        }
        TrieNode *get(char c)
        {
            return links[c - 'a'];
        }
        void setEnd()
        {
            isEnd = true;
        }
        bool hasEnded()
        {
            return isEnd == true;
        }
    };
    TrieNode *root;

public:
    Trie()
    {
        root = new TrieNode();
    }

    void insert(string word)
    {
        TrieNode *temp = root;
        for (int i = 0; i < word.size(); i++)
        {
            char c = word[i];
            if (!temp->contains(c))
                temp->put(c);
            temp = temp->get(c);
        }
        temp->setEnd();
    }

    bool search(string word)
    {
        TrieNode *temp = root;
        for (int i = 0; i < word.size(); i++)
        {
            char c = word[i];
            if (temp->contains(c))
                temp = temp->get(c);
            else
                return false;
        }
        return temp->hasEnded();
    }

    bool startsWith(string prefix)
    {
        TrieNode *temp = root;
        for (int i = 0; i < prefix.size(); i++)
        {
            char c = prefix[i];
            if (temp->contains(c))
                temp = temp->get(c);
            else
                return false;
        }
        return true;
    }
};

/*
// !LC 211 - Add and Search Word

Trie with wildcard search.

Insert:
Normal Trie insertion.

Search:
DFS through Trie.
If character is '.', try all children.

Time:
O(L) normal
O(26^L) worst case with wildcards
*/
class WordDictionary
{

public:
    struct TrieNode
    {
        TrieNode *links[26] = {nullptr};
        bool end = false;

        void setEnd() { end = true; }
        bool hasEnded() { return end; }
        TrieNode *get(char c) { return links[c - 'a']; }
        bool contains(char c) { return links[c - 'a'] != nullptr; }
        void put(char c) { links[c - 'a'] = new TrieNode(); }
    };
    TrieNode *root;
    WordDictionary() { root = new TrieNode(); }

    void addWord(string word)
    {
        TrieNode *temp = root;
        for (auto c : word)
        {
            if (!temp->contains(c))
                temp->put(c);
            temp = temp->get(c);
        }
        temp->setEnd();
    }
    bool dfs(string &word, int i, TrieNode *node)
    {
        if (i == word.size())
        {
            return node->hasEnded();
        }
        char c = word[i];
        if (c == '.')
        {
            for (char t = 'a'; t <= 'z'; t++)
            {
                if (node->contains(t) && dfs(word, i + 1, node->get(t)))
                    return true;
            }
        }
        else
        {
            if (!node->contains(c))
                return false;
            return dfs(word, i + 1, node->get(c));
        }
        return false;
    }
    bool search(string word) { return dfs(word, 0, root); }
};

/*
// !LeetCode 212 — Word Search II
Approach: Trie + DFS Backtracking + Trie Pruning Optimization

---------------------------------------------------------------
PROBLEM
Given a board of characters and a list of words, return all
words that can be formed by sequentially adjacent cells
(up, down, left, right). A cell cannot be reused in a word.

---------------------------------------------------------------
INTUITION

Naive:
Run Word Search (LC79) for every word → too slow.

Better idea:
Store all words in a Trie so common prefixes are shared.

DFS from each board cell:
- follow Trie edges while moving in board
- stop early if Trie prefix doesn't exist
- when Trie node contains a word → add to result

---------------------------------------------------------------
TRIE STRUCTURE

Each TrieNode stores:
- links[26] → children
- word → stores complete word at terminal node

Why store word?
So we can directly push the word when we reach that node
without rebuilding the string.

---------------------------------------------------------------
DFS LOGIC

1. Start DFS only if root contains board[i][j]
2. Mark cell visited using '#'
3. Move in 4 directions
4. Continue only if Trie contains next character
5. Backtrack by restoring board cell

---------------------------------------------------------------
DUPLICATE PREVENTION

After finding a word:

    res.push_back(node->word);
    node->word = "";

This ensures the same word isn't added again.

---------------------------------------------------------------
TRIE PRUNING OPTIMIZATION

Observation:
If a Trie node has:
    node->word == ""
AND
    no children

Then no remaining word uses this prefix.

So we delete the branch from parent:

    parent->links[currChar-'a'] = nullptr;

---------------------------------------------------------------
EXAMPLE

Words = ["oath","eat"]

Trie:

root
 ├─ o → a → t → h (word="oath")
 └─ e → a → t     (word="eat")

DFS finds "oath":

1. push "oath"
2. node->word = ""

Node 'h' has:
- no children
- no word

Delete branch:

t->links['h'] = nullptr

Pruning continues upward if nodes become childless.

Final Trie:

root
 └─ e → a → t (word="eat")

Future DFS starting from 'o' is skipped immediately.

---------------------------------------------------------------
TIME & SPACE COMPLEXITY (CLEAR VERSION)

Definitions:
M = number of rows in board
N = number of columns in board
W = number of words
L = maximum length of a word

------------------------------------------------

BUILD TRIE

Each word inserts L characters.

Time:
O(W * L)

Space:
O(W * L)
(worst case when no prefixes overlap)

------------------------------------------------

DFS SEARCH

We start DFS from every board cell.

Total starts:
M * N

From each cell we explore at most 4 directions.

Depth of DFS = at most L
(max word length)

Worst case branching:
4 * 3^(L-1)

Explanation:
First step → 4 directions
Next steps → cannot go back to previous cell
so only 3 choices remain.

Worst case DFS per cell:

O(4 * 3^(L-1)) ≈ O(3^L)

Total worst-case DFS:

O(M * N * 3^L)

------------------------------------------------

BUT IN PRACTICE

Trie prefix checking stops DFS early.

If board prefix doesn't exist in Trie:
DFS stops immediately.

Trie pruning optimization further reduces work
because used prefixes are removed.

So real runtime is much smaller.

------------------------------------------------

FINAL COMPLEXITY

Time:
Trie build  → O(W * L)
DFS search → O(M * N * 3^L)

Space:
Trie → O(W * L)
DFS stack → O(L)
---------------------------------------------------------------
KEY INTERVIEW TALKING POINTS

Trie → avoids repeating prefix checks
DFS → explore board paths
Backtracking → mark '#' to prevent reuse
Store word in Trie node
Clear word to avoid duplicates
Prune Trie branches when useless

This pruning significantly speeds up LeetCode runtime.
*/

class Trie
{
public:
    struct TrieNode
    {
        TrieNode *links[26] = {nullptr};
        string word = "";

        bool contains(char c) { return links[c - 'a'] != nullptr; }
        TrieNode *get(char c) { return links[c - 'a']; }
        void put(char c) { links[c - 'a'] = new TrieNode(); }

        // >OPTIMIZATION and EXAMPLE
        // Remove Trie branches when:
        // node has no children AND node->word == ""
        // preventing future DFS from exploring already exhausted prefixes

        // >EXAMPLE
        // eg trie "oath", "eat"
        // once a word (o->a->t->h) is found and stored node->word = ""
        // and it also has no links/child
        // once "o->a->t->h" found, and 'h' has no more links
        // it means no remaining words use this prefix
        // so we can delete that branch( t->h ) from the Trie.
        // deleting means t->links[h-'a'] = nullptr;
        // for dfs o->a->t->h,
        // first i j will be at h, that is delted
        // then when recusion comes back it deletes t,o,a
        bool hasChildren()
        {
            for (int i = 0; i < 26; i++)
                if (links[i])
                    return true;
            return false;
        }
    };

    TrieNode *root;

    Trie() { root = new TrieNode(); }

    void addWord(string word)
    {
        TrieNode *node = root;
        for (char c : word)
        {
            if (!node->contains(c))
                node->put(c);
            node = node->get(c);
        }
        node->word = word;
    }
};

class Solution
{
public:
    int dir[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    // void dfs(vector<vector<char>>& board, int r, int c, Trie::TrieNode* node, vector<string>& res){
    void dfs(vector<vector<char>> &board, int r, int c, Trie::TrieNode *node, Trie::TrieNode *parentNode, char currChar, vector<string> &res)
    {

        if (node->word != "")
        {
            res.push_back(node->word);
            node->word = ""; // avoid duplicates
        }

        char temp = board[r][c];
        board[r][c] = '#';

        int m = board.size();
        int n = board[0].size();

        for (auto &d : dir)
        {
            int nr = r + d[0];
            int nc = c + d[1];

            if (nr >= 0 && nc >= 0 && nr < m && nc < n && board[nr][nc] != '#')
            {
                char next = board[nr][nc];

                if (node->contains(next))
                {
                    dfs(board, nr, nc, node->get(next), node, next, res);
                }
            }
        }

        board[r][c] = temp;
        // Pruning
        if (!node->hasChildren() && node->word == "")
        {
            parentNode->links[currChar - 'a'] = nullptr;
        }
    }

    vector<string> findWords(vector<vector<char>> &board, vector<string> &words)
    {

        Trie trie;

        for (string &w : words)
            trie.addWord(w);

        int m = board.size();
        int n = board[0].size();

        vector<string> res;

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {

                char c = board[i][j];

                if (trie.root->contains(c))
                {
                    // dfs(board, i, j, trie.root->get(c), res);
                    dfs(board, i, j, trie.root->get(c), trie.root, c, res);
                }
            }
        }

        return res;
    }
};

// !For LeetCode 202 — Happy Number
/*

!two solutions: without using extra space
 n ≤ 2^31 − 1 → at most 10 digits
 Each iteration:
 compute sum of squares of digits
 work per iteration = O(10) ≈ O(1)

 After first step:
 max possible value = 10 * 9^2 = 810

 So all future numbers are in range [1..810]

 Therefore the sequence can only visit
 at most ~810 different values before
 either reaching 1 or forming a cycle.
 Let k = number of different sums seen before cycle or reaching 1

 Time Complexity:
 O(k) or O(810)
 Each step computes sum of squares of digits
 digits ≤ 10 (since n ≤ 2^31−1), so constant work per step

 Space Complexity:
 O(k)
 unordered_set stores all previously seen sums to detect cycle

 n ≤ 2^31 − 1 → at most 10 digits
 Each iteration:
 compute sum of squares of digits
 work per iteration = O(10) ≈ O(1)

 After first step:
 max possible value = 10 * 9^2 = 810

 So all future numbers are in range [1..810]

 Therefore the sequence can only visit
 at most ~810 different values before
 either reaching 1 or forming a cycle.

*/

bool solve(int num, unordered_set<int> &sums)
{
    // find sum of squares
    int currSum = 0;
    while (num != 0)
    {
        int dig = num % 10;
        num /= 10;
        currSum += dig * dig;
    }
    if (sums.count(currSum))
        return false;
    else if (currSum == 1)
        return true;
    else
    {
        sums.insert(currSum);
        return solve(currSum, sums);
    }
}
bool isHappy(int n)
{
    // find sum of squares of digits
    // if sum already achieved, cycle
    // if sum==1 true
    unordered_set<int> sums;
    return solve(n, sums);
}

int sumOfSquares(int num)
{
    // find sum of squares
    int currSum = 0;
    while (num != 0)
    {
        int dig = num % 10;
        num /= 10;
        currSum += dig * dig;
    }
    return currSum;
}
bool isHappy(int n)
{
    if (sumOfSquares(n) == 1)
        return true;
    int slow = sumOfSquares(n);
    int fast = sumOfSquares(sumOfSquares(n));

    while (slow != fast)
    {
        if (slow == 1 || fast == 1)
            return true;
        slow = sumOfSquares(slow);
        fast = sumOfSquares(sumOfSquares(fast));
    }
    return false;
}

/*
// !Max Area of Island (LC 695)

DFS from each cell with value 1.
Mark visited by setting grid[r][c] = 0.

Island area = 1 (current cell) + DFS of all 4 neighbors.

Track maximum area across all islands.

Time: O(m*n)  -> each cell visited once
Space: O(m*n) -> recursion stack worst case
*/
int dir[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
int dfs(vector<vector<int>> &grid, int r, int c)
{
    grid[r][c] = 0;
    int ans = 1;
    for (int i = 0; i < 4; i++)
    {
        int nr = r + dir[i][0];
        int nc = c + dir[i][1];
        if (nr >= 0 && nc >= 0 && nr < grid.size() && nc < grid[0].size() && grid[nr][nc] != 0)
        {
            ans += dfs(grid, nr, nc);
        }
    }
    return ans;
}
int maxAreaOfIsland(vector<vector<int>> &grid)
{
    int ans = 0;
    int m = grid.size();
    int n = grid[0].size();
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 1)
            {
                int size = dfs(grid, i, j);
                ans = max(ans, size);
            }
        }
    }
    return ans;
}

/*
// !LC 684 Redundant Connection

Graph was originally a tree (n nodes, n-1 edges).
One extra edge added → exactly one cycle.

Use Union-Find:
For each edge (u,v):
    if find(u) == find(v)
        → already connected → this edge forms cycle → return it
    else
        union(u,v)

Optimizations:
- Path Compression (find)
- Union by Rank

findParent → almost O(1) (amortized)
union → almost O(1)

TC: O(n α(n)) ≈ O(n)
SC: O(n)
*/

class DSU
{
    vector<int> parent;
    vector<int> rank;

public:
    DSU(int n)
    {
        rank.assign(n + 1, 0);
        parent.resize(n + 1);
        for (int i = 1; i <= n; i++)
            parent[i] = i;
    }

    int findParent(int node)
    {
        if (parent[node] == node)
            return node;
        else
            return parent[node] = findParent(parent[node]);
    }

    void unify(int node1, int node2)
    {
        int p1 = findParent(node1);
        int p2 = findParent(node2);

        if (p1 == p2)
            return;

        if (rank[p1] > rank[p2])
        {
            parent[p2] = p1;
        }
        else if (rank[p2] > rank[p1])
        {
            parent[p1] = p2;
        }
        else
        {
            parent[p1] = p2;
            rank[p2]++;
        }
    }
};

class Solution
{
public:
    vector<int> findRedundantConnection(vector<vector<int>> &edges)
    {
        DSU d(edges.size() + 1);
        for (auto &e : edges)
        {
            int node1 = e[0];
            int node2 = e[1];

            if (d.findParent(node1) != d.findParent(node2))
            {
                d.unify(node1, node2);
            }
            else
            {
                return {node1, node2};
            }
        }
        return {};
    }
};

/*
// !LC 1584 Min Cost to Connect Points

Use Prim's algorithm for MST.

Start from node 0.
Always pick the smallest edge connecting a new node to MST.

Steps:
1. Use min heap storing (weight, node)
2. Pop smallest edge
3. If node not visited:
       add to MST
       add cost
4. Push edges from this node to all other nodes

Edge weight = Manhattan distance.

TC: O(n^2 log n)
SC: O(n^2)
*/

int minCostConnectPoints(vector<vector<int>> &points)
{
    // no of points == nodes
    int n = points.size();
    vector<int> vis(n, 0);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q; // wt,node
    q.push({0, 0});
    int cost = 0;
    while (!q.empty())
    {
        auto [wt, node] = q.top();
        q.pop();
        if (vis[node])
            continue;
        vis[node] = 1;
        cost += wt;
        for (int i = 0; i < n; i++)
        {
            if (i == node || vis[i] == 1)
                continue;
            //  |xi - xj| + |yi - yj|
            int currWt = abs(points[node][0] - points[i][0]) + abs(points[node][1] - points[i][1]);
            q.push({currWt, i});
        }
    }
    return cost;
}

/*
// !678. Valid Parenthesis String
Given a string s containing only three types of characters:
'(', ')' and '*', return true if s is valid.


DP state:
(i, open) → index and number of unmatched '('

Transitions:
'(' → open+1
')' → open-1
'*' → treat as '(', ')' or empty

Valid if at end open == 0.

// ! Greedy O(n) solution possible

TC: O(n²)
SC: O(n²)
*/

bool solve(int i, int open, string &s, vector<vector<int>> &dp)
{
    if (open < 0)
        return false;
    if (i == s.size())
        return open == 0;
    if (dp[i][open] != -1)
        return dp[i][open];
    if (s[i] == '(')
        return dp[i][open] = solve(i + 1, open + 1, s, dp);
    else if (s[i] == ')')
        return dp[i][open] = solve(i + 1, open - 1, s, dp);
    else
    {
        return dp[i][open] = solve(i + 1, open + 1, s, dp) ||
                             solve(i + 1, open - 1, s, dp) ||
                             solve(i + 1, open, s, dp);
    }
}
bool checkValidString(string s)
{
    int n = s.size();
    // vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
    // return solve(0, 0, s, dp);

    /*
    Because '*' can be '(', ')' or empty, we cannot track a single open
    count. Instead we track a range of possible opens: [low, high].
    '(' increases both, ')' decreases both, '*' expands the range.
    If high < 0 → too many ')'. At the end low must be 0 to close all '('.

    possible open count
    [low ........ high]
    */
    int low = 0;
    int high = 0;
    for (int i = 0; i < n; i++)
    {
        if (s[i] == ')')
        {
            low--;
            high--;
        }
        else if (s[i] == '(')
        {
            low++;
            high++;
        }
        else
        {
            low--;  // '*' as ')'
            high++; // '*' as '('
        }
        if (high < 0)
            return false;

        // we assumed '*' acted like ')'
        // but there was no '(' to close
        // So we discard that possibility and treat * as ""  (empty)
        low = max(low, 0);
    }
    return low == 0;
}

/*
// !LC 53 Find the maximum sum of a contiguous subarray and print the subarray.
Test Case: [5, 4, -10, 1]
OP: 9
* * INTUITION:
* We iterate through the array, adding elements to a running 'sum'.
* 1. If 'sum' exceeds our 'mx', we update the max and "lock in" the
* current start and end boundaries.
* 2. If 'sum' drops below 0, it will only decrease the value of any future
* subarray. So, we reset 'sum' to 0 and move our 'potential_start' to the next index.

* * TIME COMPLEXITY: O(n) - Single pass through the array.
* SPACE COMPLEXITY: O(1) - Only a few extra variables for tracking.
*/

int maxSubArray(vector<int> &nums)
{
    int mx = INT_MIN, sum = 0;
    int start = 0, end = 0, temp_start = 0;

    for (int i = 0; i < nums.size(); i++)
    {
        sum += nums[i];

        if (sum > mx)
        {
            mx = sum;
            start = temp_start; // Sync the actual start
            end = i;            // Sync the actual end
        }

        if (sum < 0)
        {
            sum = 0;
            temp_start = i + 1; // Potential new start
        }
    }

    // Print the result subarray
    for (int i = start; i <= end; i++)
        cout << nums[i] << " ";

    return mx;
}

/*
// !PROBLEM: 152. Maximum Product Subarray
TC: O(n) | SC: O(1)
INTUITION:
Track both max and min products ending at the current index.
When encountering a negative number, swap max and min.
At each step, decide to either "restart" at nums[i] or "continue" the product.

SAMPLE TC: nums = [2, 3, -2, 4]
i=0: max=2, min=2, res=2
i=1: max=6, min=3, res=6
i=2: swap(6, 3)->(3, 6) -> max=max(-2, 3*-2)=-2, min=min(-2, 6*-2)=-12, res=6
i=3: max=max(4, -2*4)=4, min=min(4, -12*4)=-48, res=6
*/
int maxProduct(vector<int> &nums)
{
    // Initializing with first element handles the single-element array edge case.
    int res = nums[0];
    int maxprod = nums[0];
    int minprod = nums[0];

    for (int i = 1; i < nums.size(); i++)
    {
        /* * INTUITION: Multiplying by a negative number flips the signs.
         * A huge positive becomes a huge negative (the new min).
         * A huge negative becomes a huge positive (the new max).
         * Swapping them before calculation accounts for this flip.
         */
        if (nums[i] < 0)
            swap(minprod, maxprod);

        /*
         * DECISION POINT: At each index, we decide:
         * 1. Continue the current product (nums[i] * maxprod)
         * 2. Or start a fresh subarray at the current element (nums[i])
         * This handles '0's naturally by resetting the product chain.
         */
        maxprod = max(nums[i], nums[i] * maxprod);
        minprod = min(nums[i], nums[i] * minprod);

        // Keep track of the highest value seen ending at ANY index.
        res = max(res, maxprod);
    }

    return res;
    // !to track start and end
    /*
    INTUITION:
    We track the start index for BOTH maxprod and minprod.
    When we swap the products, we also swap their corresponding start indices.
    If a product "restarts" at nums[i], we update its start index to i.
    */

    int n = nums.size();
    int res = nums[0], maxprod = nums[0], minprod = nums[0];
    int s = 0, e = 0;
    int maxStart = 0, minStart = 0; // Track starts for both states

    for (int i = 1; i < n; i++)
    {
        if (nums[i] < 0)
        {
            swap(maxprod, minprod);
            swap(maxStart, minStart); // Swap the starting points too!
        }

        // Update maxprod and its start point
        if (nums[i] > nums[i] * maxprod)
        {
            maxprod = nums[i];
            maxStart = i; // Restarted here
        }
        else
        {
            maxprod = nums[i] * maxprod;
        }

        // Update minprod and its start point
        if (nums[i] < nums[i] * minprod)
        {
            minprod = nums[i];
            minStart = i; // Restarted here
        }
        else
        {
            minprod = nums[i] * minprod;
        }

        if (maxprod > res)
        {
            res = maxprod;
            s = maxStart;
            e = i;
        }
    }

    // Print the result subarray
    for (int i = s; i <= e; i++)
        cout << nums[i] << " ";
    return res;
}
/* PROBLEM: 152. Maximum Product Subarray (Two-Pass)
TC: O(n) | SC: O(1)
INTUITION:
An even number of negatives makes the whole array positive.
An odd number of negatives means we must exclude one negative to get a positive max.
The max product will either be the prefix (up to the last negative) or
the suffix (from the first negative). Zeros act as boundaries that reset the product.

SAMPLE TC: nums = [3, -2, -3, -2, 4]
Forward Pass: 3, -6, 18, -36, -144 (Max: 18)
Backward Pass: 4, -8, 24, -48, -144 (Max: 24)
Final Result: 24
*/

int maxProduct(vector<int> &nums)
{
    int n = nums.size();
    double prefix = 1, suffix = 1;
    double res = nums[0];

    for (int i = 0; i < n; i++)
    {
        // Reset product to 1 if the previous element was 0
        prefix = (prefix == 0 ? 1 : prefix) * nums[i];
        suffix = (suffix == 0 ? 1 : suffix) * nums[n - 1 - i];

        res = max({res, prefix, suffix});
    }

    return (int)res;
}

/*
// !PROBLEM: 153. Find Minimum in Rotated Sorted Array
TC: O(log n) | SC: O(1)
INTUITION:
Compare mid with the RIGHTmost element (r).
The goal is to find the "inflection point" where the numbers drop.

Case 1: nums[mid] > nums[r]
The right side is "broken." The minimum MUST be to the right of mid.
Action: l = mid + 1 (mid is too large to be the min).

Case 2: nums[mid] <= nums[r]
The right side is sorted. The minimum is either mid itself or to its left.
Action: r = mid (keep mid as a candidate).

CASE 1: mid is on the High Slope (x is between l and Pivot)
Condition: nums[mid] > nums[r]
Visual: l ---- x ---- [Pivot] ---- r
Action: l = mid + 1 (Min is to the right of x)

CASE 2: mid is on the Low Slope (x is between Pivot and r)
Condition: nums[mid] <= nums[r]
Visual: l ---- [Pivot] ---- x ---- r
Action: r = mid (x could be the Pivot, or Pivot is to the left)

CASE 3: mid is EXACTLY at the Pivot
Condition: nums[mid] <= nums[r] (Specifically, nums[mid] is the smallest)
Visual: l ---- [x] ---- r
Action: r = mid (We keep x as the potential answer)
*/

int findMin(vector<int> &nums)
{
    int l = 0, r = nums.size() - 1;
    while (l < r)
    {
        int mid = l + (r - l) / 2;
        if (nums[mid] > nums[r])
            l = mid + 1; // Case 1
        else
            r = mid; // Case 2 & 3
    }
    return nums[r];
}

/*
// !PROBLEM: 33. Search in Rotated Sorted Array (Two-Pass)
TC: O(log n) | SC: O(1)
INTUITION:
1. Find the Pivot (s2): Use Binary Search to find the smallest element.
2. Determine Range:
   - If target <= nums[n-1], it's on the right slope [s2, n-1].
   - Else, it's on the left slope [0, s2-1].
3. Standard Binary Search: Search only within that identified sorted range.

VISUAL:
    [Slope 1]
      /
     /    | [Slope 2]
   [0]    |    /
       [s2-1][s2]...[n-1]
*/

int search(vector<int> &nums, int target)
{
    int n = nums.size();
    int l = 0, r = n - 1;

    // Pass 1: Find the pivot index (s2)
    while (l < r)
    {
        int mid = l + (r - l) / 2;
        if (nums[mid] > nums[r])
            l = mid + 1;
        else
            r = mid;
    }
    int s2 = l;

    // Decide which sorted half to search
    l = 0;
    r = n - 1;
    if (target <= nums[r])
        l = s2;
    else
        r = s2 - 1;

    // Pass 2: Standard Binary Search on the chosen half
    while (l <= r)
    {
        int mid = l + (r - l) / 2;
        if (nums[mid] == target)
            return mid;
        if (nums[mid] > target)
            r = mid - 1;
        else
            l = mid + 1;
    }
    return -1;
}

/* PROBLEM: 150. Evaluate Reverse Polish Notation (Postfix)
TC: O(N) - Single pass through tokens
SC: O(N) - Stack stores operands

ALGORITHM:
1. If token is a NUMBER: Push to stack.
2. If token is an OPERATOR:
   a. Pop 'right' operand (top of stack).
   b. Pop 'left' operand (next on stack).
   c. Apply operator: (left <op> right).
   d. Push result back to stack.

VISUAL STACK TRACE ["2", "1", "+", "3", "*"]:
1. Push 2: [2]
2. Push 1: [2, 1]
3. Pop 1, Pop 2 -> (2 + 1) -> Push 3: [3]
4. Push 3: [3, 3]
5. Pop 3, Pop 3 -> (3 * 3) -> Push 9: [9]
Result: 9
*/

// Helper: Handles basic arithmetic
int eval(int left, int right, string token)
{
    if (token == "+")
        return left + right;
    if (token == "-")
        return left - right;
    if (token == "*")
        return left * right;
    return left / right; // Division
}

int evalRPN(vector<string> &tokens)
{
    stack<int> st;
    for (const string &token : tokens)
    {
        // Check if token is an operator
        if (token == "+" || token == "-" || token == "*" || token == "/")
        {
            int right = st.top();
            st.pop();
            int left = st.top();
            st.pop();
            st.push(eval(left, right, token));
        }
        else
        {
            // Convert string to integer and push
            st.push(stoi(token));
        }
    }
    return st.top();
}

/**
 * PROBLEM: Reorder List (L0 -> Ln -> L1 -> Ln-1 -> L2 -> Ln-2 -> ...)
 *
 * SAMPLE INPUT:  [1, 2, 3, 4, 5]
 * SAMPLE OUTPUT: [1, 5, 2, 4, 3]
 *
 * COMPLEXITY:
 * - Time Complexity (TC): O(N)
 * One pass to find middle, one to reverse half, one to merge.
 * - Space Complexity (SC): O(1)
 * In-place pointer manipulation; no extra data structures.
 *
 * STRATEGY:
 * 1. Find Middle: Use slow/fast pointers.
 * - Even: [1, 2, 3, 4] -> slow at 2
 * - Odd:  [1, 2, 3, 4, 5] -> slow at 3
 *
 * 2. Reverse Second Half: Flip 'next' pointers from (slow->next) to end.
 * - Initial: [1, 2, 3] and [4, 5]
 * - Reversed: [1, 2, 3] and [5, 4]
 *
 * 3. Split: Crucial to set (slow->next = nullptr) to prevent cycles.
 *
 * 4. Merge: Interleave nodes from the first and reversed second half.
 * - Use temp pointers (tmp1, tmp2) to avoid losing the 'next' node.
 */
ListNode *reverse(ListNode *head)
{
    ListNode *prev = nullptr;
    ListNode *curr = head;
    while (curr)
    {

        ListNode *next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}
void reorderList(ListNode *head)
{
    if (!head || !head->next)
        return;

    // 1. Find Middle
    ListNode *slow = head, *fast = head;
    while (fast->next && fast->next->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 2. Reverse Second Half
    ListNode *second = reverse(slow->next);
    slow->next = nullptr; // Break the link!

    // 3. Merge
    ListNode *first = head;
    while (second)
    {
        ListNode *tmp1 = first->next, *tmp2 = second->next;
        first->next = second;
        second->next = tmp1;
        first = tmp1;
        second = tmp2;
    }
}

/*
//  !PROBLEM: Palindrome Partitioning (LC 131)

 TC (Without DP): O(N * 2^N)
 2^N possible partitions * O(N) check per substring.

 TC (With DP/Memo): O(2^N)
 O(N^2) to precompute/memoize + 2^N partitions * O(1) lookup.

 SC: O(N^2)
 To store the isPalindrome[i][j] memo table.

 LOGIC:
 1. DP: Check if s[i...j] is palindrome via:
    isPal(i, j) = (s[i] == s[j]) && isPal(i+1, j-1)
 2. Backtrack: Try every split point 'j' from 'i'.
    If s[i...j] is valid, recurse for j+1.
 3. Efficiency: Only use s.substr() when pushing to the temp path.
*/
class Solution
{
    // Declare the memo, but don't give it a fixed size here if you want to be safe
    vector<vector<int>> memo;

    bool check(int i, int j, string &s)
    {
        if (i >= j)
            return true;
        if (memo[i][j] != -1)
            return memo[i][j];
        if (s[i] != s[j])
            return memo[i][j] = 0;
        return memo[i][j] = check(i + 1, j - 1, s);
    }

    void solve(int i, string &s, vector<string> &temp, vector<vector<string>> &res)
    {
        if (i == s.size())
        {
            res.push_back(temp);
            return;
        }
        for (int j = i; j < s.size(); j++)
        {
            if (check(i, j, s))
            {
                temp.push_back(s.substr(i, j - i + 1));
                solve(j + 1, s, temp, res);
                temp.pop_back();
            }
        }
    }

public:
    vector<vector<string>> partition(string s)
    {
        int n = s.size();
        // RESET/RESIZE memo for every new test case
        memo.assign(n, vector<int>(n, -1));

        vector<string> temp;
        vector<vector<string>> res;
        solve(0, s, temp, res);
        return res;
    }
};
/*
// !LC 647: Palindromic Substrings (Expand Around Center)

 TC: O(N^2)
 We iterate through 2N-1 centers (N chars + N-1 gaps).
 Each expansion takes O(N) in the worst case (e.g., "aaaaa").

 SC: O(1)
 No DP table or recursion stack; only constant extra variables.

 LOGIC:
 1. Every char (odd) and every gap between chars (even) is a potential center.
 2. Expand outward from each center as long as s[left] == s[right].
 3. Each successful match represents one unique palindromic substring.
*/

int expand(string &s, int i, int j)
{
    int count = 0;
    while (i >= 0 && j < s.size() && s[i] == s[j])
    {
        count++;
        i--;
        j++;
    }
    return count;
}
int countSubstrings(string s)
{
    int count = 0;
    int n = s.size();
    // memo.assign(n,vector<int>(n,-1));
    // for(int i=0;i<n;i++){
    //     for(int j=i;j<n;j++){
    //         if(check(i,j,s))count++;
    //     }
    // }
    for (int i = 0; i < n; i++)
    {
        count += expand(s, i, i);
        count += expand(s, i, i + 1);
    }
    return count;
}

/**
// ! LRU Cache - Optimal Design (DLL + HashMap)
 *
 * Data Structures:
 * - list<pair<int,int>> → maintains order (MRU → front, LRU → back)
 * - unordered_map<int, iterator> → O(1) access to nodes
 *
 * Key Operations:
 * 1. get(key):
 *    - If not found → return -1
 *    - Move node to front (most recently used)
 *
 * 2. put(key, value):
 *    - If key exists → remove old node
 *    - Insert new node at front
 *    - If size > capacity → remove from back (LRU)
 *
 * Important Functions:
 * - dll.splice() → move node in O(1)
        * dll.splice(position, source_list, it)
        * position     → where to insert (iterator in destination list)
        * source_list  → list from which node is taken (same list here)
        * it           → iterator pointing to node to move
        *
        * Example:
        * dll.splice(dll.begin(), dll, it);
        * → moves node at 'it' to front of dll in O(1)
 * - dll.push_front() → insert MRU
 * - dll.pop_back() → remove LRU
 *
 * Complexity:
 * - Time: O(1) for both get and put
 * - Space: O(capacity)
 *
 * Why this works:
 * - HashMap gives direct access to nodes
 * - DLL maintains usage order
 * - No duplicates → strict O(1)
 */
#include <bits/stdc++.h>
using namespace std;

class LRUCache
{
    int capacity;

    // Doubly Linked List → stores {key, value}
    // Front = Most Recently Used (MRU)
    // Back  = Least Recently Used (LRU)
    list<pair<int, int>> dll;

    // HashMap → key -> iterator to node in DLL
    unordered_map<int, list<pair<int, int>>::iterator> m;

public:
    LRUCache(int capacity)
    {
        this->capacity = capacity;
    }

    int get(int key)
    {
        // If key not present → return -1
        auto it = m.find(key);
        if (it == m.end())
            return -1;

        // Get node from DLL
        auto node = it->second;
        int value = node->second;

        // Move accessed node to front (MRU)
        dll.splice(dll.begin(), dll, node);

        // Update iterator in map (good practice)
        m[key] = dll.begin();

        return value;
    }

    void put(int key, int value)
    {
        // Edge case: capacity = 0
        if (capacity == 0)
            return;

        auto it = m.find(key);

        // If key already exists → remove old node
        if (it != m.end())
        {
            dll.erase(it->second);
            m.erase(it);
        }

        // Insert new node at front (MRU)
        dll.push_front({key, value});
        m.emplace(key, dll.begin());

        // If capacity exceeded → remove LRU (back)
        if (m.size() > capacity)
        {
            auto [k, v] = dll.back();
            dll.pop_back();
            m.erase(k);
        }
    }
};
/*
// !LC 139: Word Break (Trie + Memoization)

 TC: O(N^2 + W*L)
 - W*L to build the Trie (W = number of words, L = avg length).
 - N^2 for the DP: There are N states in 'memo', and for each state,
   we potentially iterate through the remaining string (N).

 SC: O(W*L + N)
 - W*L space to store the Trie nodes.
 - O(N) for the memoization table and recursion stack.

 LOGIC:
 1. Insert all dictionary words into a Trie for O(WordLen) prefix matching.
 2. Use recursion with memoization to check if s[i...n] can be partitioned.
 3. At each index 'i', walk down the Trie. Whenever we hit an 'isEnd' node:
    - It means s[i...j] is a valid word.
    - Recurse on the remaining suffix solve(j + 1).
 4. Use memo[i] to skip re-calculating suffixes we've already checked.
*/
// class Trie
// {
// public:
//     struct TrieNode
//     {
//         TrieNode *links[26] = {nullptr};
//         bool isEnd = false;

//         bool contains(char c) { return links[c - 'a'] != nullptr; }
//         void put(char c) { links[c - 'a'] = new TrieNode(); }
//         TrieNode *get(char c) { return links[c - 'a']; }
//         void setEnd() { isEnd = true; }
//         bool hasEnded() { return isEnd; }
//     };

//     TrieNode *root;
//     Trie() { root = new TrieNode(); }

//     void addWord(const string &word)
//     {
//         TrieNode *node = root;
//         for (char c : word)
//         {
//             if (!node->contains(c))
//                 node->put(c);
//             node = node->get(c);
//         }
//         node->setEnd();
//     }

//     TrieNode *getRoot() { return root; }
// };

class Solution
{
public:
    bool solve(int i, string &s, Trie::TrieNode *root, vector<int> &memo)
    {
        // Base case: reached the end of the string
        if (i == s.size())
            return true;

        // Return cached result (0 = false, 1 = true)
        if (memo[i] != -1)
            return memo[i];

        Trie::TrieNode *curr = root;
        for (int j = i; j < s.size(); j++)
        {
            // If the character isn't in the Trie, no more words can start with this prefix
            if (!curr->contains(s[j]))
                break;

            curr = curr->get(s[j]);

            // If we find a valid word, try to solve for the remaining suffix
            // if (curr->hasEnded()) // commenting for vs code error same trie class twice
            {
                if (solve(j + 1, s, root, memo))
                {
                    return memo[i] = 1;
                }
            }
        }

        return memo[i] = 0;
    }

    bool wordBreak(string s, vector<string> &wordDict)
    {
        Trie myTrie;
        for (const string &word : wordDict)
        {
            myTrie.addWord(word);
        }

        // Using s.size() for the memo table size
        vector<int> memo(s.size(), -1);
        // return solve(0, s, myTrie.getRoot(), memo);
    }
};

/*
// !CLONE GRAPH — NOTES

Optimal Approach (BFS/DFS):
- Use unordered_map<Node*, Node*> → original → clone
- Traverse graph once (BFS/DFS)
- For each node:
    - if neighbor not in map → create clone + push to queue
    - always connect: clone[curr]->neighbors.push_back(clone[nei])

Key Ideas:
- Map acts as both → visited + cloned
- Clone graph "on the fly" (no need to rebuild)

Why not use val?
- Node values may not be unique
- Always use Node* as identity

Complexity:
- Time: O(V + E)
- Space: O(V)

Common Mistakes:
- Forgetting to connect edges after creating node
- Using val instead of pointer
- Doing 2-pass (build adj + rebuild graph) → unnecessary
*/
class Node
{
public:
    int val;
    vector<Node *> neighbors;
    Node()
    {
        val = 0;
        neighbors = vector<Node *>();
    }
    Node(int _val)
    {
        val = _val;
        neighbors = vector<Node *>();
    }
    Node(int _val, vector<Node *> _neighbors)
    {
        val = _val;
        neighbors = _neighbors;
    }
};
Node *cloneGraph(Node *node)
{
    if (!node)
        return nullptr;
    unordered_map<Node *, Node *> m; // clone and visited
    queue<Node *> q;
    q.push(node);
    m[node] = new Node(node->val);
    while (!q.empty())
    {
        Node *curr = q.front();
        q.pop();
        for (auto nei : curr->neighbors)
        {
            if (!m.count(nei))
            { // means not visited
                m[nei] = new Node(nei->val);
                q.push(nei);
            }
            m[curr]->neighbors.push_back(m[nei]);
        }
    }
    return m[node];
}

/*
Problem: 128. Longest Consecutive Sequence
Time Complexity: O(n) (Amortized)
- O(n) to build the hash set for O(1) lookups.
- O(n) for the outer loop to identify sequence "starters".
- O(n) total across all 'while' loops because each number in a
trail is visited exactly once in the entire program.
- Total: O(n) + O(n) + O(n) = O(3n) -> O(n).

Only start from numbers which are sequence START
i.e., num-1 does NOT exist
This avoids re-counting sequences again and again

Space Complexity: O(n)
- Required to store all unique elements in the unordered_set.
Efficiency Example:
- Given a trail [1, 2, 3, 4]:
- When i = 1: (1-1=0) is not in set. While loop runs for 2, 3, 4.
- When i = 2: (2-1=1) IS in set. Skip while loop.
- When i = 3: (3-1=2) IS in set. Skip while loop.
- When i = 4: (4-1=3) IS in set. Skip while loop.
- Result: The entire trail is iterated only once by the 'starter' (1).
- All trails in nums are effectively processed once by the while loop.
*/

int longestConsecutive(vector<int> &nums)
{
    unordered_set<int> s;
    for (int num : nums)
        s.insert(num);

    int res = 0;
    for (int num : s)
    {
        if (!s.count(num - 1))
        {
            int currTrail = 1;
            int curr = num;
            while (s.count(curr + 1))
            {
                currTrail++;
                curr = curr + 1;
            }
            res = max(res, currTrail);
        }
    }
    return res;
}
/**
// !GAS STATION (LC 134) - GREEDY
 *
 * Key Ideas:
 * 1. If total gas < total cost → impossible → return -1
 * 2. Traverse once, keep current tank
 * 3. If tank becomes negative at i:
 *      → cannot start from any index ≤ i
 *      → reset start = i+1 and tank = 0
 * 4. Final start is answer (if total >= 0)
 *
 * Why greedy works:
 * - If we fail at i, any start before i will also fail
 * - So we skip all those and jump directly to i+1
 *
 * Complexity:
 * - Time: O(n)
 * - Space: O(1)
 */

int canCompleteCircuit(vector<int> &gas, vector<int> &cost)
{

    int totalNet = 0;
    int start = 0;
    int currFuel = 0;
    for (int i = 0; i < cost.size(); i++)
    {
        int currNet = gas[i] - cost[i];
        totalNet += currNet;
        currFuel += currNet;
        if (currFuel < 0)
        { // means started from wrong station
            start = i + 1;
            currFuel = 0;
        }
    }
    return totalNet < 0 ? -1 : start;
}
/**
// !WORD LADDER (LC 127) - OPTIMAL BFS
 *
 * Idea:
 * - Use BFS from beginWord → shortest path
 * - Instead of building graph (O(n²)), generate neighbors on the fly
 *
 * Approach:
 * - Put all words in unordered_set for O(1) lookup
 * - For each word:
 *     try changing each character (a → z)
 *     if new word exists in set:
 *         push to queue + remove from set (mark visited)
 *
 * Key Points:
 * - BFS ensures shortest transformation
 * - Erasing from set prevents revisiting
 * - No need for explicit visited map
 *
 * Complexity:
 * - Time:  O(n * L * 26) ≈ O(n * L)
 * - Space: O(n)
 *
 * Insight:
 * - Generate neighbors instead of comparing all pairs
 */

/**
 * PSEUDO + COMPLEXITY
 *
 * 1. Build adjacency list (graph)
 *    for i = 0 → n:
 *      for j = i+1 → n:
 *        if isValid(word[i], word[j]) → O(L)
 *           connect both
 *
 *    Time: O(n² * L)
 *
 * 2. Connect beginWord with all valid words
 *    loop over wordList:
 *      isValid(beginWord, word) → O(L)
 *
 *    Time: O(n * L)
 *
 * 3. BFS traversal
 *    push beginWord
 *    while queue:
 *      process level
 *      for each neighbor:
 *         visit once
 *
 *    Time: O(n²) (edges in worst case)
 *
 * --------------------------------------------------
 * FINAL TIME: O(n² * L)
 * SPACE: O(n²) (adjacency list) + O(n) (queue, visited)
 * --------------------------------------------------
 */
int ladderLength(string beginWord, string endWord, vector<string> &wordList)
{
    unordered_set<string> s(wordList.begin(), wordList.end());
    if (!s.count(endWord))
        return 0;
    queue<string> q;
    q.push(beginWord);
    int l = 0;
    while (!q.empty())
    {
        l++;
        int n = q.size();
        for (int i = 0; i < n; i++)
        {
            string word = q.front();
            q.pop();
            if (word == endWord)
                return l;
            for (int j = 0; j < word.size(); j++)
            {
                char c = word[j];
                for (char k = 'a'; k <= 'z'; k++)
                {
                    if (k == c)
                        continue;
                    word[j] = k;
                    if (s.count(word))
                    {
                        q.push(word);
                        s.erase(word);
                    }
                }
                word[j] = c;
            }
        }
    }
    return 0;
}
/**
// !Max Path Sum in Binary Tree
 *
 * - Use DFS (postorder)
 * - At each node:
 *     compute left and right gains
 *     ignore negatives (take 0)
 *
 * - Update global max with:
 *     left + right + root
 *
 * - Return to parent:
 *     root + max(left, right)
 *
 * Time: O(n)
 * Space: O(h)
 */
int solve(TreeNode *root, int &mx)
{
    if (!root)
        return 0;
    int lsum = solve(root->left, mx);
    int rsum = solve(root->right, mx);
    mx = max(mx, lsum + rsum + root->val);
    return max(max(lsum, rsum) + root->val, 0);
}
int maxPathSum(TreeNode *root)
{
    // at a node, l sum + r sum + node
    int mx = INT_MIN;
    solve(root, mx);
    return mx;
}

/*
PROBLEM: Count distinct subsequences of s that equal t

IDEA:
- At each index i in s, we have 2 choices:
    1) Take s[i] (only if s[i] == t[j])
    2) Skip s[i]
- This forms a classic pick / not-pick recursion

STATE:
- dp[i][j] = number of ways to form t[0..j] using s[0..i]

TRANSITION:
- If s[i] == t[j]:
    dp[i][j] = dp[i-1][j-1] (take) + dp[i-1][j] (skip)
- Else:
    dp[i][j] = dp[i-1][j] (skip only)

BASE CASES:
- j < 0 → t is fully formed → return 1
- i < 0 → s खत्म but t left → return 0

ANSWER:
- solve(n-1, m-1)

COMPLEXITY:
- Time: O(n * m)
- Space: O(n * m) (can optimize to O(m))

EDGE CASES:
- t == "" → always 1
- s == "" → 0 if t not empty
- repeated chars → multiple combinations

INTUITION (1 line):
- Count all ways to match t as a subsequence in s using include/exclude.
*/
class Solution
{
public:
    /*
    dp[i][j] = number of ways to form t[0..j] using s[0..i]
    */

    int solve(int i, int j, string &s, string &t, vector<vector<int>> &dp)
    {

        // ✅ If t is fully formed
        if (j < 0)
            return 1;

        // ❌ If s खत्म but t still left
        if (i < 0)
            return 0;

        if (dp[i][j] != -1)
            return dp[i][j];

        // If characters match → 2 choices
        if (s[i] == t[j])
        {
            int take = solve(i - 1, j - 1, s, t, dp); // match both
            int skip = solve(i - 1, j, s, t, dp);     // skip s[i]
            return dp[i][j] = take + skip;
        }

        // If not match → only skip s[i]
        return dp[i][j] = solve(i - 1, j, s, t, dp);
    }

    int numDistinct(string s, string t)
    {
        int n = s.size(), m = t.size();
        vector<vector<int>> dp(n, vector<int>(m, -1));
        return solve(n - 1, m - 1, s, t, dp);
    }
};

/*
// !LC 621 - Task Scheduler

INTUITION:
- Always run the most frequent task (maxHeap)
- After running → it goes into cooldown (queue)
- If no task available → jump time to next available task

WE STORE:
- queue: {remaining_freq, unavailable_till_time}

PSEUDO:

build freq map → push into maxHeap

curr_time = 0

while (heap not empty OR queue not empty):

    // ⏩ if nothing to execute, jump time
    if (heap empty AND queue not empty):
        curr_time = queue.front().time + 1

    // release all tasks whose cooldown finished
    while (queue not empty AND queue.front().time < curr_time):
        move back to heap

    // execute best task
    if (heap not empty):
        freq = heap.pop()

        if (freq - 1 > 0):
            queue.push({freq-1, curr_time + n})  // unavailable till this time

    curr_time++

return curr_time

COMPLEXITY:
- Time: O(T log 26) ~ O(T)
- Space: O(26)

KEY POINTS:
- storing "unavailable till time"
- next usable time = t + 1
- so release when: t < curr_time
- cooldown push = curr_time + n
- time jump avoids idle simulation
*/
int leastInterval(vector<char> &tasks, int n)
{
    priority_queue<int> heap; // freq
    queue<pair<int, int>> q;  // freq, unavailable_till_time

    unordered_map<char, int> freqMap; // task freq
    for (char c : tasks)
    {
        freqMap[c]++;
    }
    for (auto it : freqMap)
    {
        char c = it.first;
        int freq = it.second;
        heap.push(freq);
    }

    int curr_time = 0;
    while (!heap.empty() || !q.empty())
    {
        if (heap.empty() && !q.empty())
            curr_time = max(curr_time, q.front().second + 1);
        if (!q.empty() && q.front().second < curr_time)
        {
            // if curr_time > unavailable till time, task can be done
            auto [freq, unavailable_time] = q.front();
            q.pop();
            heap.push(freq);
        }
        if (!heap.empty())
        { // some task can be done now
            int freq = heap.top();
            heap.pop();
            if (freq - 1 > 0)
                q.push({freq - 1, curr_time + n});
        }
        curr_time++;
    }
    return curr_time;
}
/*
LC 621 - Task Scheduler

--------------------------------------------------
CORE INTUITION:
- Most frequent task = bottleneck
- Place it first → creates forced gaps
- Remaining tasks try to fill those gaps

--------------------------------------------------
FORMULA DERIVATION (STEP-BY-STEP):

Suppose:
maxFreq = frequency of most frequent task

Example:
A A A   (maxFreq = 3, n = 2)

We must separate them:
A _ _ A _ _ A

Think in blocks:
(A _ _) (A _ _) (A)

--------------------------------------------------
STEP 1: Number of blocks
- We have (maxFreq - 1) full gaps between tasks

blocks = maxFreq - 1

--------------------------------------------------
STEP 2: Size of each block
- Each gap must have at least n spaces
- Plus 1 position for the task itself

block size = (n + 1)

--------------------------------------------------
STEP 3: Base structure length

(min length without last block)
= (maxFreq - 1) * (n + 1)

--------------------------------------------------
STEP 4: Add last block

If multiple tasks have maxFreq:

Example:
A A A
B B B

Last block = A B  → size = countMax

So:
+ countMax

--------------------------------------------------
FINAL FORMULA:

min_required_length =
    (maxFreq - 1)*(n + 1) + countMax

--------------------------------------------------
WHY max(total, formula)?

Case 1:
remaining tasks ≥ gaps
→ they fill all empty slots
→ no idle needed
→ answer = total

Case 2:
remaining tasks < gaps
→ some slots remain empty
→ idle needed
→ answer = formula

--------------------------------------------------
FINAL ANSWER:

ans = max(total, (maxFreq - 1)*(n + 1) + countMax)

--------------------------------------------------
KEY INSIGHT:
- Formula gives MINIMUM required schedule length
- total gives ACTUAL tasks count
- answer = whichever is larger

--------------------------------------------------
ONE-LINE MEMORY TRICK:
- "(blocks × block_size) + last_block"
*/
int leastInterval(vector<char> &tasks, int n)
{

    unordered_map<char, int> m;
    for (char c : tasks)
        m[c]++;
    int maxFreq = 0;
    for (auto it : m)
    {
        maxFreq = max(maxFreq, it.second);
    }

    int countMax = 0;
    for (auto i : m)
    {
        if (i.second == maxFreq)
            countMax++;
    }
    return max((int)tasks.size(), (maxFreq - 1) * (n + 1) + countMax);
}
/*
// !======================= GROUP ANAGRAMS - 3 APPROACHES ========================

Anagrams = strings having same characters with same frequency.
Example:
"eat", "tea", "ate" are anagrams.

-------------------------------------------------------------------------------
APPROACH 1: SORTING + HASHMAP OF VECTORS
-------------------------------------------------------------------------------
Idea:
Sort every string and use sorted string as key.

Example:
"eat" -> "aet"
"tea" -> "aet"
"ate" -> "aet"

So all strings with same sorted form go in same group.

Code idea:
unordered_map<string, vector<string>> groups;

for each string s:
    key = sorted(s)
    groups[key].push_back(s)

Then collect all map values into result.

Time:
- sorting each string = O(k log k)
- total = O(n * k log k)

Space:
- O(n * k)

Pros:
- simplest and most standard
- easiest to explain and write

Cons:
- sorting makes it slower than frequency approach

-------------------------------------------------------------------------------
APPROACH 2: SORTING + HASHMAP OF INDEX
-------------------------------------------------------------------------------
Idea:
Same sorted key as approach 1,
but instead of storing vector in map,
store index of that group inside result.

This avoids a second loop over hashmap.

Example:
m["aet"] = 0   -> res[0] stores all anagrams of "aet"
m["ant"] = 1   -> res[1] stores all anagrams of "ant"

Code idea:
unordered_map<string, int> m;
vector<vector<string>> res;

for each string s:
    key = sorted(s)

    if key not present:
        m[key] = res.size()
        res.push_back({})

    res[m[key]].push_back(s)

Time:
- sorting each string = O(k log k)
- total = O(n * k log k)

Space:
- O(n * k)

Pros:
- slightly cleaner / more optimized than approach 1
- builds answer directly in one pass

Cons:
- still uses sorting

-------------------------------------------------------------------------------
APPROACH 3: FREQUENCY COUNT KEY (OPTIMIZED)
-------------------------------------------------------------------------------
Idea:
Instead of sorting string,
count frequency of characters and build a unique key.

Because two strings are anagrams iff
all 26 character counts are same.

Possible key styles:

1) Full frequency key:
   "eat" -> "#1#0#0#0#1...#1..."
   (safe and common)

2) Compact key:
   "eat" -> "a1e1t1"
   "tea" -> "a1e1t1"

Code idea:
count freq[26]
build key using freq
use hashmap to group strings

Time:
- counting chars = O(k)
- building key = O(26)
- total = O(n * k)

Space:
- O(n * k)

Pros:
- best time complexity
- better than sorting approach
- interview standout optimization

Cons:
- slightly less intuitive than sorting
- key construction must be done carefully

-------------------------------------------------------------------------------
WHICH ONE TO USE?
-------------------------------------------------------------------------------
1) If interviewer wants clean/simple:
   -> use SORTING approach

2) If you want slightly better implementation:
   -> use SORTING + INDEX approach

3) If interviewer asks optimization / better than sorting:
   -> use FREQUENCY KEY approach

-------------------------------------------------------------------------------
INTERVIEW ONE-LINER
-------------------------------------------------------------------------------
"Sorting solution is standard O(n * k log k),
but we can optimize to O(n * k) by using
character frequency as a unique key."

===============================================================================
*/

/*
------------------ APPROACH 1: SORTING + HASHMAP OF VECTORS ------------------

Use sorted string as key.
All anagrams become same after sorting.

Example:
"eat", "tea", "ate" -> "aet"

Time: O(n * k log k)
Space: O(n * k)
*/

vector<vector<string>> groupAnagrams1(vector<string> &strs)
{
    unordered_map<string, vector<string>> groups;

    for (string &s : strs)
    {
        string key = s;
        sort(key.begin(), key.end());
        groups[key].push_back(s);
    }

    vector<vector<string>> res;
    for (auto &it : groups)
    {
        res.push_back(it.second);
    }

    return res;
}

/*
------------------- APPROACH 2: SORTING + HASHMAP OF INDEX -------------------

Use sorted string as key.
Instead of storing vector in hashmap,
store index of group in result.

This avoids extra traversal of hashmap.

Time: O(n * k log k)
Space: O(n * k)
*/

vector<vector<string>> groupAnagrams2(vector<string> &strs)
{
    unordered_map<string, int> m;
    vector<vector<string>> res;

    for (string &s : strs)
    {
        string key = s;
        sort(key.begin(), key.end());

        if (!m.count(key))
        {
            m[key] = res.size();
            res.emplace_back();
        }

        res[m[key]].push_back(s);
    }

    return res;
}

/*
-------------------- APPROACH 3: FREQUENCY KEY (OPTIMIZED) -------------------

Instead of sorting, count character frequencies.

Build compact key like:
"eat" -> "a1e1t1"
"tea" -> "a1e1t1"

All anagrams produce same frequency key.

Time: O(n * k)
Space: O(n * k)
*/

string makeKey(string &s)
{
    int freq[26] = {0};

    for (char c : s)
    {
        freq[c - 'a']++;
    }

    string key = "";
    for (int i = 0; i < 26; i++)
    {
        if (freq[i] > 0)
        {
            key += char(i + 'a');      // actual character
            key += to_string(freq[i]); // its frequency
        }
    }

    return key;
}

vector<vector<string>> groupAnagrams3(vector<string> &strs)
{
    unordered_map<string, int> m;
    vector<vector<string>> res;

    for (string &s : strs)
    {
        string key = makeKey(s);

        if (!m.count(key))
        {
            m[key] = res.size();
            res.emplace_back();
        }

        res[m[key]].push_back(s);
    }

    return res;
}

/*
PROBLEM: Interleaving string
Check if s3 is formed by interleaving s1 and s2.
Order of characters in s1 and s2 must be preserved.
State:
solve(i, j) = can we form s3 from s1[i...] and s2[j...] ?

At any step, next needed char in s3 is at index (i + j),
because we already used i chars from s1 and j chars from s2.

Try:
1) take s1[i] if it matches s3[i+j]
2) take s2[j] if it matches s3[i+j]

Use DP to avoid recomputing same (i, j) state.
TIME COMPLEXITY:
O(m * n)

Each state (i, j) computed once

-----------------------------------------------------------------------
SPACE COMPLEXITY:
O(m * n)  (DP table + recursion stack)

-----------------------------------------------------------------------
COMMON MISTAKES:
1) Building string (curr) → unnecessary & slow ❌
2) Not using i + j for s3 index ❌
3) Wrong base case ❌
4) Not checking length mismatch ❌
5) DP size (m x n) instead of (m+1 x n+1) ⚠️

-----------------------------------------------------------------------
INTERVIEW ONE-LINER:
"At each step, we match s3[i+j] with either s1[i] or s2[j],
and use DP on (i, j) to avoid recomputation."

*/

bool solve(int i, int j, string &s1, string &s2, string &s3, vector<vector<int>> &dp)
{
    if (i == s1.size() && j == s2.size())
        return true;
    // if(i == s1.size()){
    //     while(j < s2.size()){
    //         if(s3[i+j] != s2[j++]) return false;
    //     }
    //     return true;;
    // }
    // if(j == s2.size()){
    //     while(i<s1.size()){
    //         if(s3[i+j] != s1[i++]) return false;
    //     }
    //     return true;
    // }
    if (dp[i][j] != -1)
        return dp[i][j];

    bool op1 = false;
    bool op2 = false;
    if (i < s1.size() && s1[i] == s3[i + j])
        op1 = solve(i + 1, j, s1, s2, s3, dp);
    if (j < s2.size() && s2[j] == s3[i + j])
        op2 = solve(i, j + 1, s1, s2, s3, dp);

    return dp[i][j] = (op1 || op2) ? 1 : 0;
}
bool isInterleave(string s1, string s2, string s3)
{
    int m = s1.size();
    int n = s2.size();
    if (m + n != s3.size())
        return false;
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return solve(0, 0, s1, s2, s3, dp);
}

/*
// !====================== 91. DECODE WAYS - CPP NOTES ======================

PROBLEM:
Given a string of digits, count number of ways to decode it.
Mapping: '1'->'A', ..., '26'->'Z'

At each index:
1) Take one digit if current char is not '0'
2) Take two digits if formed number is between 10 and 26

Use DP[i] = number of ways to decode string starting from index i.

TIME COMPLEXITY:
O(n) Each index computed once

SPACE COMPLEXITY:
O(n)

*/

int solve(int i, string &s, vector<int> &dp)
{
    if (i >= s.size())
        return 1;
    if (s[i] == '0')
        return 0;
    // // 10,20 valid rest 00 30 40 50 ... invalid
    // if(s[i] == '0' && i-1 >= 0 && (s[i-1] > '2' || s[i-1] == '0')) {
    //     // cout<<s[i-1]<<" "<<s[i]<<" ";
    //     return 0;
    // }
    if (dp[i] != -1)
        return dp[i];
    int single = solve(i + 1, s, dp);
    int notSingle = 0;
    if (s[i] == '1' && i + 1 < s.size())
        notSingle = solve(i + 2, s, dp);
    if (s[i] == '2' && i + 1 < s.size() && s[i + 1] <= '6')
        notSingle = solve(i + 2, s, dp);

    return dp[i] = single + notSingle;
}
int numDecodings(string s)
{
    // leading zeros
    //  if(s[0] == '0') return 0;
    vector<int> dp(s.size(), -1);
    return solve(0, s, dp);
}

// !Detect Squares
/*

Intuition (short)

For query point (x, y):
    choose another point (x, y1) on same vertical line
    now side length = |y1 - y|
    possible square can be:
    right side → x + d
    left side → x - d
    multiply frequencies of all 3 needed points

Algo:

Store freq of each point as m[x][y].

For query point (x,y):
- Pick every other point (x,y1) on same vertical line.
- Side length = abs(y1-y).
- Try making square on:
    1) right side -> x + d
    2) left side  -> x - d
- Add product of frequencies of the 3 required points.

Time:
- add()   -> O(1) avg
- count() -> O(number of distinct y on same x), worst O(n)

Space:
- O(total distinct points)

*/
class DetectSquares
{
    unordered_map<int, unordered_map<int, int>> m; // x,y,freq
public:
    DetectSquares() {}

    void add(vector<int> point)
    {
        int x = point[0];
        int y = point[1];
        m[x][y]++;
    }

    int count(vector<int> point)
    {
        int currx = point[0];
        int curry = point[1];

        int count = 0;
        for (auto &[nexty, freq] : m[currx])
        {
            // all points having same x
            int d = abs(nexty - curry);
            if (d == 0)
                continue; // skip same point

            // pair of points formed
            // p1: {currx,curry} and p2: {currx,nexty}
            // i need two more points to form a square
            // at last count = f2*f3*f4; (product of all freq except {currx,curry})

            // square 1:  p3: currx + d, curry
            //            p4: currx + d, nexty

            // square 2:  p3: currx - d,curry
            //            p4: currx - d, nexty

            count += freq * m[currx + d][curry] * m[currx + d][nexty];
            count += freq * m[currx - d][curry] * m[currx - d][nexty];
        }
        return count;
    }
};
/*
// !84. Largest Rectangle in Histogram
INTUITION:
Every bar wants to become the rectangle height.

For each bar i:
area = heights[i] * width

To get max width for this height,
find:
1) NSL = index of next smaller element on left
2) NSR = index of next smaller element on right

Then:
width = NSR - NSL - 1

WHY MONOTONIC STACK?
A smaller bar tells where current bar's rectangle must stop.
Use increasing stack to find first smaller on both sides in O(n).

IMPORTANT:
Use '<' or '<=' carefully.
- '<'  -> equal heights stay separate (still works)
- '<=' -> equal heights merge earlier (more standard)

TIME:  O(n)
SPACE: O(n)
// ! two methods:
! 1. maintain nextSmallerRight and nextSmallerLeft arrays
! 2. second method: arrays not required
Increasing stack of indices.

When current height is smaller than stack top:
- current i becomes NSR
- new stack top becomes NSL
- compute area for popped bar

Run till i = n with currHeight = 0
to force all remaining bars to be processed.

Time: O(n), Space: O(n)
*/
int largestRectangleArea(vector<int> &heights)
{
    /*
    Every bar wants to be the rectangle height.
    area = ht * width
    width = index(nextSmallerOnRight) - index(nextSmallerOnLeft) - 1;
    */
    int n = heights.size();
    // stack<pair<int, int>> st; // val, index
    // vector<int> nsl(n, -1);
    // for (int i = n - 1; i >= 0; i--) {
    //     while (!st.empty() && heights[i] < st.top().first) {
    //         // if h[i] smaller it can pop
    //         nsl[st.top().second] = i;
    //         st.pop();
    //     }
    //     st.push({heights[i],i});
    // }

    // stack<pair<int,int>>empty;
    // st.swap(empty);

    // vector<int>nsr(n,n);
    stack<int> st;
    int area = 0;
    for (int i = 0; i <= n; i++)
    {
        // while (!st.empty() && heights[i] < st.top().first) {
        //     // if h[i] smaller it can pop
        //     nsr[st.top().second] = i;
        //     st.pop();
        // }
        // st.push({heights[i],i});

        int currHeight = (i == n) ? 0 : heights[i];
        while (!st.empty() && currHeight < heights[st.top()])
        {
            // for st.top() current ele is nsr
            // element just below is nsl
            // calculate area for this element
            int ht = heights[st.top()];
            st.pop();
            int nsr = i;
            int nsl = (st.empty()) ? -1 : st.top();
            int width = nsr - nsl - 1;
            area = max(area, ht * width);
        }
        st.push(i);
    }
    return area;

    // int res=0;
    // for(int i=0;i<n;i++){
    //     int width = nsr[i]-nsl[i] - 1;
    //     int area = heights[i] * width;
    //     res=max(res,area);
    // }
    // return res;
}
/*
========================================================
// !LC 567. Permutation in String
========================================================

PROBLEM:
Given two strings s1 and s2,
return true if s2 contains a substring that is a permutation of s1,
otherwise return false.

A permutation means:
same characters + same frequency, but order can be different.

--------------------------------------------------------
SAMPLE INPUT / OUTPUT
--------------------------------------------------------

Input:
s1 = "ab"
s2 = "eidbaooo"

Output:
true

Explanation:
"ba" is a substring of s2,
and "ba" is a permutation of "ab".


Input:
s1 = "ab"
s2 = "eidboaoo"

Output:
false


Input:
s1 = "adc"
s2 = "dcda"

Output:
true

Explanation:
"cda" is a permutation of "adc"

--------------------------------------------------------
INTUITION
--------------------------------------------------------

We need to check every substring of s2 of size s1.length().

Because:
A permutation of s1 must have:
1) same length
2) same character frequencies

So instead of checking all substrings separately,
we use SLIDING WINDOW of fixed size = s1.length().

--------------------------------------------------------
APPROACH 1: Sliding Window + full freq check
--------------------------------------------------------

Maintain freq[] for s1.

For every window in s2:
- subtract chars entering window
- when window size becomes s1.size():
    check if all freq values are 0

If all freq become 0:
=> current window is a permutation

Time Complexity:
O(26 * n) ≈ O(n)

Space Complexity:
O(26)

--------------------------------------------------------
APPROACH 2: Sliding Window + need counter (Better)
--------------------------------------------------------

freq[ch] = how many more of this char are still needed

need = total characters still needed

Rules:
- When adding right char:
    if freq[ch] > 0 => it was useful => need--
    then freq[ch]--

- When removing left char:
    freq[ch]++
    if freq[ch] > 0 => now this char is needed again => need++

If window size == s1.length() and need == 0:
=> valid permutation found

This avoids checking all 26 chars every time.

Time Complexity:
O(n)

Space Complexity:
O(26)

--------------------------------------------------------
IMPORTANT OBSERVATION
--------------------------------------------------------

freq[ch] meaning:

freq[ch] > 0  => still need this char
freq[ch] == 0 => perfectly matched
freq[ch] < 0  => extra char present in window

--------------------------------------------------------
WHY DUPLICATES MATTER
--------------------------------------------------------

Example:
s1 = "aabc"

Valid permutation must contain:
a -> 2 times
b -> 1 time
c -> 1 time

So using set/unordered_set is WRONG.
We need FREQUENCY ARRAY.

--------------------------------------------------------
EDGE CASES
--------------------------------------------------------

1) s1.size() > s2.size()
=> impossible => return false

2) duplicate chars in s1
=> must match exact count

3) extra chars in window
=> freq can go negative, that's okay

--------------------------------------------------------
OPTIMAL CODE IDEA
--------------------------------------------------------

Window size = s1.length()

Expand right
If char helps => need--

When window size becomes valid:
    if need == 0 => answer found

Then remove left and slide forward

--------------------------------------------------------
DRY RUN
--------------------------------------------------------

s1 = "ab"
s2 = "eidbaooo"

freq initially:
a=1, b=1

need = 2

Window process:
"e"   -> extra char
"ei"  -> extra chars
"id"  -> no match
"db"  -> b matched
"ba"  -> a matched, need = 0

Answer = true

--------------------------------------------------------
MOST COMMON MISTAKES
--------------------------------------------------------

1) Not keeping fixed window size
2) Using set instead of frequency
3) Wrong order while updating freq and need
4) Comparing sorted substrings (works but slower)

--------------------------------------------------------
INTERVIEW ONE-LINER
--------------------------------------------------------

Use a fixed-size sliding window of length s1.length()
and track character frequency difference.
If all required chars are matched in any window,
then s2 contains a permutation of s1.

========================================================
*/
bool found(vector<int> &freq)
{
    for (int i = 0; i < 26; i++)
    {
        if (freq[i] != 0)
            return false;
    }
    return true;
}

bool checkInclusion1(string s1, string s2)
{
    if (s1.size() > s2.size())
        return false;

    vector<int> freq(26);
    for (char c : s1)
    {
        freq[c - 'a']++;
    }

    int start = 0;

    for (int i = 0; i < s2.size(); i++)
    {
        freq[s2[i] - 'a']--; // include current char in window

        int wsize = i - start + 1;

        if (wsize < s1.size())
            continue;

        // check if all matched
        if (found(freq))
        {
            cout << s2.substr(start, s1.size()) << endl;
            return true;
        }

        // remove left char
        freq[s2[start] - 'a']++;
        start++;
    }

    return false;
}

/*
APPROACH 2: Sliding Window + need counter (Optimal)

Key Idea:
- freq[ch] = how many more of this char we still need
- need = total characters still needed

When adding a char:
- if it was needed → need--

When removing a char:
- if it becomes needed again → need++

If need == 0 → valid permutation

Time Complexity:
- O(n)

Space:
- O(26)

Handles:
- duplicates
- extra chars (negative freq)
*/

bool checkInclusion(string s1, string s2)
{
    if (s1.size() > s2.size())
        return false;

    vector<int> freq(26);
    for (char c : s1)
    {
        freq[c - 'a']++;
    }

    int need = s1.size(); // total chars still required
    int start = 0;

    for (int i = 0; i < s2.size(); i++)
    {

        // include right char in window
        if (freq[s2[i] - 'a'] > 0)
        {
            // this char was still needed
            need--;
        }
        freq[s2[i] - 'a']--;

        int wsize = i - start + 1;

        if (wsize < s1.size())
            continue;

        // if all chars matched
        if (need == 0)
        {
            cout << s2.substr(start, s1.size()) << endl;
            return true;
        }

        // remove left char from window
        freq[s2[start] - 'a']++;

        if (freq[s2[start] - 'a'] > 0)
        {
            // removing a useful matched char
            need++;
        }

        start++;
    }

    return false;
}
/*
========================================================
FIXED SIZE SLIDING WINDOW TEMPLATE
========================================================

Used when:
- window size is fixed (k)
- examples:
  - permutation / anagram in string
  - max sum subarray of size k
  - first negative in every window
  - count distinct in every window

--------------------------------------------------------
TEMPLATE
--------------------------------------------------------


int start = 0;
for(int end = 0; end < n; end++) {

    // 1) include s[end] in window
    // update data structure

    int wsize = end - start + 1;

    // 2) if window not yet size k, continue expanding
    if (wsize < k) continue;

    // 3) now window size == k
    // process / check answer here

    // 4) remove s[start] before sliding
    // update data structure

    start++;
}

Ask 4 things:

1) What does entering element do?
2) What does leaving element do?
3) When is window valid?
4) What answer do I compute for each valid window?
*/

/*
// !Problem: Min Window Substring
Find the smallest substring of s that contains
all characters of t (including duplicates).

Example:
s = "ADOBECODEBANC"
t = "ABC"
ans = "BANC"

Idea:
- Keep freq of chars needed from t.
- Expand right pointer:
    if current char was needed, reduce need.
- When need == 0 => valid window found.
- Now shrink from left as much as possible.
- Update minimum valid window.

Meaning of freq[ch]:

        freq[ch] > 0  => still needed
        freq[ch] == 0 => exactly satisfied
        freq[ch] < 0  => extra in window

TC: O(n+m)
Each character is visited at most:
once by r
once by l
So total = 2n + O(m) for creating freq array

SC: O(k)   // k = unique chars in t
*/

/*
APPROACH 1: Sliding Window + freq comparison

Idea:
- freq[ch] = required count of each char from s1
- As window grows, we subtract chars from freq
- If all freq become 0 → exact match (permutation found)

Time Complexity:
- O(26 * n) ≈ O(n)

Space:
- O(26)

Example:
s1 = "ab", s2 = "eidbaooo"
Window "ba" → freq becomes all 0 → match
*/

string minWindow(string s, string t)
{

    int n = s.size();
    int need = t.size();
    unordered_map<char, int> m;
    for (int i = 0; i < t.size(); i++)
        m[t[i]]++;

    int ws = -1;
    int we = 0;
    int win = INT_MAX;

    int l = 0;
    for (int i = 0; i < n; i++)
    {

        // expand window
        if (m[s[i]] > 0)
            need--; // if char found in map and freq 1,2,3,.. etc then need--
        m[s[i]]--;

        // at a point valid window would be found
        // however it is possible ws is not at right place
        // eg: dopeadobec and abc
        // ws=0, window valid found when i is at end
        // now ws needs to move till adobec to be minimum
        while (need == 0)
        {
            // once a valid windwo found try shrinking it
            if (i - l + 1 < win)
            {
                // better window found
                win = i - l + 1;
                ws = l;
                we = i;
            }

            m[s[l]]++;
            // if this is some unnecessary letter like dope its freq will stay <= 0
            // as it was already m[s[i]]-- by i
            if (m[s[l]] > 0)
            {
                // map says i have a char which is required
                need++;
            }
            l++;
        }
    }

    string res = ws == -1 ? "" : s.substr(ws, win);
    return res;
}

/*
// !LC 72 - Edit Distance

Problem:
Given two strings word1 and word2,
return minimum operations needed to convert word1 -> word2.

Allowed operations:
1) Insert a character
2) Delete a character
3) Replace a character

--------------------------------------------------
Example:
Input:
word1 = "horse"
word2 = "ros"

Output:
3

Explanation:
horse -> rorse   (replace h -> r)
rorse -> rose    (delete r)
rose  -> ros     (delete e)

--------------------------------------------------
Intuition:
At every step compare current chars of both strings.

State:
solve(i, j) = minimum operations to convert
word1[i....] -> word2[j....]

Two cases:

1) If chars already same:
   No operation needed, move both pointers.

2) If chars different:
   Try all 3 operations and take minimum:

   a) Delete word1[i]
      -> move i only
      -> 1 + solve(i+1, j)

   b) Insert word2[j] into word1
      -> current word1[i] still not matched
      -> move j only
      -> 1 + solve(i, j+1)

   c) Replace word1[i] with word2[j]
      -> both chars handled
      -> move both
      -> 1 + solve(i+1, j+1)

--------------------------------------------------
Base Cases:

1) If word1 is fully used:
   Need to insert all remaining chars of word2
   cost = word2.size() - j

2) If word2 is fully formed:
   Need to delete all remaining chars of word1
   cost = word1.size() - i

--------------------------------------------------
Memoization:
dp[i][j] stores answer for state (i, j)
to avoid recomputation.

dp[i][j] = min operations for
word1[i....] -> word2[j....]

--------------------------------------------------
Pseudo Code:

solve(i, j):
    if i == word1.size(): return word2.size() - j
    if j == word2.size(): return word1.size() - i

    if word1[i] == word2[j]:
        return solve(i+1, j+1)

    del = 1 + solve(i+1, j)
    ins = 1 + solve(i, j+1)
    rep = 1 + solve(i+1, j+1)

    return min(del, ins, rep)

Answer = solve(0, 0)

--------------------------------------------------
Time Complexity:
O(m * n)

Reason:
There are only m*n states (i, j),
and each state is solved once.

--------------------------------------------------
Space Complexity:
O(m * n)   -> dp table
O(m + n)   -> recursion stack

Overall usually written as:
O(m * n)

--------------------------------------------------
Key Idea Summary:
Compare suffixes of both strings.
If chars match -> move both.
Else try delete / insert / replace and take minimum.
*/
int solve(int i, int j, string &s1, string &s2, vector<vector<int>> &dp)
{
    if (i == s1.size())
    {
        // s1 fully used, so s1->s2 insert all remaining s2 chars in s1
        return s2.size() - j;
    }
    if (j == s2.size())
    {
        // s2 fully formed, remove remaining from s1
        return s1.size() - i;
    }

    if (dp[i][j] != -1)
        return dp[i][j];

    if (s1[i] == s2[j]) // no operatin required
        return dp[i][j] = solve(i + 1, j + 1, s1, s2, dp);

    int del = 0;
    int ins = 0;
    int rep = 0;

    // delete means removeing one from s1, then comparing next s1 char with
    // curr s2 char
    del = 1 + solve(i + 1, j, s1, s2, dp);

    // insert means jth s2 char is inserted in s1, now compare
    // next s2's char
    ins = 1 + solve(i, j + 1, s1, s2, dp);

    // rep means i have mathced s1's i and s2's j
    rep = 1 + solve(i + 1, j + 1, s1, s2, dp);

    return dp[i][j] = min({del, ins, rep});
}
int minDistance(string word1, string word2)
{
    int m = word1.size();
    int n = word2.size();
    vector<vector<int>> dp(m, vector<int>(n, -1));
    return solve(0, 0, word1, word2, dp);
}
/*
// !LC 572 - Subtree of Another Tree
// ! solve using serialize and kmp string matching
Idea:
At every node of root:
    check if subtree starting here is identical to subRoot

Functions:
1) same(a, b) -> check identical trees
2) isSubtree -> try every node as starting point

Return:
same(root, subRoot) ||
isSubtree(left) ||
isSubtree(right)

TC: O(n * m)
SC: O(h)
*/
bool same(TreeNode *a, TreeNode *b)
{
    if (!a && !b)
        return true;
    if (!a || !b)
        return false;
    if (a->val != b->val)
        return false;

    return same(a->left, b->left) && same(a->right, b->right);
}
bool isSubtree(TreeNode *root, TreeNode *subRoot)
{
    if (!root)
        return false;

    if (same(root, subRoot))
        return true;
    else
        return isSubtree(root->left, subRoot) ||
               isSubtree(root->right, subRoot);
};

/*
// !LC 57 - Insert Interval

Idea:
Intervals are already sorted and non-overlapping.

Input: intervals = [[1,2],[3,5],[6,7],[8,10],[12,16]],
        newInterval = [4,8]
Output: [[1,2],[3,10],[12,16]]

So process in 3 parts:
1) Add all intervals before newInterval
2) Merge all overlapping intervals into newInterval
3) Add all remaining intervals

Overlap condition:
intervals[i][0] <= newend

TC: O(n)
SC: O(n)
*/
vector<vector<int>> insert(vector<vector<int>> &intervals, vector<int> &newInterval)
{
    vector<vector<int>> res;

    // Current interval that may expand after merging overlaps
    int newstart = newInterval[0];
    int newend = newInterval[1];

    int n = intervals.size();
    int i = 0;

    // 1) Add all intervals completely before newInterval
    // No overlap if current interval ends before newInterval starts
    while (i < n && intervals[i][1] < newstart)
    {
        res.push_back(intervals[i++]);
    }

    // 2) Merge all overlapping intervals with newInterval
    // Overlap exists if current interval starts before/equal to newInterval end
    while (i < n && intervals[i][0] <= newend)
    {
        newstart = min(newstart, intervals[i][0]);
        newend = max(newend, intervals[i][1]);
        i++;
    }

    // Push the final merged interval once
    res.push_back({newstart, newend});

    // 3) Add all remaining intervals (all lie after merged interval)
    while (i < n)
    {
        res.push_back(intervals[i++]);
    }

    return res;
}

/*
==================================================
LC 55. Jump Game
==================================================

PROBLEM:
Can we reach last index?

Sample:
nums = [2,3,1,1,4] -> true
nums = [3,2,1,0,4] -> false

INTUITION:
At every index, track farthest position reachable so far.
If current index > farthest, this index is unreachable.

GREEDY:
farthest = max(farthest, i + nums[i])

TC: O(n)
SC: O(1)

--------------------------------------------------

LC 45. Jump Game II
==================================================

PROBLEM:
Find minimum jumps needed to reach last index.

Sample:
nums = [2,3,1,1,4] -> 2
Explanation:
0 -> 1 -> 4

INTUITION:
Think in BFS levels:
current range = all indices reachable in current jump
farthest = best reach for next jump

When current range ends:
=> must take one jump

GREEDY:
- expand farthest
- when i == currEnd, take jump

TC: O(n)
SC: O(1)

--------------------------------------------------

KEY DIFFERENCE:
Jump Game I  -> Can reach? (true/false)
Jump Game II -> Min jumps needed

Both are GREEDY optimal.
*/

bool canJump(vector<int> &nums)
{
    int farthest = 0;

    for (int i = 0; i < nums.size(); i++)
    {
        if (i > farthest)
            return false; // can't even reach this index
        farthest = max(farthest, i + nums[i]);
    }

    return true;
}
bool canJump(vector<int> &nums)
{
    int n = nums.size();
    if (n == 1)
        return true;
    int jumpsRem = nums[0];
    int i = 1;
    while (i < n && jumpsRem > 0)
    {
        jumpsRem--;
        // cout << i << " " << jumpsRem << " " << nums[i] << endl;
        jumpsRem = max(nums[i], jumpsRem);
        i++;
    }
    return i == n;
}

int jump(vector<int> &nums)
{
    int jumps = 0, range = 0, farthest = 0;

    for (int i = 0; i < nums.size() - 1; i++)
    {
        farthest = max(farthest, i + nums[i]);

        if (i == range)
        { // current jump range finished
            jumps++;
            range = farthest;
        }
    }

    return jumps;
}
int jump(vector<int> &nums)
{
    int n = nums.size();
    if (n == 1)
        return 0;
    int jump = 1;
    int farthest = nums[0];
    int range = nums[0];
    for (int i = 1; i < n; i++)
    {
        if (range >= n - 1)
            return jump;
        if (i > range)
        {
            range = farthest;
            jump++;
        }
        farthest = max(farthest, i + nums[i]);
    }
    return jump;
}

// Problem: 43. Multiply Strings
// Input: num1 = "123", num2 = "45"
// Output: "5535"

// Intuition:
// Do normal school multiplication digit by digit.
// Product of num1[j] and num2[i] contributes to positions i+j and i+j+1 in result.

// Pseudo:
// 1. Create res array of size m+n filled with 0
// 2. Traverse both strings from right to left
// 3. Multiply digits and add to res[i+j+1]
// 4. Put carry in res[i+j]
// 5. Skip leading zeros and build final string

// Mistakes:
// Don't convert whole string to int/long long -> overflow
// Don't forget res size should be m+n
// Be careful with carry placement: ones at i+j+1, carry at i+j
// Handle "0" case separately

// TC: O(m*n)
// SC: O(m+n)

long long solve(string num)
{
    // "123" = 100 + 20 + 3
    long long res = 0;
    long long n = num.size();
    long long multiplier = 1;
    for (long long i = n - 1; i >= 0; i--)
    {
        char c = num[i];
        res += ((c - '0') * multiplier);
        multiplier *= 10;
    }
    return res;
}
string solve2(long long num)
{
    // 123 = "1" + "2" + "3"
    if (num == 0)
        return "0";
    string res = "";
    while (num)
    {
        long long d = num % 10;
        num /= 10;
        res.push_back((d + '0'));
    }
    reverse(res.begin(), res.end());
    return res;
}
string multiply(string num1, string num2)
{
    if (num1 == "0" || num2 == "0")
        return "0";
    int m = num1.size();
    int n = num2.size();
    vector<int> res(m + n, 0);

    //         j
    //     4 9 2 num1
    //       1 3 num2
    //         i
    //    --------
    for (int i = n - 1; i >= 0; i--)
    {
        for (int j = m - 1; j >= 0; j--)
        {
            int a = num2[i] - '0';
            int b = num1[j] - '0';
            int prod = a * b;
            // res[i+j+1] = ones place
            // res[i+j]   = carry
            res[i + j + 1] += (prod);
            res[i + j] += res[i + j + 1] / 10;
            res[i + j + 1] %= 10;
        }
    }
    int i = 0;
    while (res[i] == 0)
        i++;
    string prod = "";
    while (i < res.size())
    {
        prod.push_back(res[i++] + '0');
    }
    return prod;
}
// Problem: 50. Pow(x, n)
// Input: x = 2.0, n = 10
// Output: 1024.0

// Intuition:
// Instead of multiplying x n times, use binary exponentiation.
// If n is odd -> multiply answer by x
// Every step: square x and halve n

// Why it works:
// x^13 = x * x^12
//      = x * (x^2)^6
// so each step reduces exponent fast

// Handle negative power:
// x^-n = 1 / x^n
// Convert n to long long first to safely handle INT_MIN

// Pseudo:
// 1. If n < 0, make x = 1/x and n = -n
// 2. While n > 0:
//      if n is odd -> res *= x
//      x = x * x
//      n /= 2
// 3. Return res

// TC: O(log n)
// SC: O(1)

// Mistakes:
// Don't do O(n) multiplication
// Don't use abs(n) directly on int -> breaks for INT_MIN
// Convert n to long long first
double p(double x, long long n)
{
    if (n == 0)
        return 1.0;
    if (n % 2 == 0)
        return p((x * x), n / 2);
    else
        return x * p((x * x), n / 2);
}
double myPow(double x, int n)
{
    if (n == 0 || x == 1)
        return 1;

    long long N = n;
    if (N < 0)
    {
        x = 1 / x;
        N = -N;
    }
    // double res = p(x, N);
    double res = 1.0;
    while (N)
    {
        if (N % 2 != 0)
        {
            // odd
            res *= x;
        }
        x *= x;
        N /= 2;
    }
    return res;
}
// ! ==========

// !=============================================================================

int missingNumber(vector<int> &nums)
{
    // int n=nums.size();
    // unordered_map<int,int>m;
    // for(int i=0;i<=n;i++)m[i]=1;

    // for(int i=0;i<n;i++){
    //     m[nums[i]]--;
    // }
    // for(int i=0;i<=n;i++){
    //     if(m[i] == 1) return i;
    // }
    // return -1;
    // int sum=0;
    // for(int i:nums) sum+=i;

    // int n = nums.size();
    // int actualSum = n*(n+1)/2;
    // return actualSum - sum;
    int ans = 0;
    // 0^5 = 5
    // 5^5 = 0 Same numbers cancel
    // 4^5 = 5^4 Order doesn’t matter
    for (int i = 0; i < nums.size(); i++)
    {
        ans ^= i ^ nums[i];
    }
    ans ^= nums.size();
    // ans = (0 ^ 1 ^ 2 ^ 3) ^ (0 ^ 1 ^ 3)
    return ans;
}

// ! container with most water
int maxArea(vector<int> &h)
{
    int n = h.size();
    int res = 0;
    // for(int i=0;i<n;i++){
    //     for(int j=i+1;j<n;j++){
    //         int water=min(h[i],h[j])*(j-i);
    //         res=max(res,water);
    //     }
    // }
    int i = 0;
    int j = n - 1;
    while (i < j)
    {
        int water = min(h[i], h[j]) * (j - i);
        res = max(res, water);
        if (h[i] < h[j])
            i++;
        else
            j--;
    }
    return res;
}

// ! count good nodes
void dfs(TreeNode *node, int &count, int mx)
{
    if (!node)
        return;
    if (node->val >= mx)
    {
        count++;
        mx = max(mx, node->val);
    }
    dfs(node->left, count, mx);
    dfs(node->right, count, mx);
}
int goodNodes(TreeNode *root)
{
    // dfs
    // if curr node >= max node till now, count++, update max
    int count = 0;
    dfs(root, count, -1e9);
    return count;
}
// Time : O(n) — every node visited once
// Space : O(h) — recursion stack
int main()
{

    return 0;
}