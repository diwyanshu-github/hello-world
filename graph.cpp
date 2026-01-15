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

/*
BFS & DFS Theory:

BFS (Breadth First Search):
- Explores nodes level by level using a queue.
- Useful for shortest path in unweighted graphs.
- Time  : O(V + E)
- Space : O(V)  (queue + visited array)

DFS (Depth First Search):
- Explores as deep as possible before backtracking.
- Implemented using recursion or an explicit stack.
- Useful for cycle detection, connected components, topological sort.
- Time  : O(V + E)
- Space : O(V)  (recursion stack or stack + visited)

Notes:
- V = number of vertices
- E = number of edges
- Each vertex and edge is processed once
- Visited array prevents infinite loops in cyclic graphs
*/

/*

 DFS TIME COMPLEXITY EXPLANATION (BLUEPRINT)

/* ---------- Adjacency LIST ----------

for (int i = 0; i < V; i++) {        // O(V) → each vertex checked once
    if (!vis[i]) dfs(i);
}

void dfs(int node) {
    vis[node] = true;               // O(1), done once per node → O(V)

    for (int nei : adj[node]) {     // Iterates only actual neighbors
        if (!vis[nei]) {
            dfs(nei);
        }
    }
}

• Each vertex is visited ONCE        → O(V)
• Each edge is traversed ONCE        → O(E)
• Total Time Complexity              → O(V + E)

----------------------------------------------------

 ---------- Adjacency MATRIX ----------

for (int i = 0; i < V; i++) {        // O(V)
    if (!vis[i]) dfs(i);
}

void dfs(int node) {
    vis[node] = true;

    for (int j = 0; j < V; j++) {    // Scans ALL possible neighbors
        if (matrix[node][j] == 1 && !vis[j]) {
            dfs(j);
        }
    }
}

• Each DFS call scans V columns      → O(V)
• DFS can be called for V nodes      → O(V)
• Total Time Complexity              → O(V * V) = O(V²)

----------------------------------------------------

 Key Insight:
 Adjacency list → loops over EXISTING edges
 Adjacency matrix → loops over ALL POSSIBLE edges

*/

// ! bfs
vector<int> bfs(vector<vector<int>> &adj)
{
    int n = adj.size();
    vector<bool> vis(n, false);
    vector<int> res;
    queue<int> q;

    for (int start = 0; start < n; start++)
    {
        if (vis[start])
            continue;

        q.push(start);
        vis[start] = true;

        while (!q.empty())
        {
            int node = q.front();
            q.pop();
            res.push_back(node); // process

            for (int nei : adj[node])
            {
                if (!vis[nei])
                {
                    vis[nei] = true; // mark visited before pushing in queue
                    q.push(nei);
                }
            }
        }
    }
    return res;
}
// ! rotten oranges
// Time Complexity: O(n * m)
// Each cell is processed at most once in BFS.

// Space Complexity: O(n * m)
// Queue can hold all cells in the worst case.

int orangesRotting(vector<vector<int>> &grid)
{

    /*
    Intuition:
    - Use multi-source BFS starting from all initially rotten oranges.
    - Each BFS level represents 1 minute.
    - Count fresh oranges initially.
    - Whenever a fresh orange becomes rotten, decrement freshCount.
    - IMPORTANT: mark orange as rotten WHEN PUSHING into queue
      to avoid multiple enqueues and incorrect time calculation.
    */

    int n = grid.size();
    int m = grid[0].size();

    queue<pair<int, int>> q;
    int freshCount = 0;

    // Initialization: push all rotten oranges and count fresh ones
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (grid[i][j] == 2)
            {
                q.push({i, j});
            }
            else if (grid[i][j] == 1)
            {
                freshCount++;
            }
        }
    }

    // If there are no fresh oranges, no time is needed
    if (freshCount == 0)
        return 0;

    int minutes = 0;

    // Multi-source BFS
    while (!q.empty())
    {
        int sz = q.size();

        for (int i = 0; i < sz; i++)
        {
            auto [r, c] = q.front();
            q.pop();

            for (int d = 0; d < 4; d++)
            {
                int nr = r + dx[d];
                int nc = c + dy[d];

                if (nr < 0 || nc < 0 || nr >= n || nc >= m)
                    continue;

                if (grid[nr][nc] == 1)
                {
                    // Mark visited at PUSH time to prevent duplicate enqueues
                    grid[nr][nc] = 2;
                    freshCount--;
                    q.push({nr, nc});
                }
            }
        }

        minutes++;

        // If fresh oranges remain, it's impossible to rot all
        return freshCount == 0 ? minutes - 1 : -1;
    }
}

// ! dfs
void solveDFS(vector<vector<int>> &adj, vector<bool> &vis, int i, vector<int> &res)
{
    vis[i] = true;    // mark visited after pushing in stack
    res.push_back(i); // process
    for (int nei : adj[i])
    {
        if (!vis[nei])
            solveDFS(adj, vis, nei, res); // push to stack
    }
}

// BACKTRACKING DFS (explore ALL paths)
// visited[] means "used in current path", NOT global
// After returning, we unmark visited to allow other paths

// Example:
// 4 → {1,2,3}
// 5 → {3}
//
// Path 1: 4 → 3  (visited[3] = true, then reset)
// Path 2: 5 → 3  (visited[3] = false again → allowed)
//
// This is why backtracking covers multiple paths to same node

vector<int> dfs(vector<vector<int>> &adj)
{
    // Code here
    int n = adj.size();
    vector<bool> vis(n, false);
    vector<int> res;
    for (int i = 0; i < n; i++)
    {
        if (!vis[i])
        {
            solveDFS(adj, vis, i, res);
        }
    }
    return res;
}

vector<int> dfsIterative(vector<vector<int>> &adj)
{
    int n = adj.size();
    vector<bool> vis(n, false);
    vector<int> res;

    for (int i = 0; i < n; i++)
    {
        if (vis[i])
            continue;

        stack<int> st;
        st.push(i);

        while (!st.empty())
        {
            int node = st.top();
            st.pop();

            if (vis[node])
                continue;
            vis[node] = true;
            // visit when popping to match recursive dfs ordering
            res.push_back(node);

            // push neighbors in reverse to mimic recursive DFS
            for (int j = adj[node].size() - 1; j >= 0; j--)
            {
                int nei = adj[node][j];
                if (!vis[nei])
                {
                    st.push(nei);
                }
            }
        }
    }
    return res;
}

// ! number of islands
// Time: O(n × m)
// Each cell visited once
// Space: O(n × m) worst-case recursion stack
int dx[4] = {0, -1, 1, 0};
int dy[4] = {1, 0, 0, -1};

void dfs(vector<vector<char>> &grid, int row, int col)
{
    grid[row][col] = '0';
    // for all nei of this node up,down,left,right
    for (int i = 0; i < 4; i++)
    {
        int newR = row + dx[i];
        int newC = col + dy[i];
        if (newR < 0 || newC < 0 || newR > grid.size() - 1 || newC > grid[0].size() - 1)
            continue;
        if (grid[newR][newC] != '0')
        {
            dfs(grid, newR, newC);
        }
    }
}
int numIslands(vector<vector<char>> &grid)
{
    int n = grid.size();    // n rows
    int m = grid[0].size(); // m cols

    int count = 0;
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < m; col++)
        {
            if (grid[row][col] == '1')
            {
                dfs(grid, row, col);
                count++;
            }
        }
    }
    return count;
}
/*
!Cycle Detection in Undirected Graph (BFS)

Approach:
1. Use BFS with (node, parent)
2. If a visited neighbor is NOT the parent → cycle exists

Time Complexity:
- O(V + E)
  Each vertex and edge is processed once.

Space Complexity:
- O(V)
  Used for visited array + queue + adjacency list
*/

bool hasCycle(int n, vector<vector<int>> &edges)
{
    // Build adjacency list
    vector<vector<int>> adj(n);
    for (auto &e : edges)
    {
        // We use references to avoid copying large objects;
        // for primitive types like int, copying is cheap so & isn’t needed
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]); // undirected
    }

    vector<bool> vis(n, false);
    queue<pair<int, int>> q; // {node, parent}

    for (int i = 0; i < n; i++)
    {
        if (vis[i])
            continue;

        vis[i] = true;
        q.push({i, -1});

        while (!q.empty())
        {
            auto [node, parent] = q.front();
            q.pop();

            for (int nei : adj[node])
            {
                if (!vis[nei])
                {
                    vis[nei] = true;
                    q.push({nei, node});
                }
                else if (nei != parent)
                {
                    return true; // cycle found
                }
            }
        }
    }
    return false;
}

/*
!Cycle Detection in an Undirected Graph using DFS

Approach:
- Use DFS with (node, parent)

Time Complexity:
- O(V + E)
  Each vertex and edge is processed once.

Space Complexity:
- O(V)
  Used for visited array + recursion stack.
*/

bool hasCycle(int node, int parent, vector<bool> &vis, vector<vector<int>> &adj)
{
    vis[node] = true;

    for (int nei : adj[node])
    {
        if (!vis[nei])
        {
            if (hasCycle(nei, node, vis, adj))
                return true;
        }
        else if (nei != parent)
        {
            return true;
        }
    }
    return false;
}

bool isCycle(int V, vector<vector<int>> &edges)
{
    vector<vector<int>> adj(V);
    vector<bool> vis(V, false);

    // build adjacency list
    for (auto &e : edges)
    {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }

    // handle disconnected components
    for (int i = 0; i < V; i++)
    {
        if (!vis[i])
        {
            if (hasCycle(i, -1, vis, adj))
                return true;
        }
    }
    return false;
}

/*
!Bipartite Graph Check using BFS (Graph Coloring)

Idea:
- Try to color graph using 2 colors (0 and 1)
- Adjacent nodes must have different colors
- If conflict occurs → odd cycle → not bipartite

Key facts:
- Even-length cycle → bipartite
- Odd-length cycle → NOT bipartite
- Works for disconnected graphs

Time Complexity:
- O(V + E)

Space Complexity:
- O(V)  (queue + color array)
*/

bool isBipartite(vector<vector<int>> &graph)
{
    int n = graph.size();
    vector<int> group(n, -1); // -1 = uncolored, 0 or 1 = color
    queue<int> q;

    for (int i = 0; i < n; i++)
    {
        if (group[i] != -1)
            continue;

        // start BFS from this component
        q.push(i);
        group[i] = 0;

        while (!q.empty())
        {
            int node = q.front();
            q.pop();

            for (int nei : graph[node])
            {
                // if neighbor not colored → assign opposite color
                if (group[nei] == -1)
                {
                    group[nei] = 1 - group[node];
                    q.push(nei);
                }
                // if same color → odd cycle found
                else if (group[nei] == group[node])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

// !Bipartite check using DFS + 2-coloring
// Color nodes with 0/1 such that no adjacent nodes share same color
// If conflict found → graph is NOT bipartite

// Time Complexity: O(V + E)
// Space Complexity: O(V)  // color array + recursion stack
bool dfs(int node, int parentColor,
         vector<vector<int>> &graph, vector<int> &color)
{
    color[node] = 1 - parentColor;

    for (int nei : graph[node])
    {
        if (color[nei] == -1)
        {
            if (!dfs(nei, color[node], graph, color))
                return false;
        }
        // neighbor already colored → conflict check
        else if (color[nei] == color[node])
        {
            return false;
        }
    }
    return true;
}

bool isBipartite(vector<vector<int>> &graph)
{
    int n = graph.size();
    vector<int> color(n, -1);

    for (int i = 0; i < n; i++)
    {
        if (color[i] == -1)
        {
            if (!dfs(i, 0, graph, color))
                return false;
        }
    }
    return true;
}
//! CYCLE DETECTION IN DIRECTED GRAPH (DFS)
// Uses TWO states:
//
// vis[]     → node is fully processed (global, never reset)
// inStack[] → node is in current DFS recursion path
//
// Key idea:
// A cycle exists if we find an edge to a node already in inStack[]
// (this edge is called a BACK EDGE)
//
// Important:
// - vis[] is NOT backtracked → ensures O(V + E)
// - inStack[] IS backtracked → represents current DFS call stack
//
// Difference from backtracking:
// - Backtracking explores ALL paths (visited is reset)
// - Cycle detection only checks recursion stack, not all paths
//
// Time Complexity:  O(V + E)
// Space Complexity: O(V)  // vis + inStack + recursion stack
bool dfsCyclic(int node, vector<bool> &vis, vector<int> &path, vector<vector<int>> &adj)
{
    vis[node] = true;
    path[node] = 1;
    for (auto nei : adj[node])
    {
        if (!vis[nei])
        {
            if (dfsCyclic(nei, vis, path, adj))
                return true;
        }
        else if (path[nei] == 1)
        {
            return true;
        }
    }
    path[node] = -1;
    return false;
}
bool isCyclic(int V, vector<vector<int>> &edges)
{
    vector<bool> vis(V, false);
    vector<int> path(V, -1);
    vector<vector<int>> adj(V);
    for (auto &e : edges)
    {
        adj[e[0]].push_back(e[1]);
    }
    for (int i = 0; i < V; i++)
    {
        if (vis[i])
            continue;
        if (dfsCyclic(i, vis, path, adj))
            return true;
    }
    return false;
}

// ! TOPOLOGICAL SORT (KAHN’S ALGORITHM – BFS)

/*
 Time Complexity:
 • Building adjacency list        → O(E)
 • Computing indegrees            → O(V + E)
 • BFS processing (each node once,
   each edge once)                → O(V + E)
 • Total                          → O(V + E)

 Space Complexity:
 • Adjacency list                 → O(V + E)
 • Indegree array                 → O(V)
 • Queue + result array           → O(V)
 • Total                          → O(V + E)

 Key Idea:
 • Nodes with indegree 0 have no prerequisites
 • Remove them layer by layer using BFS
 • Works only for DAG (Directed Acyclic Graph)

*/
vector<int> topoSort(int V, vector<vector<int>> &edges)
{
    // code here
    vector<int> in(V, 0);
    vector<int> res;
    vector<vector<int>> adj(V);
    for (auto &e : edges)
    {
        adj[e[0]].push_back(e[1]);
    }
    for (int i = 0; i < V; i++)
    {
        for (int nei : adj[i])
        {
            in[nei]++;
        }
    }
    queue<int> q;
    for (int i = 0; i < V; i++)
    {
        if (in[i] == 0)
            q.push(i);
    }

    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        res.push_back(node);
        for (int nei : adj[node])
        {
            in[nei]--;
            if (in[nei] == 0)
                q.push(nei);
        }
    }
    return res;
}

// ! Topological sort DFS
void dfs(int node, vector<bool> &vis, vector<int> &res, vector<vector<int>> &adj)
{
    vis[node] = true;
    for (int nei : adj[node])
    {
        if (!vis[nei])
            dfs(nei, vis, res, adj);
    }
    res.push_back(node);
}
vector<int> topoSort(int V, vector<vector<int>> &edges)
{
    vector<int> res;
    vector<bool> vis(V, false);
    vector<vector<int>> adj(V);
    for (auto &e : edges)
    {
        adj[e[0]].push_back(e[1]);
    }

    for (int i = 0; i < V; i++)
    {
        if (!vis[i])
            dfs(i, vis, res, adj);
    }
    reverse(res.begin(), res.end());
    return res;
}
// TC O(V + E)
//  • DFS traversal:
//      - Each vertex visited once  → O(V)
//      - Each edge explored once   → O(E)
//  • Reverse result array           → O(V)
// SC O(V) recursion stack, res, visited

// ! Shortest Path in Undirected Graph with unit weight
/*
 Why BFS?
 • All edges have unit weight (1)
 • BFS explores nodes level by level
 • First time we reach a node = shortest distance

 Time Complexity:
 • Building adjacency list        → O(E)
 • BFS traversal:
     - Each vertex visited once  → O(V)
     - Each edge explored once   → O(E)
 • Total                          → O(V + E)

 Space Complexity:
 • Adjacency list                 → O(V + E)
 • Distance array                 → O(V)
 • Visited array                  → O(V)
 • Queue                          → O(V)
 • Total                          → O(V + E)                        → O(V + E)
 */
vector<int> shortestPath(int n, vector<vector<int>> &edges, int src)
{
    // code here
    vector<vector<int>> adj(n);
    for (auto &e : edges)
    {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }
    vector<int> res(n, -1);
    vector<bool> vis(n, false);
    queue<int> q;
    q.push(src);
    vis[src] = true;
    res[src] = 0;
    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        for (int nei : adj[node])
        {
            if (!vis[nei])
            {

                res[nei] = res[node] + 1;
                vis[nei] = true;
                q.push(nei);
            }
        }
    }
    return res;
}
// !Shortest Path in Binary Matrix (BFS)
/*
 ✔ Uses 8-direction movement correctly
 ✔ Layer-based BFS gives shortest path length
 ✔ Marking grid as visited avoids extra vis array

 Time Complexity:
 • Each cell is visited at most once        → O(n²)
 • Each visit checks 8 directions           → O(8 * n²) ≈ O(n²)

 Space Complexity:
 • Queue can hold up to n² cells            → O(n²)
 • Direction array                          → O(1)
*/
bool isValid(int n, int x, int y, vector<vector<int>> &grid)
{
    return x >= 0 && y >= 0 && x < n && y < n && grid[x][y] == 0;
}
int shortestPathBinaryMatrix(vector<vector<int>> &grid)
{
    if (grid[0][0] == 1)
        return -1;
    int n = grid.size();
    vector<vector<int>> nei = {{1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}};
    queue<pair<int, int>> q; // x,y
    q.push({0, 0});
    grid[0][0] = 1;
    int layer = 0;
    while (!q.empty())
    {
        int qs = q.size();
        layer++;
        for (int i = 0; i < qs; i++)
        {
            auto [x, y] = q.front();
            q.pop();
            if (x == n - 1 && y == n - 1)
                return layer;
            for (auto &d : nei)
            {
                if (isValid(n, x + d[0], y + d[1], grid))
                { // valid neigh
                    grid[x + d[0]][y + d[1]] = 1;
                    q.push({x + d[0], y + d[1]});
                }
            }
            grid[x][y] = 1;
        }
    }
    return -1;
}

// ! DIJKSTRA’S ALGORITHM (UNDIRECTED WEIGHTED GRAPH)

/*
 Algorithm:
 • Use min-heap (priority_queue with greater) to always
   expand the node with smallest current distance
 • Relax edges: if a shorter path is found, update distance
 • Skip outdated heap entries using lazy deletion

 Why it works:
 • All edge weights are non-negative
 • Once the minimum-distance node is popped, its
   shortest path is finalized

 Time Complexity:
 • Building adjacency list        → O(E)
 • Dijkstra with min-heap         → O(E log V)

 Space Complexity:
 • Adjacency list                → O(V + E)
 • Distance array                → O(V)
 • Priority queue                → O(V)

 Note
 • Not valid for graphs with negative weights
*/

#define ppi pair<int, int>
vector<int> dijkstra(int n, vector<vector<int>> &edges, int src)
{
    vector<vector<ppi>> adj(n); // next node, wt
    for (auto &e : edges)
    {
        adj[e[0]].push_back({e[1], e[2]});
        adj[e[1]].push_back({e[0], e[2]});
    }
    priority_queue<ppi, vector<ppi>, greater<ppi>> q;
    vector<int> dist(n, INT_MAX);
    q.push({0, src}); // dist,node
    dist[src] = 0;
    // Using visited[] does NOT make Dijkstra asymptotically faster.
    // It may reduce some PQ pushes when Dense graphs or
    // Graphs where many alternative longer paths exist
    // !vector<bool> visited(n, false);
    while (!q.empty())
    {
        auto [nodeDist, node] = q.top();
        q.pop();
        // if (visited[node])
        //     continue;
        // visited[node] = true;
        if (nodeDist > dist[node])
            continue; // why push neighbours with longer path

        for (auto [nei, neiDist] : adj[node]) // O(E log V)
        {
            if (dist[nei] > dist[node] + neiDist)
            // if(!visited[nei] && dist[nei] > dist[node] + neiDist ){
            {
                dist[nei] = dist[node] + neiDist;
                q.push({dist[nei], nei});
            }
        }
    }
    return dist;
}
// ! dijkstra with path
vector<int> dijkstra(int n, vector<vector<int>> &edges, int src)
{
    vector<vector<ppi>> adj(n); // node, wt
    // [
    //     [{1,2},{2,3}],
    //     [{3,1},{4,1}]
    // ]

    for (auto &e : edges)
    { // O(E)
        adj[e[0]].push_back({e[1], e[2]});
        adj[e[1]].push_back({e[0], e[2]});
    }
    vector<int> parent(n, -1);
    priority_queue<ppi, vector<ppi>, greater<ppi>> q; // dist till node, node
    q.push({0, src});                                 // O(log V)
    vector<int> dist(n, INT_MAX);
    dist[src] = 0;
    while (!q.empty())
    {
        auto [dtn, node] = q.top();
        q.pop();
        if (dtn > dist[node])
            continue;

        for (auto [nei, wt] : adj[node])
        {
            if (dist[nei] > dtn + wt)
            {
                dist[nei] = dtn + wt;
                parent[nei] = node;
                q.push({dist[nei], nei});
            }
        }
    }
    int dest = 8;
    if (dist[dest] == INT_MAX)
        cout << "No path\n";
    vector<int> path;
    path.push_back(dest);
    while (parent[dest] != -1)
    {
        path.push_back(parent[dest]);
        dest = parent[dest];
    }
    reverse(path.begin(), path.end());
    for (int i : path)
        cout << i << " ";
    cout << endl;
    return dist;
}
// !Shortest path in Directed Acyclic Graph
/*
 Building adjacency list: O(E)
 Topological sort (DFS): O(N + E)
 Relaxation pass: O(N + E)
 Total Time: O(N + E)

 Space:
 adj list: O(N + E)
 recursion stack: O(N)
 dist + vis + stack: O(N)
 Total Space: O(N + E)
*/
#define ppi pair<int, int>
void topoSort(int node, vector<bool> &vis, stack<int> &st, vector<vector<ppi>> &adj)
{
    vis[node] = true;
    for (auto [nei, wt] : adj[node])
    {
        if (!vis[nei])
            topoSort(nei, vis, st, adj);
    }
    st.push(node);
}
vector<int> shortestPath(int n, vector<vector<int>> &edges)
{

    vector<vector<ppi>> adj(n);

    for (auto &e : edges)
    {
        adj[e[0]].push_back({e[1], e[2]});
    }

    vector<bool> vis(n, false);
    stack<int> st;
    for (int i = 0; i < n; i++)
    {
        if (!vis[i])
            topoSort(i, vis, st, adj);
    }

    vector<int> dist(n, INT_MAX);
    dist[0] = 0;
    // Relax edges in topological order
    while (!st.empty())
    {
        int node = st.top();
        st.pop();
        if (dist[node] == INT_MAX)
            continue;
        for (auto [nei, wt] : adj[node])
        {
            if (dist[nei] > dist[node] + wt)
            {
                dist[nei] = dist[node] + wt;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (dist[i] == INT_MAX)
        {
            dist[i] = -1;
        }
    }

    return dist;
}

// !MST using Prims
// Prim’s Algorithm (Min Spanning Forest)
// Intuition:
// Always pick the minimum-weight edge that is connected to selected vertices
// Preference : connected and minimum
// Guarantees a spanning tree at every step (graph stays connected)

// Time: O(E log V)   // heap operations for edges
// Space: O(E + V)    // adjacency list + heap + visited

#define ppi pair<int, int>
int spanningTree(int n, vector<vector<int>> &edges)
{
    // find min wt
    //! dont need to find minimum edge in graph, start from any node
    // int firstNode;
    // int minPath=INT_MAX;
    // for(auto &e:edges){
    //     if(e[2] < minPath){
    //         firstNode=e[0];
    //         minPath = e[2];
    //     }
    // };

    // create adj
    vector<vector<ppi>> adj(n);
    for (auto &e : edges)
    {
        adj[e[0]].push_back({e[1], e[2]});
        adj[e[1]].push_back({e[0], e[2]});
    }

    priority_queue<ppi, vector<ppi>, greater<ppi>> q;
    vector<bool> vis(n, false);
    int ans = 0;
    // ! dont need to push nei of a node outside while loop, push in while loop
    // by not marking this node visited before loop
    // push all nei of firstNode in minHeap
    // for(auto [nei, wt] : adj[firstNode]){
    //     q.push({wt,nei});
    // }
    // vis[firstNode]=true;

    // use for loop and visited to iterate non connected component also
    for (int i = 0; i < n; i++)
    {
        if (vis[i])
            continue;

        q.push({0, i});

        while (!q.empty())
        {
            auto [wt, node] = q.top();
            q.pop();
            if (vis[node])
                continue;
            vis[node] = true;
            ans += wt;
            for (auto [nei, wt] : adj[node])
            {
                if (!vis[nei])
                    q.push({wt, nei});
            }
        }
    }

    return ans;
}

/*
Prim → Dense graphs
Grows the MST node by node
At each step, many edges are available
Using a heap, choosing the smallest edge is efficient
Doesn’t need to look at all edges at once
 Dense graph = many edges per node → Prim fits naturally

Kruskal → Sparse graphs
Sorts all edges first
When edges are few, sorting is cheap
DSU quickly rejects cycle-forming edges
 Sparse graph = few edges → Kruskal is faster & simpler
*/
// ! MST using Kruskal
// DSU (Disjoint Set Union) / Union-Find
// What:
// Maintains disjoint sets where each set represents
// a connected component of the graph.

// How:
// - parent[x] stores the leader of x
// - find(x) finds the leader with path compression
// - union(x, y) merges two sets using rank to keep trees shallow

// Why:
// Enables fast cycle detection in Kruskal’s algorithm.

// Time Complexity:
// - find / union: ~O(α(N)) amortized (almost constant)
// Space Complexity:
// - O(N) for parent and rank arrays

class DSU
{
    vector<int> parent, rank;
    /*
     * What:
     * Rank is a heuristic value that represents an
     * approximate height (or depth) of the tree.
     *
     * Why:
     * Used during union to attach the smaller tree
     * under the larger one, keeping trees shallow.
     *
     * How:
     * - Initially, rank = 0 for all nodes
     * - Rank increases ONLY when two trees of equal rank are merged
     */
public:
    DSU(int n)
    {
        parent.resize(n);
        rank.assign(n, 0);
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }
    // ! slower, path compression missing
    // int findParent(int i)
    // {
    //     if (parent[i] == i)
    //         return i;
    //     else
    //         return findParent(parent[i]);
    // }

    int findParent(int x)
    {
        if (parent[x] != x)
            parent[x] = findParent(parent[x]); // path compression
        return parent[x];
    }

    void unify(int x, int y)
    {
        int px = findParent(x);
        int py = findParent(y);

        if (px == py)
            return;

        if (rank[px] > rank[py])
        {
            parent[py] = px;
        }
        else if (rank[py] > rank[px])
        {
            parent[px] = py;
        }
        else
        {
            parent[px] = py;
            rank[py]++;
        }
    }
};
// Kruskal’s Algorithm (MST)
// Intuition:
// Pick edges in increasing order of weight.
// Add an edge only if it connects two different components
// (checked using DSU) to avoid cycles.
// Stop after selecting (V - 1) edges.

// Time Complexity: O(E log E)   // sorting edges
// Space Complexity: O(E + V)    // edge list + DSU

int spanningTree(int n, vector<vector<int>> &edges)
{
    DSU d(n);
    sort(edges.begin(), edges.end(), [](const auto &a, const auto &b)
         { return a[2] < b[2]; });
    // Comparators must not modify elements;
    // using const enforces read-only access and matches STL’s expectations.
    int count = 0;
    int mst = 0;
    for (auto &e : edges)
    {
        int u = e[0];
        int v = e[1];
        int wt = e[2];
        if (d.findParent(u) != d.findParent(v))
        {
            mst += wt;
            count++;
            d.unify(u, v);
        }
        if (count == n - 1)
            return mst;
    }
    return -1;
}
// ! Bellman Ford algo
/*
 - For negative weight and cycle
 - We can use dijkstra for negative weights but not cyclic negative weights
 -
*/
/**
 * Bellman–Ford Intuition (edge-by-edge propagation)
 *
 * Each iteration allows shortest paths that use
 * ONE more edge to become correct.
 *
 * Round 1  → paths using ≤ 1 edge are correct
 * Round 2  → paths using ≤ 2 edges are correct
 * ...
 * Round V-1 → all shortest paths are correct
 *
 * Distance updates DO NOT cascade within the same round.
 * An improvement at edge u→v in round k can only help
 * v’s outgoing edges starting from round k+1.
 *
 * Extra relaxation after V-1 rounds ⇒ negative cycle.
 */

/**
 * Bellman–Ford Example (intuition with rounds)
 *
 * Edges:
 * 1→6 (10)
 * 5→6 (-2)
 * 4→5 (-2)
 * 3→4 (-2)
 * 2→3 (-2)
 * 6→7 (2)
 * 1→2 (10)
 * Source = 1
 * dist = [1:0, 2:∞, 3:∞, 4:∞, 5:∞, 6:∞, 7:∞]
 * Round 1:  dist[2]=10, dist[6]=10  [1:0, 2:10, 3:∞, 4:∞, 5:∞, 6:10, 7:12]
 * Round 2:  dist[3]=8,  dist[7]=12  [1:0, 2:10, 3:8,  4:∞, 5:∞, 6:10, 7:12]
 * Round 3:  dist[4]=6
 * Round 4:  dist[5]=4
 * Round 5:  dist[6]=2   (improved via 1→2→3→4→5→6)
 * Round 6:  dist[7]=4   [1:0, 2:10, 3:8,  4:6,  5:4,  6:2,  7:4]
 *
 * Note:
 * In actual code, distance updates may cascade within
 * the same iteration depending on edge order.
 *
 * However, Bellman–Ford guarantees correctness only
 * after V−1 iterations, since each round ensures
 * shortest paths using one more edge.
 *
 * Each round pushes shortest paths ONE edge farther.
 */
// Time: O(V * E)  // relax all E edges (V - 1) times
// Space: O(V)    // distance array for V vertices
vector<int> bellmanFord(int n, vector<vector<int>> &edges, int src)
{

    vector<int> dist(n, 1e8);

    dist[src] = 0;
    for (int i = 0; i < n - 1; i++)
    {
        // relax edges v-1 times

        for (auto &e : edges)
        {
            int u = e[0];
            int v = e[1];
            int wt = e[2];
            if (dist[u] != 1e8 && dist[v] > dist[u] + wt)
            {
                dist[v] = dist[u] + wt;
            }
        }
    }

    for (auto &e : edges)
    {
        int u = e[0];
        int v = e[1];
        int wt = e[2];
        if (dist[u] != 1e8 && dist[v] > dist[u] + wt)
        {
            return {-1};
        }
    }

    return dist;
}

/**
 * !Floyd–Warshall Algorithm

 * If the shortest path from i to j passes through some vertex k,
 * then the path from i to k and the path from k to j must also be
 * shortest paths.
 * The iterative approach ensures that by the time vertex k is considered,
 * all shortest paths using only vertices 0 to k-1 have already been computed.

 * Intuition:
 * Try every node `k` as an intermediate point.
 * If going from `i → k → j` is cheaper than `i → j`,
 * update the shortest distance.
 *
 * Handles:
 * - Negative edge weights
 * - All-pairs shortest paths
 * - Detects negative cycles (if dist[i][i] < 0)
 * - Time Complexity: O(N^3)
 */

void floydWarshall(vector<vector<int>> &dist, int n)
{
    const int INF = 100000000;

    for (int k = 0; k < n; k++)
    { // intermediate node
        for (int i = 0; i < n; i++)
        { // source
            for (int j = 0; j < n; j++)
            { // destination
                if (dist[i][k] == INF || dist[k][j] == INF)
                    continue;
                if (dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}
// !Kosaraju SCC
/**
 * Why topo (finish-time order) is required in Kosaraju
 *
 * Think of each SCC as a single node.
 * The SCC graph is always a DAG:
 *
 *     SCC1 → SCC2 → SCC3
 *
 * First DFS (topo):
 * - DFS finishes SCC3 first, then SCC2, then SCC1
 * - Stack (top last): [ SCC3, SCC2, SCC1 ]
 * - So SCC1 is on top of the stack
 *
 * Reverse the graph:
 *
 *     SCC1 ← SCC2 ← SCC3
 *
 * Second DFS (using stack order):
 * - Start from SCC1 (stack top)
 * - SCC1 has no outgoing edges in reversed graph
 * - DFS cannot go to any other SCC
 * - Exactly one SCC is found
 *
 * Why this order matters:
 * - If we start DFS from SCC2 or SCC3 first,
 *   DFS would leak into other SCCs and merge them
 *   incorrectly.
 *
 * Conclusion:
 * Topo / finish-time order ensures we always start
 * DFS from an SCC that is isolated in the reversed graph.
 */

// 1. Topo DFS → fill stack by finish time
// 2. Reverse graph
// 3. Pop from stack and DFS on reversed graph
/**
 * Kosaraju's Algorithm (Strongly Connected Components)
 *
 * Intuition:
 * - Nodes finishing last in DFS belong to SCCs that
 *   cannot be reached from other SCCs.
 * - Reversing edges preserves SCCs but reverses
 *   connections between different SCCs.
 *
 * Steps:
 * 1. Run DFS and push nodes to stack by finish time.
 * 2. Reverse the graph.
 * 3. Pop nodes from stack and DFS on reversed graph.
 *    Each DFS gives exactly one SCC.
 *
 * Why it works:
 * Stack order ensures we always start DFS from an SCC
 * that is isolated from others in the reversed graph.
 */
// Time Complexity: O(V + E)
// Reason: Each DFS runs once over all nodes and edges.
//
// Space Complexity: O(V + E)
// Reason: Adjacency list + reversed graph + recursion stack.
void topo(int node, vector<bool> &vis, vector<vector<int>> &adj, stack<int> &st)
{
    vis[node] = true;
    for (auto nei : adj[node])
    {
        if (!vis[nei])
        {
            topo(nei, vis, adj, st);
        }
    }
    st.push(node);
}

void dfs(int node, vector<bool> &vis, vector<vector<int>> &adj, vector<int> &comp)
{
    vis[node] = true;
    comp.push_back(node); // store node in current SCC

    for (int nei : adj[node])
    {
        if (!vis[nei])
        {
            dfs(nei, vis, adj, comp);
        }
    }
}

int kosaraju(vector<vector<int>> &adj)
{
    int n = adj.size();
    vector<bool> vis(n, false);
    stack<int> st;
    for (int i = 0; i < n; i++)
    {
        if (!vis[i])
            topo(i, vis, adj, st);
    }

    vector<vector<int>> rev(n);
    for (int u = 0; u < n; u++)
    {
        for (int v : adj[u])
        {
            rev[v].push_back(u); // reverse edge
        }
    }

    vis.assign(n, false);
    int res = 0;
    vector<vector<int>> sccs;

    while (!st.empty())
    {
        int node = st.top();
        st.pop();
        if (!vis[node])
        {
            vector<int> comp; // one SCC
            dfs(node, vis, rev, comp);
            sccs.push_back(comp); // store SCC
            res++;
        }
    }

    return res;
}
/**
 * !Bridges in an Undirected Graph (Tarjan's Algorithm)
 *
 * What is a bridge?
 * An edge whose removal increases the number of connected components.
 *
 * Key idea:
 * - DFS tree + discovery times (tin)
 * - low[u] = earliest discovery time reachable from u
 * ! if some nodes have same low, earliest reachable ancestor is the same”

 * Bridge condition:
 * For a DFS tree edge (u → v),
 * if low[v] > tin[u], then (u, v) is a bridge.
 *
 * Meaning:
 * v's subtree cannot reach u or any ancestor of u
 * without using edge (u, v).
 */
// Time Complexity: O(V + E)
// Reason: Single DFS traversal over all nodes and edges.
//
// Space Complexity: O(V)
// Reason: tin[], low[], visited[] arrays + recursion stack.

class Solution
{
public:
    vector<int> tin, low;
    vector<bool> vis;
    vector<vector<int>> res;
    int timer = 0;

    void dfs(int node, int parent, vector<vector<int>> &adj)
    {
        vis[node] = true;
        tin[node] = low[node] = timer++;

        for (int nei : adj[node])
        {
            if (nei == parent)
                continue;

            if (!vis[nei])
            {
                // Tree edge
                dfs(nei, node, adj);

                // this nei has a tree edge (node and nei in same subtree)
                // parent node can be reached earlier by some nei/child
                low[node] = min(low[node], low[nei]);

                // Bridge check
                // if child can be reached only later than parent node
                if (low[nei] > tin[node])
                {
                    res.push_back({node, nei});
                }
            }
            else
            {
                // this nei has a back edge (nei to some other node from another subtree)
                // already visited, can be visited from some another path
                // i dont want info of another subtree's path
                // so use tin[nei] not low[nei]
                // although code will work with both
                low[node] = min(low[node], tin[nei]);
                // Using tin keeps cycles separate;
                // using low causes wrong propagation and merges independent cycles.
            }
        }
    }
    /**
     * Why back-edge update must use tin[] and NOT low[]
     *
     * Correct updates:
     *   Tree edge  (u -> v): low[u] = min(low[u], low[v])
     *   Back edge  (u -> v): low[u] = min(low[u], tin[v])
     *
    * Graph explanation (tin / low intuition)
    *
    * Edges:
    * 0-1
    * 1-4
    * 2-4
    * 3-4
    * 2-3
    * 3-5
    * 3-6
    * 5-6
    *
    * Structure:
    * - Cycle A: {2, 3, 4}
    * - Cycle B: {3, 5, 6}
    * - Node 3 connects both cycles
    * - Chain: 0 - 1 - 4
    *
    * tin[] (example DFS order):
    * 0→1→4→2→3→5→6
    *
    * low[] meaning:
    * - Nodes in the same cycle collapse to the same low value
    * - {2,3,4} → low = tin[4]
    * - {5,6}   → low = tin[3]
    *
    * Key insight:
    * Back-edges inside a cycle pull low[] up to the
    * earliest discovery time of the cycle entry.
    *
    * Why tin[] (not low[]) for back-edges:
    * - Using tin keeps Cycle A and Cycle B distinct
    * - Using low would incorrectly merge them

     * Key rule:
     * A back-edge gives access only to the ancestor node,
     * not to what the ancestor’s subtree can reach.
     *
     * Using low[] in back-edge updates over-propagates
     * information and breaks deeper graph properties.
     * Bridge detection still works
     * Articulation points / biconnected components break
     */

    vector<vector<int>> criticalConnections(int n,
                                            vector<vector<int>> &connections)
    {

        vector<vector<int>> adj(n);
        for (auto &e : connections)
        {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        tin.assign(n, -1);
        low.assign(n, -1);
        vis.assign(n, false);

        // Handle disconnected graph
        for (int i = 0; i < n; i++)
        {
            if (!vis[i])
            {
                dfs(i, -1, adj);
            }
        }

        return res;
    }
};
// ! Articulation point
/**
 * Articulation Point (Cut Vertex) – Intuition
 *
 * A node u is an articulation point if removing u
 * disconnects at least one DFS subtree from the graph.
 *
 * Non-root case:
 * If there exists a DFS child v such that
 * low[v] >= tin[u],
 * then v’s subtree cannot reach any ancestor of u
 * without passing through u → u is an articulation point.
 *
 * Root case:
 * Root is an articulation point only if it has
 * two or more DFS children.
 */
/**
 * Common mistakes I made while implementing articulation points
 *
 * 1) Counting neighbors instead of DFS children
 *    - children count must increase ONLY when exploring
 *      an unvisited neighbor (tree edge)
 *
 * 2) Applying articulation condition to back-edges
 *    - low[v] >= tin[u] must be checked ONLY for DFS children
 *    - never for already visited neighbors
 *
 * 3) Mixing root and non-root logic
 *    - children >= 2 applies ONLY to root node
 *    - non-root needs just ONE child with low[v] >= tin[u]
 *
 * 4) Misplacing conditions inside the loop
 *    - articulation checks must be inside the
 *      `if (!vis[nei])` block
 *
 * Key takeaway:
 * Articulation points depend on DFS tree structure,
 * not on raw adjacency or number of neighbors.
 *
 * Testcase:
 *  * Graph:
 *
 *    2-----4
 *     \   /
 *       3
 *      / \
 *     5---6
 *
 * Why node 3 IS an articulation point:
 * - Subgraph {2,3,4} forms a cycle
 * - Subgraph {3,5,6} forms another cycle
 * - Node 3 is the ONLY connection between these two cycles
 *
 *  Graph structure:
 *
 *     0
 *     |
 *     1
 *     |
 *     4
 *    / \
 *   2---3---6
 *        \ /
 *         5
 *
 * Adjacency List
 *
 * 0: 1
 * 1: 0, 4
 * 2: 4, 3
 * 3: 4, 2, 5, 6
 * 4: 1, 2, 3
 * 5: 3, 6
 * 6: 5, 3
 *
 * (Edges explicitly shown)
 * - 2-3 completes cycle {2,3,4}
 * - 3-5, 5-6, 3-6 completes cycle {3,5,6}
 *
 * Articulation Points:
 * - 1 : removing disconnects node 0
 * - 4 : separates chain {0,1} from rest
 * - 3 : separates cycle {5,6} from cycle {2,4}
 *
 */

vector<int> tin, low;
vector<bool> vis;
vector<int> res;
int timer = 1;
void dfs(int node, int parent, vector<int> adj[])
{
    vis[node] = true;
    tin[node] = low[node] = timer++;
    int children = 0;

    for (int nei : adj[node])
    {
        if (nei == parent)
            continue;

        if (!vis[nei])
        {
            children++;
            dfs(nei, node, adj);

            low[node] = min(low[node], low[nei]);

            // non-root articulation condition
            if (parent != -1 && low[nei] >= tin[node])
            // this nei can be visited only by passing through node
            {
                res[node] = 1;
            }
        }
        else
        {
            // back-edge
            low[node] = min(low[node], tin[nei]);
        }
    }

    // root articulation condition
    if (parent == -1 && children >= 2)
    {
        res[node] = 1;
    }
}
vector<int> articulationPoints(int n, vector<int> adj[])
{
    tin.assign(n, -1);
    low.assign(n, -1);
    res.assign(n, 0);
    vis.assign(n, false);
    // Handle disconnected graph
    for (int i = 0; i < n; i++)
    {
        if (!vis[i])
        {
            dfs(i, -1, adj);
        }
    }
    vector<int> ans;
    for (int i = 0; i < n; i++)
    {
        if (res[i] > 0)
            ans.push_back(i);
    }
    if (ans.size() == 0)
        return {-1};
    return ans;
}
int main()
{

    return 0;
}