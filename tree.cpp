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
#include <string>
#include <sstream>

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    // Always use initializer lists for constructors instead of assignment inside constructor body
};

// ! nullptr vs NULL
// In C++ nullptr is literally an integer constant 0.
// void f(int);
// void f(void*);

// f(NULL);   // calls f(int) because NULL = 0
// f(nullptr);  // calls f(void*) correctly

/*
//! Initializer List vs initialize inside constructor body
class Person {
    const string name;  // Immutable after construction
    int age;
    public:
    Person(string n, int a) {
    name = n;  //! Error: cannot assign to const member
    age = a; //!age is default-initialized (indeterminate) then assigned 10, extra work, slower.
    }
    Person(string n, int a) : name(n), age(a) {}
};
*/
// Const data members have no default constructor value.
// If not initialized in the initializer list → compile-time error.
// Initializer List runs before body
// Assignment in Body runs after members default-constructed

void inOrder(TreeNode *root)
{
    if (!root)
        return;
    inOrder(root->left);
    cout << root->val << " ";
    inOrder(root->right);
}
TreeNode *root;
void createTree()
{
    int x;
    cout << "enter root ";
    cin >> x;
    if (x == -1)
    {
        root = nullptr;
        return;
    }
    root = new TreeNode(x);
    queue<TreeNode *> q;
    q.push(root);
    while (!q.empty())
    {
        TreeNode *curr = q.front();
        q.pop();
        int nextTreeNode;
        cout << "enter left of " << curr->val << " ";
        cin >> nextTreeNode;
        if (nextTreeNode != -1)
        {
            curr->left = new TreeNode(nextTreeNode);
            q.push(curr->left);
        }
        cout << "enter right of " << curr->val << " ";
        cin >> nextTreeNode;
        if (nextTreeNode != -1)
        {
            curr->right = new TreeNode(nextTreeNode);
            q.push(curr->right);
        }
    }
}

// ===============================
//! Iterative INORDER (Left, Node, Right)
// ===============================
// !Push left chain, process node, then go right.
vector<int> inorderIter(TreeNode *root)
{
    vector<int> res;
    stack<TreeNode *> st;
    TreeNode *cur = root;

    // Intuition: go as left as possible, process node, then go right
    while (cur || !st.empty())
    {
        while (cur)
        { // keep pushing left children
            st.push(cur);
            cur = cur->left;
        }
        cur = st.top();
        st.pop(); // visit node
        res.push_back(cur->val);
        cur = cur->right; // then explore right subtree
    }
    return res;
}
// TC: O(n) — each node visited once
// SC: O(h) — stack height (worst O(n))

// ===============================
//! Iterative PREORDER (Node, Left, Right)
// ===============================
// !Visit node first, then push right,
// !then left so left is processed before right.
vector<int> preorderIter2(TreeNode *root)
{
    vector<int> res;
    if (!root)
        return res;

    stack<TreeNode *> st;
    st.push(root);

    // Intuition:
    // Preorder = Node → Left → Right
    // Stack is LIFO, so push RIGHT first, LEFT second,
    // ensuring LEFT is processed before RIGHT.
    while (!st.empty())
    {
        TreeNode *node = st.top();
        st.pop();

        res.push_back(node->val); // visit node

        if (node->right)
            st.push(node->right); // push right first
        if (node->left)
            st.push(node->left); // left processed first
    }

    return res;
}

// TC: O(n) — each node visited once
// SC: O(h) — stack height (worst O(n), best O(log n))

vector<int> preorderIter(TreeNode *root)
{
    vector<int> res;
    if (!root)
        return res;
    stack<TreeNode *> st;

    TreeNode *node = root;
    while (node || !st.empty())
    {
        while (node)
        {
            res.push_back(node->val);
            st.push(node);
            node = node->left;
        }
        node = st.top();
        st.pop();
        node = node->right;
    }
    return res;
}

// TC: O(n)
// SC: O(h)

// ===============================
//! Iterative POSTORDER (Left, Right, Node)
// ===============================
// !Use a stack + prev to detect when both children have been processed
// !before visiting the node.
vector<int> postorderTraversal(TreeNode *root)
{
    if (!root)
        return {};
    vector<int> res;
    stack<TreeNode *> st;
    TreeNode *node = root;
    TreeNode *prev = nullptr; // last processed node

    // Intuition:
    // 1. Go left as far as possible
    // 2. If right child not visited, go right
    // 3. Otherwise process node
    while (node || !st.empty())
    {
        while (node)
        { // go left as far as possible
            st.push(node);
            node = node->left;
        }

        node = st.top();

        // If right subtree exists and has not been processed, go right
        if (node->right && node->right != prev)
        {
            node = node->right;
        }
        else
        {
            res.push_back(node->val); // visit node when left & right done
            st.pop();
            prev = node;    // mark as processed
            node = nullptr; // prevent revisiting left
        }
    }
    return res;
}
// TC: O(n)
// SC: O(h)

vector<vector<int>> levelOrder(TreeNode *root)
{
    vector<vector<int>> res;
    if (!root)
        return res;

    queue<TreeNode *> q;
    q.push(root);

    // Intuition: BFS → process nodes level by level using a queue.
    // For each level, pop 'size' nodes, record values, and push their children.

    while (!q.empty())
    {
        int sz = q.size(); // number of nodes in current level
        vector<int> level;

        for (int i = 0; i < sz; i++)
        {
            TreeNode *node = q.front();
            q.pop();
            level.push_back(node->val);

            if (node->left)
                q.push(node->left);
            if (node->right)
                q.push(node->right);
        }

        res.push_back(level);
    }

    return res;
}

// TC: O(n)     — each node is visited once.
// SC: O(n)     — queue holds up to n nodes in worst case (last level).

// ! Left view Right view
vector<int> rightSideView(TreeNode *root)
{
    if (!root)
        return {};
    vector<int> res;
    queue<TreeNode *> q;
    q.push(root);
    while (!q.empty())
    {
        int n = q.size();
        for (int i = 0; i < n; i++)
        {
            TreeNode *temp = q.front();
            q.pop();
            if (i == n - 1)
                res.push_back(temp->val);
            if (temp->left)
                q.push(temp->left);
            if (temp->right)
                q.push(temp->right);
        }
    }
    return res;
}
// Time: O(n) — every node visited once
// Space: O(n) — queue stores up to one full level

// ! Top or Bottom view
vector<int> topView(TreeNode *root)
{
    if (!root)
        return {};
    vector<int> res;
    queue<pair<int, TreeNode *>> q; // col, node
    map<int, int> m;
    q.push({0, root});
    while (!q.empty())
    {
        int y = q.front().first;
        TreeNode *t = q.front().second;
        q.pop();
        if (m.find(y) == m.end())
            m[y] = t->val;
        if (t->left)
            q.push({y - 1, t->left});
        if (t->right)
            q.push({y + 1, t->right});
    }
    for (auto it : m)
    {
        res.push_back(it.second);
    }
    return res;
}
// Time: O(n log n)  — map insertions
// Space: O(n)       — queue + map

// ! Vertical Order Traversal
// TC: O(N log N)  // map + multiset insertions take logN per node
// SC: O(N)        // storing map, queue, and result
vector<vector<int>> verticalTraversal(TreeNode *root)
{
    vector<vector<int>> res;
    if (!root)
        return res;
    queue<pair<TreeNode *, pair<int, int>>> q; // node col-row
    map<int, map<int, multiset<int>>> m;       // col
    q.push({root, {0, 0}});
    while (!q.empty())
    {
        auto [node, cr] = q.front();
        auto [col, row] = cr;
        q.pop();
        m[col][row].insert(node->val);
        if (node->left)
            q.push({node->left, {col - 1, row + 1}});
        if (node->right)
            q.push({node->right, {col + 1, row + 1}});
    }

    for (auto &it : m)
    {
        vector<int> t;
        for (auto &it2 : it.second)
        {
            t.insert(t.end(), it2.second.begin(), it2.second.end());
        }
        res.push_back(t);
    }
    return res;
}

// !width of binary tree
int widthOfBinaryTree(TreeNode *root)
{
    if (!root)
        return 0;
    queue<pair<TreeNode *, long long>> q;
    q.push({root, 1});
    long long width = 0;
    while (!q.empty())
    {
        int n = q.size();
        long long start, end = 0;
        long long base = q.front().second;
        for (int i = 0; i < n; i++)
        {
            auto [node, label] = q.front();
            q.pop();
            label -= base;
            if (i == 0)
                start = label;
            if (i == n - 1)
                end = label;
            if (node->left)
                q.push({node->left, (long long)2 * label});
            if (node->right)
                q.push({node->right, (long long)2 * label + 1});
        }
        width = max(width, end - start + 1);
    }
    return width;
}
// TC: O(N)  -- BFS visits each node once
// SC: O(N)  -- queue stores up to one full level

// !zig zag traversal
vector<vector<int>> zigzagLevelOrder(TreeNode *root)
{
    vector<vector<int>> res;
    if (!root)
        return res;
    queue<TreeNode *> q;
    q.push(root);
    bool even = true; // 0
    while (!q.empty())
    {
        int n = q.size();
        vector<int> temp(n);
        for (int i = 0; i < n; i++)
        {
            auto node = q.front();
            q.pop();
            if (even)
                temp[i] = node->val;
            else
                temp[n - 1 - i] = node->val;
            if (node->left)
                q.push(node->left);
            if (node->right)
                q.push(node->right);
        }
        even = !even;
        res.push_back(temp);
    }
    return res;
}
// TC: O(N)  // each node processed once
// SC: O(N)  // queue + result storage

// !Binary Tree Paths
// Given the root of a binary tree, return all root-to-leaf paths in any order.
void getPath(TreeNode *node, string s, vector<string> &v)
{
    if (!node)
        return;
    s += to_string(node->val);
    if (!node->left && !node->right)
    {
        v.push_back(s);
        return;
    }
    getPath(node->left, s + "->", v);
    getPath(node->right, s + "->", v);
    // ! no backtrack required as pass by value
    return;
}
// TC: O(N·H) – each node visited once, path construction costs O(H)
// SC: O(H)   – recursion depth + path string

// !Root to node path
bool getPath(TreeNode *root, int target, vector<int> &path)
{
    if (!root)
        return false;

    path.push_back(root->val); // include current node

    if (root->val == target)
        return true; // target found

    // search left or right
    if (getPath(root->left, target, path) ||
        getPath(root->right, target, path))
        return true;

    path.pop_back(); // backtrack (remove current)
    return false;
}
// TC = O(N), SC = O(H)

// !Height of bt
// TC = O(N), SC = O(H) recursion stack
int maxDepth(TreeNode *root)
{
    if (!root)
        return 0;
    int lh = maxDepth(root->left);
    int rh = maxDepth(root->right);
    return 1 + max(lh, rh);
}
// !diameter of bt
int height(TreeNode *root, int &w)
{
    if (!root)
        return 0;
    int lh = height(root->left, w);
    int rh = height(root->right, w);
    int width = lh + rh;
    w = max(width, w);
    return 1 + max(lh, rh);
}
int diameterOfBinaryTree(TreeNode *root)
{
    int res = 0;
    height(root, res);
    return res;
}
// TC O(N) Each node is visited once
// SC O(H) recursion stack Worst case (skewed tree): O(N), Balanced tree: O(log N)

// !identical tree
bool isSameTree(TreeNode *root1, TreeNode *root2)
{
    if (!root1 && !root2)
        return true;
    if (!root1 || !root2)
        return false;
    if (root1->val != root2->val)
        return false;

    return isSameTree(root1->left, root2->left) &&
           isSameTree(root1->right, root2->right);

    // queue<TreeNode *> qu;
    // qu.push(p);
    // qu.push(q);

    // while (!qu.empty())
    // {
    //     TreeNode *a = qu.front();
    //     qu.pop();
    //     TreeNode *b = qu.front();
    //     qu.pop();

    //     if (!a && !b)
    //         continue;
    //     if (!a || !b)
    //         return false;
    //     if (a->val != b->val)
    //         return false;

    //     qu.push(a->left);
    //     qu.push(b->left);
    //     qu.push(a->right);
    //     qu.push(b->right);
    // }
    // return true;
}
// TC O(N) Each node is visited once
// SC O(H) recursion stack Worst case (skewed tree): O(N), Balanced tree: O(log N)

// ! symmetric tree
bool isSymmetric(TreeNode *a, TreeNode *b)
{
    if (!a && !b)
        return true;
    if (!a || !b)
        return false;
    if (a->val != b->val)
        return false;
    return isSymmetric(a->left, b->right) && isSymmetric(a->right, b->left);
}
bool isSymmetric(TreeNode *root)
{
    return isSymmetric(root->left, root->right);

    queue<TreeNode *> q;
    q.push(root->left);
    q.push(root->right);

    while (!q.empty())
    {
        TreeNode *a = q.front();
        q.pop();
        TreeNode *b = q.front();
        q.pop();

        if (!a && !b)
            continue;
        if (!a || !b)
            return false;
        if (a->val != b->val)
            return false;

        // mirror order
        q.push(a->left);
        q.push(b->right);
        q.push(a->right);
        q.push(b->left);
    }
    return true;
}
// Time Complexity: O(N) — each node is compared once
// Space Complexity: O(H) — recursion stack, H = height of tree

// !child sum parent
bool isSumProperty(TreeNode *root)
{
    // Null or leaf node always satisfies the property
    if (!root || isLeaf(root))
        return true;

    int childSum = 0;
    if (root->left)
        childSum += root->left->val;
    if (root->right)
        childSum += root->right->val;

    // Check current node + recurse
    return (root->val == childSum) &&
           isSumProperty(root->left) &&
           isSumProperty(root->right);

    // queue<TreeNode *> q;
    // q.push(root);

    // while (!q.empty())
    // {
    //     TreeNode *node = q.front();
    //     q.pop();

    //     // Skip leaf nodes
    //     if (!node->left && !node->right)
    //         continue;

    //     int childSum = 0;
    //     if (node->left)
    //     {
    //         childSum += node->left->val;
    //         q.push(node->left);
    //     }
    //     if (node->right)
    //     {
    //         childSum += node->right->val;
    //         q.push(node->right);
    //     }

    //     if (node->val != childSum)
    //         return false;
    // }
    // return true;
}
// Time Complexity: O(N)
// Space Complexity: O(H)  (recursion stack) or O(N) queue

// ! LCA-1 both node exist
TreeNode *lowestCommonAncestor1(TreeNode *root, TreeNode *p, TreeNode *q)
{
    if (!root)
        return nullptr;
    if (root == p || root == q)
        return root; // if itself matches, return itself
    TreeNode *left = lowestCommonAncestor1(root->left, p, q);
    TreeNode *right = lowestCommonAncestor1(root->right, p, q);
    if (left && right)
        return root;
    if (left)
        return left;
    if (right)
        return right;
    return nullptr;
}
// TC O(N) Each node is visited once
// SC O(H) recursion stack Worst case (skewed tree): O(N), Balanced tree: O(log N)

// ! LCA-2 if one or both node missing
TreeNode *lca = nullptr;
bool dfs(TreeNode *root, TreeNode *p, TreeNode *q)
{
    if (!root)
        return false;
    bool left = dfs(root->left, p, q);
    bool right = dfs(root->right, p, q);
    bool itself = (root == p || root == q);

    if (left + right + itself >= 2)
        lca = root;
    // case: One node in left subtree and one in right subtree
    // case: One node is X itself, the other is in a subtree
    // in both cases current node X is LCA

    return {left || right || itself};
}
TreeNode *lowestCommonAncestor2(TreeNode *root, TreeNode *p, TreeNode *q)
{
    dfs(root, p, q);
    return lca;
}
// TC O(N) Each node is visited once
// SC recursion stack O(H) Worst case (skewed tree): O(N), Balanced tree: O(log N)

// ! LCA-3 iteratively
TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q)
{
    if (!root)
        return nullptr;

    unordered_map<TreeNode *, TreeNode *> parent;
    queue<TreeNode *> qu;

    parent[root] = nullptr;
    qu.push(root);

    // Build parent map
    while (!qu.empty() && (!parent.count(p) || !parent.count(q)))
    {
        TreeNode *node = qu.front();
        qu.pop();

        if (node->left)
        {
            parent[node->left] = node;
            qu.push(node->left);
        }
        if (node->right)
        {
            parent[node->right] = node;
            qu.push(node->right);
        }
    }

    // Missing-node check
    if (!parent.count(p) || !parent.count(q))
        return nullptr;

    unordered_set<TreeNode *> ancestors;

    // Store ancestors of p
    while (p)
    {
        ancestors.insert(p);
        p = parent[p];
    }

    // Find LCA
    while (q)
    {
        if (ancestors.count(q))
            return q;
        q = parent[q];
    }

    return nullptr;
}
// Time Complexity:
// BFS traversal        → O(N)
// Ancestor traversal   → O(H)
// Overall              → O(N)

// Space Complexity:
// Parent map + queue   → O(N)
// Ancestor set         → O(H)
// Overall              → O(N)

// ! LCA-4 LCA with Parent Pointer
TreeNode *lowestCommonAncestor(TreeNode *p, TreeNode *q)
{
    // unordered_set<TreeNode *> ancestors;

    // // Store ancestors of p
    // while (p)
    // {
    //     ancestors.insert(p);
    //     p = p->parent;
    // }

    // // Move q upwards
    // while (q)
    // {
    //     if (ancestors.count(q))
    //         return q;
    //     q = q->parent;
    // }

    // return nullptr;

    // !it becomes intersection of LLs problem
    TreeNode *a = p;
    TreeNode *b = q;

    while (a != b)
    {
        a = a ? a->parent : q;
        b = b ? b->parent : p;
    }
    // if no intersection, both will meet at root->parent is nullptr
    return a;
}
// TC O(H) Each pointer traverses at most 2× height
// SC O(1) two pointers

// ! boundary traversal
bool isLeaf(TreeNode *node)
{
    return node && !node->left && !node->right;
}

// Add left boundary excluding leaf nodes
void addLeftBoundary(TreeNode *node, vector<int> &leftBoundary)
{
    while (node)
    {
        if (!isLeaf(node))
            leftBoundary.push_back(node->val);

        if (node->left)
            node = node->left;
        else
            node = node->right;
    }
}

// Add right boundary excluding leaf nodes
void addRightBoundary(TreeNode *node, vector<int> &rightBoundary)
{
    while (node)
    {
        if (!isLeaf(node))
            rightBoundary.push_back(node->val);

        if (node->right)
            node = node->right;
        else
            node = node->left;
    }
}

// Add all leaf nodes (in-order traversal)
void addLeafNodes(TreeNode *node, vector<int> &leafNodes)
{
    if (!node)
        return;

    if (isLeaf(node))
    {
        leafNodes.push_back(node->val);
        return;
    }

    addLeafNodes(node->left, leafNodes);
    addLeafNodes(node->right, leafNodes);
}

vector<int> boundaryTraversal(TreeNode *root)
{
    if (!root)
        return {};

    // Single-node tree
    if (isLeaf(root))
        return {root->val};

    vector<int> leftBoundary;
    vector<int> rightBoundary;
    vector<int> leafNodes;
    vector<int> result;

    // Root
    result.push_back(root->val);

    // Boundaries and leaves
    addLeftBoundary(root->left, leftBoundary);
    addLeafNodes(root, leafNodes);
    addRightBoundary(root->right, rightBoundary);

    // Combine result
    result.insert(result.end(), leftBoundary.begin(), leftBoundary.end());
    result.insert(result.end(), leafNodes.begin(), leafNodes.end());

    reverse(rightBoundary.begin(), rightBoundary.end());
    result.insert(result.end(), rightBoundary.begin(), rightBoundary.end());

    return result;
}
// TC: O(N) — leaf traversal visits all nodes, boundaries at most H nodes
// SC: O(H) recursion stack + O(N) output vector

// !max path sum
int sum(TreeNode *node, int &res)
{
    if (!node)
        return 0;
    int l = sum(node->left, res);
    int r = sum(node->right, res);
    res = max(res, l + r + node->val);
    return max(max(l, r) + node->val, 0);
}
// At each node, I compute the best downward path to parent
// and update the global maximum using a path that passes through the node
int maxPathSum(TreeNode *root)
{
    int res = INT_MIN;
    sum(root, res);
    return res;
}
// Time Complexity: O(N) — each node is visited exactly once
// Space Complexity: O(H) — recursion stack, H = height of tree

// ! tree from inorder preorder
/*
Tree A:
    1
   /
  2
   \
    3

Tree B:
    1
     \
      2
     /
    3

Preorder (Root, Left, Right):
Tree A: 1 2 3
Tree B: 1 2 3

Postorder (Left, Right, Root):
Tree A: 3 2 1
Tree B: 3 2 1

=> SAME preorder and postorder
=> DIFFERENT trees

Conclusion:
Preorder + Postorder is NOT sufficient to uniquely build a binary tree.

Preorder (Root, L, R)	Who is the root
Postorder (L, R, Root)	Who is the root
Inorder (L, Root, R)	What goes left and what goes right
*/

/*
Input:
preorder = [3, 9, 20, 15, 7]
inorder  = [9, 3, 15, 20, 7]

Step 1:
root = 3

inorder:
[9 | 3 | 15 20 7]
leftSize = 1

leftIn  = [9]
rightIn = [15 20 7]

pre: A B C D E F G
 if 2 elements in left st and 4 in right st, current node A, left side
 pre (B C) right side: skip 2, then all: (D E F G)
preorder:
[3 | 9 | 20 15 7]

leftPre  = [9]
rightPre = [20 15 7]

---------------------------------
Left subtree:
root = 9
leftIn = []
rightIn = []
→ leaf node

---------------------------------
Right subtree:
root = 20

inorder:
[15 | 20 | 7]

leftIn  = [15]
rightIn = [7]

leftPre  = [15]
rightPre = [7]

---------------------------------
Final Tree:

        3
       / \
      9   20
         /  \
        15   7
*/

TreeNode *solve(vector<int> inorder, vector<int> preorder)
{

    // Base case:
    // If there are no nodes in this inorder segment,
    // it means this subtree is empty.
    if (inorder.empty())
        return nullptr;

    // Step 1:
    // The first element of preorder is always the ROOT
    // of the current subtree.
    int rootVal = preorder[0];
    TreeNode *root = new TreeNode(rootVal);

    // Step 2:
    // Find root position in inorder traversal.
    // Left of this index -> left subtree
    // Right of this index -> right subtree
    auto it = find(inorder.begin(), inorder.end(), rootVal);
    int leftSize = it - inorder.begin(); // number of nodes in left subtree

    // Step 3:
    // Split inorder into left and right subtrees
    vector<int> leftIn(inorder.begin(), inorder.begin() + leftSize);
    vector<int> rightIn(inorder.begin() + leftSize + 1, inorder.end());

    // Step 4:
    // Split preorder:
    // preorder[0] is root, so skip it
    // Next leftSize elements belong to left subtree
    vector<int> leftPre(preorder.begin() + 1,
                        preorder.begin() + 1 + leftSize);

    // Remaining elements belong to right subtree
    vector<int> rightPre(preorder.begin() + 1 + leftSize,
                         preorder.end());

    // Step 5:
    // Recursively build left and right subtrees
    root->left = solve(leftIn, leftPre);
    root->right = solve(rightIn, rightPre);

    // Step 6:
    // Return constructed subtree root
    return root;
}

TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder)
{
    // Build tree using preorder and inorder traversals
    return solve(inorder, preorder);
}
// One call per node → n recursion calls
// TC = n * (find + vector copying)
//    = n * (O(n) + O(n))
//    = O(n²)

// O(n) space for extra vectors at each recursion call, for n calls n² space
// SC = recursion stack + extra vectors
//    = O(n) + O(n²)
//    = O(n²)

// !optimal
TreeNode *buildBinaryTree(vector<int> &pre, unordered_map<int, int> &m, int inStart, int inEnd, int preStart, int preEnd)
{
    // 1. if no elements on this side, return null
    // if (in.size() == 0)
    if (inStart > inEnd || preStart > preEnd)
        return nullptr;

    // 2. find element in inorder
    // auto it = find(in.begin(), in.end(), pre[0]);
    // int i = it - in.begin();
    int i = m[pre[preStart]];

    // 3. 0 to i-1 in left side inorder, i+1 to n in right side inorder
    //  pre: A B C D E F G
    // if 2 elements in left st and 4 in right st, current node A, left side
    // pre (B C) right side: skip 2, then all: DEFG
    TreeNode *node = new TreeNode(pre[preStart]); // current node
    // vector<int> left(in.begin(), in.begin() + i);
    // vector<int> right(in.begin() + i + 1, in.end());
    // vector<int> preLeft(pre.begin() + 1, pre.begin() + i + 1);
    // vector<int> preRight(pre.begin() + i + 1, pre.end());
    //! i-inStart == number of elements on left subtree
    node->left = buildBinaryTree(pre, m, inStart, i - 1, preStart + 1, preStart + i - inStart);
    node->right = buildBinaryTree(pre, m, i + 1, inEnd, preStart + i - inStart + 1, preEnd);
    return node;
}
TreeNode *buildTree2(vector<int> &preorder, vector<int> &inorder)
{
    unordered_map<int, int> m; // ele, index
    for (int i = 0; i < inorder.size(); i++)
    {
        m[inorder[i]] = i;
    }
    return buildBinaryTree(preorder, m, 0, inorder.size() - 1, 0, preorder.size() - 1);
}

// TC = O(1) hashmap lookup × n nodes
//    = O(n)
// SC = HashMap + Recursion stack
//    = O(n) + O(n)
//    = O(n)

// ! Bt from inorder, postorder
// In postorder, the last element is root. Inorder splits left and right.
// We recurse by skipping right subtree size in postorder.
TreeNode *solve(int poststart, int instart, int inend, vector<int> &postorder, unordered_map<int, int> &m)
{
    if (instart > inend)
        return nullptr;
    int i = m[postorder[poststart]];

    int n = inend - i; // ele on right
    TreeNode *node = new TreeNode(postorder[poststart]);
    node->left = solve(poststart - n - 1, instart, i - 1, postorder, m);
    node->right = solve(poststart - 1, i + 1, inend, postorder, m);
    return node;
}
TreeNode *buildTree(vector<int> &inorder, vector<int> &postorder)
{
    unordered_map<int, int> m;
    for (int i = 0; i < inorder.size(); i++)
        m[inorder[i]] = i;
    TreeNode *root = solve(postorder.size() - 1, 0, inorder.size() - 1, postorder, m);
    return root;
}
// TC = O(1) hashmap lookup × n nodes
//    = O(n)
// SC = HashMap + Recursion stack
//    = O(n) + O(n)
//    = O(n)

// ! morris inorder traversal
// Morris Inorder Traversal
// Idea:
// 1. For every node, if left subtree exists, find its inorder predecessor
// 2. Connect predecessor's right to current node (temporary thread)
// 3. This allows us to come back without recursion or stack
// 4. If the thread already exists, it means left subtree is done

vector<int> inorderTraversal(TreeNode *root)
{

    // Result of inorder traversal
    vector<int> res;

    // Start from root
    TreeNode *curr = root;

    while (curr)
    {

        // Case 1: Left subtree exists
        if (curr->left)
        {

            // Find inorder predecessor:
            // Rightmost node in left subtree
            TreeNode *predecessor = curr->left;

            // Move to the rightmost node
            // Stop if right is null OR already points to current node
            while (predecessor->right && predecessor->right != curr)
            {
                predecessor = predecessor->right;
            }

            // Case 1a:
            // If predecessor already points to current,
            // left subtree is fully processed
            if (predecessor->right == curr)
            {

                // Remove the temporary thread
                predecessor->right = nullptr;

                // Visit current node (inorder)
                res.push_back(curr->val);

                // Move to right subtree
                curr = curr->right;
            }
            else
            {
                // Case 1b:
                // First time reaching current node
                // Create a temporary thread to come back later
                predecessor->right = curr;

                // !for preoder print when making thread
                //  res.push_back(curr->val);

                // Move to left child to continue traversal
                curr = curr->left;
            }
        }
        // Case 2: No left subtree
        else
        {
            // Directly visit current node
            res.push_back(curr->val);

            // Move to right subtree
            curr = curr->right;
        }
    }

    return res;
}
// Time  : O(n)
// Space : O(1)

// ! flatten bt to linked list
void flatten(TreeNode *root)
{
    if (!root)
        return;
    TreeNode *temp = root;
    while (root)
    {
        if (root->left)
        {
            TreeNode *suc = root->left;
            while (suc->right)
            {
                suc = suc->right;
            }
            suc->right = root->right;
            root->right = root->left;
            root->left = nullptr;
            root = root->right;
        }
        else
        {
            root = root->right;
        }
    }
    root = temp;
}
// Each node processed once → O(n) time

// ! pre in post in single traversal
/*
INTUITION: One traversal for Preorder, Inorder, Postorder

Think in terms of how recursion visits a node:

1) PREORDER (Root, Left, Right)
   - Node is processed the FIRST time we see it
   - This happens before going to left child

2) INORDER (Left, Root, Right)
   - After finishing left subtree, we come BACK to the node
   - Node is processed the SECOND time

3) POSTORDER (Left, Right, Root)
   - After finishing both left and right subtrees
   - Node is processed the THIRD time

-----------------------------------------
How we simulate this iteratively:

- Use a stack storing (node, state)
- state = 1 → first visit  → PREORDER
- state = 2 → second visit → INORDER
- state = 3 → third visit  → POSTORDER

Each node is pushed back with next state
until all three visits are completed.
-----------------------------------------

This exactly mimics recursive DFS,
but allows collecting all three traversals in one pass.
*/

vector<int> pre, in, post;

void allTraversals(TreeNode *root)
{
    if (!root)
        return;

    // stack holds {node, state}
    // state: 1 = preorder, 2 = inorder, 3 = postorder
    stack<pair<TreeNode *, int>> st;
    st.push({root, 1});

    while (!st.empty())
    {
        auto [node, state] = st.top();
        st.pop();

        if (state == 1)
        {
            // Preorder
            pre.push_back(node->val);

            // Move to inorder state
            st.push({node, 2});

            // Go left
            if (node->left)
                st.push({node->left, 1});
        }
        else if (state == 2)
        {
            // Inorder
            in.push_back(node->val);

            // Move to postorder state
            st.push({node, 3});

            // Go right
            if (node->right)
                st.push({node->right, 1});
        }
        else
        {
            // Postorder
            post.push_back(node->val);
        }
    }
}
// Time  : O(n)
// Space : O(n)  // stack

// ! serialize deserialize bt
// Encodes a tree to a single string (level-order)
string serialize(TreeNode *root)
{
    if (!root)
        return "";

    queue<TreeNode *> q;
    q.push(root);

    string result;

    while (!q.empty())
    {
        TreeNode *node = q.front();
        q.pop();

        if (node)
        {
            result += to_string(node->val) + ",";
            q.push(node->left);
            q.push(node->right);
        }
        else
        {
            result += "N,";
        }
    }

    // Remove trailing comma
    result.pop_back();
    return result;
}

// Decodes your encoded data to tree
TreeNode *deserialize(string data)
{
    if (data.empty())
        return nullptr;

    stringstream ss(data);
    string token;

    // Read root value
    getline(ss, token, ',');
    TreeNode *root = new TreeNode(stoi(token));

    queue<TreeNode *> q;
    q.push(root);

    while (!q.empty())
    {
        TreeNode *node = q.front();
        q.pop();

        // Left child
        if (getline(ss, token, ','))
        {
            if (token != "N")
            {
                node->left = new TreeNode(stoi(token));
                q.push(node->left);
            }
        }

        // Right child
        if (getline(ss, token, ','))
        {
            if (token != "N")
            {
                node->right = new TreeNode(stoi(token));
                q.push(node->right);
            }
        }
    }

    return root;
}
// Time  : O(n)
// Space : O(n)

//! BST search
bool searchBST(TreeNode *root, int key)
{
    while (root)
    {
        if (root->val == key)
            return true;
        else if (key < root->val)
            root = root->left;
        else
            root = root->right;
    }
    return false;
}
// Time  : O(h)
// Space : O(1)
//! BST floor/ceil
int floorBST(TreeNode *root, int key)
{
    int ans = -1;

    while (root)
    {
        if (root->val == key)
            return key;

        if (root->val < key)
        {
            ans = root->val; // possible floor, save when going right
            root = root->right;
        }
        else
        {
            root = root->left;
        }
    }
    return ans;
}
// Time  : O(h)
// Space : O(1)

// ! validate bst
bool isValidBST(TreeNode *root)
{
    TreeNode *curr = root;
    TreeNode *prev = nullptr;
    bool res = true;
    while (curr)
    {
        if (curr->left)
        {
            TreeNode *pre = curr->left;
            while (pre->right && pre->right != curr)
            {
                pre = pre->right;
            }
            if (pre->right == curr)
            {
                // in(curr)
                if (prev && prev->val >= curr->val)
                    res = false;
                prev = curr;
                pre->right = nullptr;
                curr = curr->right;
            }
            else
            {
                pre->right = curr;
                curr = curr->left;
            }
        }
        else
        {
            // in(curr)
            if (prev && prev->val >= curr->val)
                res = false;
            prev = curr;
            curr = curr->right;
        }
    }
    return res;
}
// TC O(n)
// SC O(1)

// ! insert in BST
TreeNode *insertIntoBST(TreeNode *root, int val)
{
    if (!root)
        return new TreeNode(val);
    if (val > root->val)
        root->right = insertIntoBST(root->right, val);
    else
        root->left = insertIntoBST(root->left, val);
    return root;
    // TreeNode *node,*prev;
    // node = root;
    // while(node){
    //     prev=node;
    //     if(node->val < val){
    //         node=node->right;
    //     }
    //     else{
    //         node=node->left;
    //     }
    // }
    // if(val>prev->val) prev->right = new TreeNode(val);
    // else prev->left = new TreeNode(val);
    // return root;
}
// Time  : O(h)
// Space : O(h) recursion stack

// ! delete in BST
TreeNode *deleteNode(TreeNode *root, int key)
{
    if (!root)
        return nullptr;

    // Step 1: search for the node
    if (key < root->val)
    {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->val)
    {
        root->right = deleteNode(root->right, key);
    }
    else
    {
        // -------- node found --------

        // Case 1: leaf node
        if (!root->left && !root->right)
        {
            delete root;
            return nullptr;
        }

        // Case 2: only right child
        if (!root->left)
        {
            TreeNode *temp = root->right;
            delete root;
            return temp;
        }

        // Case 3: only left child
        if (!root->right)
        {
            TreeNode *temp = root->left;
            delete root;
            return temp;
        }
        /*
                !Method-1 Swap
                root->val = suc->val;
                root->right = deleteNode(root->right, suc->val);

                !Method-2 don't swap, attach root's left subtree to succ, increases height
                suc->left = root->left;
                return root->right;
        */
        // !Method-3
        // ! detach the successor from its old parent, attach the root’s children to it
        // Case 4: two children → use inorder successor
        TreeNode *suc = root->right;

        // If successor is immediate right child
        if (!suc->left)
        {
            suc->left = root->left;
            delete root;
            return suc;
        }

        // Otherwise, find leftmost node in right subtree
        TreeNode *parent = root;
        while (suc->left)
        {
            parent = suc;
            suc = suc->left;
        }

        // Detach successor from its old position
        parent->left = suc->right;

        // Attach root's children to successor
        suc->left = root->left;
        suc->right = root->right;

        delete root;
        return suc;
    }

    return root;
}
// Time  : O(h)
// Space : O(h) recursion stack

// ! LCA in BST
TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q)
{
    if (root->val < p->val && root->val < q->val)
        return lowestCommonAncestor(root->right, p, q);
    else if (root->val > p->val && root->val > q->val)
        return lowestCommonAncestor(root->left, p, q);
    return root;
}
// Time  : O(n) visit each node once
// Space : O(h) recursion stack

// !BST two sum
bool findTarget(TreeNode *root, int k)
{
    unordered_set<int> s;
    bool res = false;
    stack<TreeNode *> st;
    while (root || !st.empty())
    {
        while (root)
        {
            st.push(root);
            root = root->left;
        }
        TreeNode *curr = st.top();
        st.pop();
        // in(curr)
        if (s.count(k - curr->val))
            return true;
        s.insert(curr->val);
        root = curr->right;
    }
    return res;
}
// TC O(n)
// SC O(n) unordered_Set

// ! BST Iterator
class BSTIterator
{
    stack<TreeNode *> st;

    void pushAll(TreeNode *node)
    {
        while (node)
        {
            st.push(node);
            node = node->left;
        }
    }

public:
    BSTIterator(TreeNode *root)
    {
        pushAll(root);
    }

    int next()
    {
        TreeNode *node = st.top();
        st.pop();
        pushAll(node->right);
        return node->val;
    }

    bool hasNext() const
    {
        return !st.empty();
    }
};
// Constructor: O(h)
// next(): amortized O(1) Each node is pushed once and popped once.
// Total stack ops = 2n → O(n) Spread across n next() calls → amortized O(1).
// hasNext(): O(1)
// SC: O(h)

// ! two sum using bst iterator
class BSTIterator
{
    stack<TreeNode *> st;
    bool isLeft;

    void pushAll(TreeNode *node)
    {
        while (node)
        {
            st.push(node);
            node = isLeft ? node->left : node->right;
        }
    }

public:
    BSTIterator(TreeNode *root, bool left)
    {
        isLeft = left;
        pushAll(root);
    }

    int next()
    {
        TreeNode *node = st.top();
        st.pop();

        if (isLeft)
            pushAll(node->right);
        else
            pushAll(node->left);

        return node->val;
    }

    bool hasNext()
    {
        return !st.empty();
    }
};

bool findTarget(TreeNode *root, int k)
{
    if (!root)
        return false;

    BSTIterator l(root, true);
    BSTIterator r(root, false);

    int i = l.next();
    int j = r.next();

    while (i < j)
    {
        int sum = i + j;
        if (sum == k)
            return true;
        else if (sum < k)
        {
            if (l.hasNext())
                i = l.next();
            else
                break;
        }
        else
        {
            if (r.hasNext())
                j = r.next();
            else
                break;
        }
    }
    return false;
}
// TC O(N)
// SC O(H)x2

void recoverTree(TreeNode *root)
{
    TreeNode *prev = nullptr;
    TreeNode *first = nullptr;
    TreeNode *second = nullptr;
    TreeNode *curr = root;
    while (curr)
    {
        if (curr->left)
        {
            TreeNode *pre = curr->left;
            while (pre->right && pre->right != curr)
                pre = pre->right;

            if (pre->right == curr)
            {
                // in(curr)
                if (prev && prev->val > curr->val)
                {
                    if (!first)
                        first = prev;
                    second = curr;
                }
                prev = curr;
                pre->right = nullptr;
                curr = curr->right;
            }
            else
            {
                pre->right = curr;
                curr = curr->left;
            }
        }
        else
        {
            // in(Curr)
            if (prev && prev->val > curr->val)
            {
                if (!first)
                    first = prev;
                second = curr;
            }
            prev = curr;
            curr = curr->right;
        }
    }
    // cout<<first->val<<" "<<second->val;
    swap(first->val, second->val);
}
// TC O(n)
// SC O(1)

// ! Inorder to BST
TreeNode *buildBST(vector<int> &nums, int i, int j)
{
    if (i > j)
        return nullptr;
    int mid = i + (j - i + 1) / 2;
    TreeNode *node = new TreeNode(nums[mid]);
    node->left = buildBST(nums, i, mid - 1);
    node->right = buildBST(nums, mid + 1, j);
    return node;
}
TreeNode *sortedArrayToBST(vector<int> &nums)
{
    TreeNode *res = buildBST(nums, 0, nums.size() - 1);
    return res;
}
// TC O(N) Every recursive call creates one node, Each element is used exactly once to create a node.
// SC O(log₂ n) Because you choose the middle every time, the tree is height-balanced ≈ log₂ n

// ! BST from preorder
/*
Create a new node

Pop elements while new_value > stack.top()->val

The last popped node becomes the parent (right child case)

If nothing was popped → new node is left child of stack top

Push new node to stack

The last popped node is the parent for the right child.
If nothing was popped, the node is the left child of stack top.
 */
TreeNode *bstFromPreorder(vector<int> &preorder)
{
    if (preorder.empty())
        return nullptr;

    // Stack will store path of nodes
    stack<TreeNode *> st;

    // First element is always root
    TreeNode *root = new TreeNode(preorder[0]);
    st.push(root);

    // Process remaining elements
    for (int i = 1; i < preorder.size(); i++)
    {
        TreeNode *node = new TreeNode(preorder[i]);
        TreeNode *parent = nullptr;

        /*
            Pop until we find a value greater than current node.
            The last popped node is the parent for RIGHT child.
        */
        while (!st.empty() && node->val > st.top()->val)
        {
            parent = st.top();
            st.pop();
        }

        /*
            Case 1:
            If we popped at least one node,
            current node is RIGHT child of last popped node
        */
        if (parent != nullptr)
        {
            parent->right = node;
        }
        /*
            Case 2:
            Otherwise, current node is LEFT child of stack top
        */
        else
        {
            st.top()->left = node;
        }

        // Push current node for future children
        st.push(node);
    }

    return root;
}
// TC O(n) Each node pushed and popped once
// SC O(h)

// ! Largest bst in bt
struct Info
{
    int sum, mx, mn;
    bool isBST;
};
int ans = 0;
Info largestBST(TreeNode *node)
{
    if (!node)
        return {0, INT_MIN, INT_MAX, true};

    Info l = largestBST(node->left);
    Info r = largestBST(node->right);

    if (l.isBST && r.isBST && node->val < r.mn && node->val > l.mx)
    {
        Info curr;
        curr.isBST = true;
        curr.sum = node->val + l.sum + r.sum;
        curr.mx = max(node->val, r.mx);
        curr.mn = min(node->val, l.mn);
        ans = max(ans, curr.sum);
        return curr;
    }
    else
    {
        return {0, INT_MAX, INT_MIN, false};
    }
}
int maxSumBST(TreeNode *root)
{
    largestBST(root);
    return ans;
}

// Time Complexity: O(N)
// Space Complexity: O(H), where H is height of tree

int main()
{

    createTree();
    inOrder(root);

    return 0;
}