/// The following data structure solves offline the following Q updates / queries in O(Q log^2 |V|):

/// [1]: Adds an edge in an undirected graph. If there is already an edge, the data structure ignores this update.
/// [2]: Removes an existing edge in an undirected graph. If there isn't an edge, the data structure ignores this update.
/// [3]: Queries if two vertices are in the same connected component.

/// How to use:

/// Step 1: Declare a solver G which takes as a parameter a vector of updates / queries (see line 14-21, 224-239).
/// Step 2: Save all results in another vector that takes the values of G.solve() (see line 241).

#include <bits/stdc++.h>

struct Query {
    char c; /// Can take the following symbols: "+", "-", "?".
    int u, v;

    /// + u v -> adds an edge between u and v
    /// - u v -> removes an edge between u and v
    /// ? u v -> queries if u and v are in the same connected component
};

struct TreeUpdate {
    /// Updates for the tree in solver class.

    int x, y, u, v;
};

struct Edge {
    /// Edges for undirected graphs.

    int u, v;
};

class dsu {
    /// This data structure also allows undos / rollbacks!

    private:
        int n;
        std :: vector < int > t, sz;
        std :: stack < std :: pair < int, int > > st;
    public:
        dsu (int _n) {
            /// Constructs a Disjoint Set Union data structure of size n.
            /// Complexity: O(n).

            n = _n;

            for (int i = 0; i < n; ++ i) {
                t.push_back(i);
                sz.push_back(1);
            }
        }

        int findRoot(int x) {
            /// Finds the root of the tree in which x is located.
            /// Complexity: O(log n).

            while (t[x] != x)
                x = t[x];

            return x;
        }

        bool add(int x, int y) {
            /// Unites two trees (only if necessary). Returns true if the union happens.
            /// Complexity: O(log n).

            x = findRoot(x);
            y = findRoot(y);

            if (x != y) {
                if (sz[x] < sz[y]) {
                    t[x] = y;
                    sz[y] += sz[x];
                    st.push({x, y});
                }
                else {
                    t[y] = x;
                    sz[x] += sz[y];
                    st.push({y, x});
                }

                return true;
            }

            return false;
        }

        bool query(int x, int y) {
            /// Queries if x and y are in the same tree.
            /// Complexity: O(log n).

            x = findRoot(x);
            y = findRoot(y);

            return (x == y);
        }

        void rollback() {
            /// Undos the last useful update in the data structure.
            /// Complexity: O(1).

            if (st.empty())
                return;

            int x = st.top().first, y = st.top().second;
            t[x] = x;
            sz[y] -= sz[x];
            st.pop();

            return;
        }
};

class solver {
    private:
        int n, Max; /// n = Q = the number of updates / queries.
        std :: vector < std :: vector < Edge > > tree;
        std :: vector < TreeUpdate > q;
        std :: vector < Query > f;
        std :: vector < int > ans;
        std :: map < std :: pair < int, int >, int > M;
    public:
        void Update(int node, int l, int r, TreeUpdate p) {
            /// Updates the nodes in the solving tree.
            /// Complexity: O(log n).

            if (r < p.x || p.y < l)
                return;

            if (p.x <= l && r <= p.y) { /// Using this technique, the segment tree data structure (tree) divides the "life-span" of an edge in O(log n) intervals.
                tree[node].push_back({p.u, p.v});
                return;
            }

            int mid = ((l + r) >> 1);

            Update(node * 2 + 1, l, mid, p);
            Update(node * 2 + 2, mid + 1, r, p);

            return;
        }

        void DFS(dsu &T, int node, int l, int r) {
            if (l > r)
                return;

            int cnt = 0;

            for (int i = 0; i < tree[node].size(); ++ i) /// Updates the Disjoint Set Union data structure.
                cnt += T.add(tree[node][i].u, tree[node][i].v);

            if (l == r && f[l].c == '?')
                ans.push_back(T.query(f[l].u, f[l].v));

            int mid = ((l + r) >> 1);

            if (l < r) {
                DFS(T, 2 * node + 1, l, mid); /// Solves the problem recursively, using the edges added along the way.
                DFS(T, 2 * node + 2, mid + 1, r); ///
            }

            while (cnt --) /// Removes all the edges, because we are about to return to an earlier stage (in which every edge we added earlier doesn't exist anymore).
                T.rollback();

            return;
        }

        solver (std :: vector < Query > v) {
            /// Constructs the tree of updates / queries (in which it propagates information).
            /// Complexity: O(n log n).

            n = v.size();

            f = v;

            Max = 0;

            for (int i = 0; i < n; ++ i) { /// Finds the "life-span" of every edge in the input.
                Max = std :: max(Max, std :: max(v[i].u, v[i].v));

                if (v[i].c == '+' && M[{v[i].u, v[i].v}] == 0)
                    M[{v[i].u, v[i].v}] = M[{v[i].v, v[i].u}] = i + 1;

                if (v[i].c == '-' && M[{v[i].u, v[i].v}] != 0) {
                    q.push_back({M[{v[i].u, v[i].v}] - 1, i, v[i].u, v[i].v});
                    M.erase({v[i].u, v[i].v});
                    M.erase({v[i].v, v[i].u});
                }
            }

            for (std :: map < std :: pair < int, int >, int > :: iterator it = M.begin(); it != M.end(); ++ it) { /// Some edges aren't removed, so they become "immortal".
                q.push_back({(it -> second) - 1, n - 1, (it -> first).first, (it -> first).second});
                M.erase({(it -> first).second, (it -> first).first});
            }

            for (int i = 0; i < 4 * n; ++ i) {
                std :: vector < Edge > w;

                tree.push_back(w);
            }

            for (int i = 0; i < q.size(); ++ i) { /// Propagates information in the "life-span" intervals of the edges.
                Update(0, 0, n - 1, q[i]);
            }
        }

        std :: vector < int > solve() {
            /// Solves the problem.
            /// Complexity: O(n log^2 |V|).

            dsu T(Max + 1);

            DFS(T, 0, 0, n - 1);

            return ans;
        }
};

std :: ifstream fin("dynamic-connectivity.in");
std :: ofstream fout("dynamic-connectivity.out");

int main() {
    int Q;
    std :: vector < Query > v;

    fin >> Q;

    while (Q --) { /// Reading the updates / queries.
        char c;
        int x, y;

        fin >> c >> x >> y;

        v.push_back({c, x, y});
    }

    solver G(v); /// Inserting them in solver.

    std :: vector < int > ans = G.solve(); /// Solves the problem.

    for (int i = 0; i < ans.size(); ++ i) /// Outputs the answers to queries.
        fout << ans[i] << "\n";

    return 0;
}

/**
Example:

Input:
11
+ 1 2
+ 1 3
+ 2 3
+ 3 5
+ 5 9
+ 9 1
? 7 9
? 9 1
- 9 1
- 2 3
? 1 9

Output:
0
1
1
**/
