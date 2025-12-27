#include <iostream>
using namespace std;

#include <vector>
#include <stack>
#include <string>
#include <algorithm>

// If a recursive function calls itself twice, and reduces problem by 1 each time → O(2ⁿ)
// Use recursion tree to count total nodes → that’s your time complexity
// If a recursive function makes k calls each reducing the problem size by 1,
// the time complexity is O(kⁿ) because the number of calls doubles/triples (etc.)
// at each of the n recursion levels.

//! identification
//  problem recursively defined
//  if op for n is depended on n-1 op
//  use IBH: design hypo, something/induction, base condition

// print 1 to n OR n to 1
void printN(int n)
{
    if (n == 0) // base condition
        return;
    // cout << n << " "; // print n to 1
    printN(n - 1);    // hypothesis
    cout << n << " "; // something/induction
}

// Sort Array using Recursion
// O(n) per call
void insertArray(vector<int> &v, int x)
{
    if (v.size() == 0 || (x >= v.back()))
    {
        v.push_back(x);
        return;
    }
    int last = v.back();
    v.pop_back();
    insertArray(v, x);
    v.push_back(last);
}
// O(n²)
void sortArray(vector<int> &v)
{
    if (v.size() == 1)
        return;
    int last = v.back();
    v.pop_back();
    sortArray(v);         // hypothesis
    insertArray(v, last); // something/induction
}

void printStack(stack<int> st)
{
    while (!st.empty())
    {
        cout << st.top() << " ";
        st.pop();
    }
    cout << endl;
}
// Sort Stack using Recursion
// O(n) per call
void insertStack(stack<int> &s, int x)
{
    if (s.empty() || x <= s.top())
    {
        s.push(x);
        return;
    }
    int top = s.top();
    s.pop();
    insertStack(s, x);
    s.push(top);
}
// O(n²)
void sortStack(stack<int> &s)
{
    if (s.size() == 0)
        return;
    int x = s.top();
    s.pop();
    sortStack(s);
    insertStack(s, x);
}

// Function to delete middle element of a stack.
// TC O(n)
void helper(stack<int> &s, int pop)
{
    if (pop == 0)
    {
        cout << "middle " << s.top() << "\n";
        s.pop();
        return;
    }
    int x = s.top();
    s.pop();
    helper(s, pop - 1);
    s.push(x);
}
void deleteMid(stack<int> &s)
{
    if (s.size() == 0)
        return;
    int n = s.size();
    int mid = n / 2;
    helper(s, mid);
}

// Reverse Stack

// TC O(n)
void insertStack2(stack<int> &s, int x)
{
    if (s.empty())
    {
        s.push(x);
        return;
    }
    int top = s.top();
    s.pop();
    insertStack2(s, x);
    s.push(top);
}

// TC O(n^2)
// SC O(n)
void reverseStack(stack<int> &s)
{
    if (s.size() == 0)
        return;
    int top = s.top();
    s.pop();
    reverseStack(s);
    insertStack2(s, top);
}

// Time Complexity: O(n)
// Space Complexity: O(n) → because of recursion stack
int kthGrammar(int n, int k)
{
    if (n == 0)
        return 0;
    // int mid = pow(2,n-1);
    int mid = 1 << (n - 1);
    if (k <= mid)
        return kthGrammar(n - 1, k);
    else
        return !kthGrammar(n - 1, k - mid);
}

// Time Complexity:2^(n) - 1
// If a recursive function calls itself twice, and reduces problem by 1 each time → O(2ⁿ)
// SC: O(n)
void toh(int n, int source, int dest, int helper, int &count)
{
    count++; // no if moves
    if (n == 1)
    {
        cout << "Move disk " << n << " from " << source << " to " << dest << endl;
        return;
    }
    toh(n - 1, source, helper, dest, count);
    cout << "Move disk " << n << " from " << source << " to " << dest << endl;
    toh(n - 1, helper, dest, source, count);
}

// TC O(n^2)
int josephus(vector<int> &v, int n, int k, int start)
{
    if (n == 1)
    {
        return v[0];
    }
    int i = (start + k) % v.size();
    v.erase(v.begin() + i);
    return josephus(v, n - 1, k, i);
}

// Print all subsets of a string TC: O(2ⁿ  × n ) SC: O(n)
void printStringSubsets(string &ip, string op, size_t i)
{
    if (i == ip.length())
    {
        cout << op << " ";
        return;
    }
    // include
    printStringSubsets(ip, op + ip[i], i + 1);
    // exclude
    printStringSubsets(ip, op, i + 1);
}

// Space Complexity:
// - Auxiliary: O(n)
// - Output: O(2ⁿ × k)
// TC: O(2ⁿ × k)  where k is average subset size (due to push/pop/copying op)
void printUniqueSubsets(vector<int> &ip, size_t i, vector<int> &op, vector<vector<int>> &res)
{
    if (i == ip.size())
    {
        res.push_back(op);
        return;
    }
    int nextIndex = upper_bound(ip.begin() + i, ip.end(), ip[i]) - ip.begin();

    // include
    op.push_back(ip[i]);
    printUniqueSubsets(ip, i + 1, op, res);
    // exclude
    op.pop_back();
    printUniqueSubsets(ip, nextIndex, op, res);
}
// unique subsets for strings
void print2DVector(const vector<vector<int>> &res)
{
    for (const auto &row : res)
    {
        for (int val : row)
        {
            cout << val << " ";
        }
        cout << endl;
    }
}

// Permutation with Spaces
//  Time Complexity = O(2ⁿ × n)
//  Auxiliary Space = O(n)
//  Result Space = O(2ⁿ × n)
void permutationWithSpaces(string &ip, string op, vector<string> &res, int i)
{
    if (i == ip.length())
    {
        cout << op << " ";
        res.push_back(op);
        return;
    }

    // with space
    permutationWithSpaces(ip, op + ' ' + ip[i], res, i + 1);

    // without space
    permutationWithSpaces(ip, op + ip[i], res, i + 1);
}
// Letter Case Permutation
// Time Complexity: O(2^k × n)
// k = number of alphabetic characters in s
// Auxiliary Space: O(n) (max depth of recursion)  Output Space: O(2^k × n)
void letterCasePermute(string &s, string &op, vector<string> &res, int i)
{
    if (i == s.length())
    {
        res.push_back(op);
        return;
    }

    if (isdigit(s[i]))
    {
        op.push_back(s[i]);
        letterCasePermute(s, op, res, i + 1);
        op.pop_back(); // backtrack
    }
    else
    {
        // uppercase branch
        op.push_back(toupper(s[i]));
        letterCasePermute(s, op, res, i + 1);
        op.pop_back();

        // lowercase branch
        op.push_back(tolower(s[i]));
        letterCasePermute(s, op, res, i + 1);
        op.pop_back();
    }
}

// Time Complexity: O(2^n × n)
// Auxiliary Space: O(n)  Output Space: O(2^n × n)
void balancedParantheses(int open, int close, string &s, vector<string> &res)
{

    if (open == 0)
    {
        string temp = s;
        temp.append(close, ')');
        res.push_back(temp);
        cout << temp << " ";
        return;
    }

    if (open > 0)
    {
        s.push_back('(');
        balancedParantheses(open - 1, close, s, res);
        s.pop_back(); // backtrack
    }

    if (open < close)
    {
        s.push_back(')');
        balancedParantheses(open, close - 1, s, res);
        s.pop_back(); // backtrack
    }
}

// TC  O(2ⁿ × n)
// SC O(n)
void NBitBinaryHelper(int ones, int zeros, int n, string &op, vector<string> &res)
{
    if (op.length() == n)
    {
        res.push_back(op);
        return;
    }

    // Always can place '1'
    op.push_back('1');
    NBitBinaryHelper(ones + 1, zeros, n, op, res);
    op.pop_back();

    // Place '0' only if ones > zeros
    if (ones > zeros)
    {
        op.push_back('0');
        NBitBinaryHelper(ones, zeros + 1, n, op, res);
        op.pop_back();
    }
}

// can be done using DP
bool isSubsetSum(vector<int> arr, int target)
{
    return subsetSum(arr, 0, target, 0);
}
bool subsetSum(vector<int> &arr, int i, int target, int sum)
{
    if (sum == target || i == arr.size())
    {
        cout << sum << " ";
        return sum == target;
    }
    return subsetSum(arr, i + 1, target, sum + arr[i]) || subsetSum(arr, i + 1, target, sum);
}

// TC O(2^n * k), For each of the 2n subsequences, 
// storing takes O(k) time where k is the average length of each combination.
void combinationSum1(vector<vector<int>> &res, vector<int> &temp, vector<int> &nums,
                     int target, int i, int currsum)
{
    if (currsum == target)
    {
        res.push_back(temp);
        return;
    }
    if (i == nums.size())
    {
        return;
    }

    if (nums[i] + currsum <= target)
    {
        temp.push_back(nums[i]);
        currsum += nums[i];
        combinationSum1(res, temp, nums, target, i, currsum);
        temp.pop_back();
        currsum -= nums[i];
        combinationSum1(res, temp, nums, target, i + 1, currsum);
    }
}


// TC O(2^n * k), For each of the 2n subsequences, 
// storing takes O(k) time where k is the average length of each combination.
void combinationSum2(vector<vector<int>> &res, vector<int> &temp, vector<int> &nums,
                     int target, int i, int currsum)
{
    if (currsum == target)
    {
        res.push_back(temp);
        return;
    }
    if (i == nums.size())
    {
        return;
    }

    if (nums[i] + currsum <= target)
    {
        temp.push_back(nums[i]);
        currsum += nums[i];
        combinationSum2(res, temp, nums, target, i + 1, currsum);
        temp.pop_back();
        currsum -= nums[i];
        int idx = upper_bound(nums.begin() + i, nums.end(), nums[i]) -
                  nums.begin();
        combinationSum2(res, temp, nums, target, idx, currsum);
    }
}

int main()
{

    //* Sort array stack using recursion
    // vector<int>arr = {1,4,5,6,7,2,0,3};
    // for(auto it:arr)cout<<it<<" ";
    // sortArray(arr);
    // cout<<endl<<"sorted ";
    // for(auto it:arr)cout<<it<<" ";
    // stack<int>st;
    // for(auto it:arr)
    // st.push(it);
    // printStack(st);
    // sortStack(st);
    // printStack(st);
    // deleteMid(st);
    // printStack(st);

    //* Print subsets
    // string s = "abc";
    // printStringSubsets(s,"",0);
    // vector<int>ip = {1,2,2,2};
    // sort(ip.begin(),ip.end());
    // vector<vector<int>>res;
    // vector<int>temp;
    // printUniqueSubsets(ip,0,temp,res);
    // print2DVector(res);

    //* Permutation with spaces
    // string s = "abc";
    // string op;
    // op += s[0];
    // vector<string> res;
    // int i = 1;
    // permutationWithSpaces(s, op, res, i);
    // letterCasePermute(s, op, res, 0);

    // string parantheses;
    // balancedParantheses(3, 3, parantheses, res);

    // int ones = 0;
    // int zeros = 0;
    // int x = 4;
    // NBitBinaryHelper(ones, zeros, x, op, res);

    // int toh_count=0;
    // toh(3,1,3,2,toh_count);
    // cout<<toh_count;
    // int n = 5;
    // int k = 3;
    // vector<int> v;
    // for (int i = 1; i <= n; i++)
    // {
    //     v.push_back(i);
    // }
    // cout<<josephus(v, n, k - 1, 0);

    if (isSubsetSum({1, 2, 3}, 5))
    {
        cout << "true";
    }
    return 0;
}