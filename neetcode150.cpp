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
#include <cmath>
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