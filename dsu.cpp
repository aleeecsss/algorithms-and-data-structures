/// The following data structure solves online the following Q updates / queries in O(Q α(|V|)):

/// [1]: Adds an edge in an undirected graph. If there is already an edge, the data structure ignores this update.
/// [2]: Queries if two vertices are in the same connected component.

/// How to use:

/// See line 80-99.

#include <bits/stdc++.h>

class dsu {
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
            /// Complexity: O(α(n)).

            if (t[x] == x)
                return x;

            return (t[x] = findRoot(t[x]));
        }

        bool add(int x, int y) {
            /// Unites two trees (only if necessary). Returns true if the union happens.
            /// Complexity: O(α(n)).

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
            /// Complexity: O(α(n)).

            x = findRoot(x);
            y = findRoot(y);

            return (x == y);
        }
};

std :: ifstream fin("dsu.in");
std :: ofstream fout("dsu.out");

int main() {
    int n, Q;

    fin >> n >> Q;

    dsu T(n + 1); /// Declaring the Disjoint Set Union data structure.

    while (Q --) {
        char c; /// Can take the following symbols: "+", "?".
        int u, v;

        /// + u v -> adds an edge between u and v
        /// ? u v -> queries if u and v are in the same connected component.

        fin >> c >> u >> v;

        if (c == '?')
            fout << T.query(u, v) << "\n";
        else
            T.add(u, v);
    }

    return 0;
}

/**
Example:

Input:
3 4
+ 1 2
? 1 3
+ 2 3
? 1 3

Output:
0
1
**/
