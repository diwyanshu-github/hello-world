#include <iostream>
using namespace std;
#include <string>
#include <vector>
#include <algorithm>

//< Backtracking Blueprint
/*
function backtrack(parameters){
    if (goal condition met):
        save current configuration (e.g., result.append(...))
        return

    for each choice in current options{
        if (choice is valid){                   // Prune invalid choices
            make the choice                     // e.g., mark visited, push to temp
            backtrack(updated parameters)       // Explore next level
            undo the choice                     // e.g., unmark, pop from temp
        }
    }
 }

 */

//: Permutations of numbers
/*
        Time Complexity:
        - Total permutations = n!
        - At each level, loop runs:
            Level 0: n
            Level 1: n-1
            ...
            Level n-1: 1
        => O(n!) total calls
        and in one call for loop for iterating choices O(n)
        !So, TC: n! * n
        !avoid anything with TC more than O(1) inside for loop
        !TC will become n!*n*n if isValid() takes O(n) time

        Space Complexity:
        - Recursion stack = O(n)
        - visited[] = O(n)
        - temp[] = O(n)
       !- storing n! vectors, each of length n, in result
        - result = O(n! * n)
*/

void permutations1(vector<int> &temp, vector<int> &choices, int count, vector<bool> &visited, vector<vector<int>> &res)
{
    // Base case: if we've picked 'n' elements, add current configuration
    if (count == choices.size())
    {
        res.push_back(temp);
        return;
    }

    // Explore all choices at the current decision point
    for (int i = 0; i < choices.size(); i++)
    {
        // Check if the current choice is valid (not already used)
        if (!visited[i])
        {
            // Make the choice
            visited[i] = true;
            temp.push_back(choices[i]);

            // Explore further with the current choice
            permutations1(temp, choices, count + 1, visited, res);

            // Undo the choice (backtrack)
            temp.pop_back();
            visited[i] = false;
        }
    }
}

//: Permutations with duplicates
//  Analogy to Subsets:
//  Idea is to Pick duplicates together, not skip–pick–skip.

// If you use the first 2, and then the second 2, it’s valid. Example path: [1, 2, 2]
// But if you skip the first 2 and use the second one instead, you’re treating them as different
// you’ll get the same path [1, 2, 2] again from a different branch.
void permutations2(vector<int> &choices, vector<int> &temp, vector<bool> &vis, vector<vector<int>> &res)
{
    if (temp.size() == choices.size())
    {
        res.push_back(temp);
        return;
    }

    for (int i = 0; i < choices.size(); i++)
    {
        if (vis[i])
            continue;
        if (i > 0 && choices[i] == choices[i - 1] && !vis[i - 1])
            continue;
        // If previous was skipped (not visited), skip the current too to avoid
        // counting the same choice as different
        // so all duplicates inlcuded in one branch only
        vis[i] = true;
        temp.push_back(choices[i]);
        permutations2(choices, temp, vis, res);
        vis[i] = false;
        temp.pop_back();
    }
}
template <typename T>
void print2DVector(const vector<vector<T>> &res)
{
    for (const auto &row : res)
    {
        for (const auto &x : row)
        {
            cout << x << " ";
        }
        cout << endl;
    }
}
/*
'&' avoids copying; allows modifying originals.
'const &' avoids copying; prevents modification.
'auto' copies; original not changed.

vector<int> nums = {1, 2, 3};
// Copies elements; originals unchanged
for (auto x : nums) {
    x += 10;
}
// nums = {1, 2, 3}

// Reference; modifies originals
for (auto& x : nums) {
    x += 10;
}
// nums = {11, 12, 13}

// Const reference; no modification allowed
for (const auto& x : nums) {
    // x += 10; // Error: can't modify const reference
    cout << x << " "; // Prints 11 12 13
}

*/

//: N-Queens
// Time	O(n!)
// For each row, you're trying all possible column placements (n choices),
// and for each valid one, you go to the next row.
//  In the worst case, you try every column in every row.
// Row 0 → up to n choices
// Row 1 → up to n - 1 choices, beacause at least one column where prev queen placed, there no queen possible now
// Row 2 → up to n - 2 choices
// Space O(n) for visited
bool isValidNQueens(vector<string> &grid, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        if (grid[i][col] == 'Q')
            return false;
    }

    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
    {
        if (grid[i][j] == 'Q')
            return false;
    }

    for (int i = row, j = col; i >= 0 && j < grid[0].size(); i--, j++)
    {
        if (grid[i][j] == 'Q')
            return false;
    }

    return true;
}
void NQueens(vector<string> &grid, int row, int n, vector<vector<string>> &res, vector<bool> &up, vector<bool> &ld, vector<bool> &rd)
{
    if (row == n)
    { // no more rows left
        res.push_back(grid);
        return;
    }
    for (int j = 0; j < n; j++)
    {
        if (up[j] || ld[n - (row - j) - 1] || rd[row + j])
            continue;
        up[j] = true;
        ld[n - (row - j) - 1] = true;
        rd[row + j] = true;
        grid[row][j] = 'Q';
        NQueens(grid, row + 1, n, res, up, ld, rd);
        grid[row][j] = '.';
        up[j] = false;
        ld[n - (row - j) - 1] = false;
        rd[row + j] = false;
    }
}


// TC: O(4^(n²))  Each cell can go in up to 4 directions,
// SC: O(n²) Max recursion depth/path length ≈ n²

void Maze(vector<vector<int>> &m, int row, int col, int n, string &temp, vector<string> &res, vector<int> &dx, vector<int> &dy, vector<char> &dir)
{
    if (row == n - 1 && col == n - 1)
    {
        res.push_back(temp);
        return;
    }
    m[row][col] = 0;
    for (int i = 0; i < 4; i++)
    {
        int nextR = row + dx[i];
        int nextC = col + dy[i];
        if (nextR >= 0 && nextC >= 0 && nextR < n && nextC < n && m[nextR][nextC] == 1)
        {
            temp.push_back(dir[i]);
            Maze(m, nextR, nextC, n, temp, res, dx, dy, dir);
            temp.pop_back();
        }
    }
    m[row][col] = 1;
    return;
}

vector<string> findPath(vector<vector<int>> &m, int n)
{
    vector<string> res;
    string temp;
    if (m[0][0] == 0 || m[n - 1][n - 1] == 0)
        return res;
    vector<int> dx = {-1, 0, 1, 0};
    vector<int> dy = {0, 1, 0, -1};
    vector<char> dir = {'U', 'R', 'D', 'L'};
    Maze(m, 0, 0, n, temp, res, dx, dy, dir);
    return res;
}

//: Sudoko
/*
TC: For each empty cell, you try all 9 digits → branching factor = 9
Maximum number of empty cells = 81 (all cells empty)
So, worst case = O(9^81) — exponential time due to backtracking
Recursion stack: max depth = 81 → O(81) = O(1) (constant space)
*/
bool isValid(vector<vector<char>> &grid, int row, int col, char choice)
{
    for (int i = 0; i < 9; i++)
    {
        if (grid[row][i] == choice || grid[i][col] == choice)
            return false;
    }

    int x = row - row % 3;
    ;
    int y = col - col % 3;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (grid[x + i][y + j] == choice)
                return false;
        }
    }
    return true;
}
bool sudoko(vector<vector<char>> &grid, int row, int col)
{
    if (row == 9)
        return true;
    if (col == 9)
        return sudoko(grid, row + 1, 0);
    if (grid[row][col] != '.')
        return sudoko(grid, row, col + 1);

    for (char i = '1'; i <= '9'; i++)
    {
        if (isValid(grid, row, col, i))
        {
            grid[row][col] = i;
            if (sudoko(grid, row, col + 1))
                return true;
            grid[row][col] = '.';
        }
    }
    return false;
}

//! Palindrome Partitioning
// A string of length n has n−1 breakpoints.
// Each breakpoint has 2 choices: cut or don’t cut.
// So ≈ 2ⁿ possible partitions → ~2ⁿ recursive calls.
// Each call may check substrings → O(n).
// TC: O(n · 2ⁿ)
//! 2ⁿ = total nodes in recursion tree
//! recursion depth = n (one cut per index)
// SC: O(n)
bool isPalindrome(string &s, int start, int end)
{
    // Loop while start < end
    while (start < end){
        // If mismatch, not a palindrome
        if (s[start] != s[end])
            return false;
        // Move pointers inward
        start++;
        end--;
    }
    // All characters matched
    return true;
}

void palindromePartition(int index, string &s, vector<string> &path, vector<vector<string>> &res)
{
    // If index reaches end of string, store current partition
    if (index == s.length())
    {
        res.push_back(path);
        return;
    }

    // Try all possible substrings
    for (int i = index; i < s.length(); i++)
    {
        // If substring is a palindrome
        if (isPalindrome(s, index, i))
        {
            // Add substring to current path
            path.push_back(s.substr(index, i - index + 1));
            // Recur for remaining string
            palindromePartition(i + 1, s, path, res);
            // Backtrack to try other partitions
            path.pop_back();
        }
    }
}
// 2ⁿ recursive calls (n indexes to cut, at each index cut/not cut two options, 2 x 2 x 2...n times)
// Each find() is O(m) where m = dic.size().
// Substring creation s.substr(i, j-i+1) is O(n).
//  TC O(2ⁿ · n · m)
//! 2ⁿ = number of TOTAL nodes in recursion tree
//! but recursion depth = n
//! Maximum recursion depth: linear → O(n)
// SC: O(n)
bool wordBreakBacktrack(string s, int i, vector<string> &dic)
{
    if (i == s.size())
    {
        return true;
    }

    for (int j = i; j < s.size(); j++)
    {
        if (find(dic.begin(), dic.end(), s.substr(i, j - i + 1)) != dic.end())
        {
            if (wordBreakBacktrack(s, j + 1, dic)) // try this cut
                return true;
        }
    }
    return false;
}
bool wordBreak(string s, vector<string> &wordDict)
{
    return wordBreakBacktrack(s, 0, wordDict);
}

//: kth Permutation
// ~n! recursive calls (each position has n, n-1, n-2... choices → n! total nodes)
// TC = O(n · n!)
// n! = total nodes in recursion tree
// but recursion depth = n (one level per digit)
// SC: O(n)
bool kthPermutation(string &s, vector<bool> &vis, string &t, int n, int &k, string &ans)
{
    if (t.size() == n)
    {
        k--;
        if (k == 0)
            ans = t;
        return k == 0; // stop recursion once found
    }

    for (int j = 0; j < n; j++)
    {
        if (!vis[j])
        {
            vis[j] = true;
            t.push_back(s[j]);

            if (kthPermutation(s, vis, t, n, k, ans))
                return true;

            vis[j] = false;
            t.pop_back();
        }
    }
    return false;
}

string getPermutation(int n, int k)
{
    string s = "123456789";
    s = s.substr(0, n);

    vector<bool> vis(n, false);
    string t = "", ans = "";

    kthPermutation(s, vis, t, n, k, ans);
    return ans;
}

// Time: O(n²) Each of n positions does an erase on a vector → O(n) each → n·n.
// Space: O(n) for nums, fact, and ans
string getPermutationOpt(int n, int k) {
        vector<int>fac={1,1,2,6,24,120,720,5040,40320,362880};
        vector<int>nums;
        for(int i=1;i<=n;i++){
            nums.push_back(i);
        }
        int grp_size;
        int grp_num;
        string res;
        k--;
        while(n>0){
            grp_size = fac[n-1];
            grp_num = k/grp_size;
            res.push_back('0' + nums[grp_num]);
            nums.erase(nums.begin()+grp_num);
            n--;
            k=k%grp_size;
        }
        return res;

    }
int main()
{
    // Permutations
    // vector<int> nums = {2, 2, 2, 3};
    // vector<vector<int>> res;                  // To store all permutations
    // vector<int> temp;                         // Current permutation
    // vector<bool> visited(nums.size(), false); // Tracks visited elements
    // int count = 0;                            // Number of elements selected so far
    // // permutations1(temp, nums, count, visited, res);
    // permutations2(nums,temp,visited,res);
    // print2DVector(res);

    // N-Queens
    // int n = 4;
    // string s;
    // for (int i = 0; i < n; i++)
    //     s.push_back('.');
    // vector<string> grid(n, s);
    // vector<vector<string>> res;
    // vector<bool> up(n, false);
    // vector<bool> ld(2 * n - 1, false);
    // vector<bool> rd(2 * n - 1, false);
    // NQueens(grid, 0, n, res, up, ld, rd);

    return 0;
}