/// This algorithm solves the single-source shortest path for directed graphs.

/// It can be adapted for undirected graphs, by adding two edges of opposite direction between two vertices.

/// How to use:

/// Step 1: Construct a directed graph G with n vertices, represented as an adjacency list. Also, define a source (see line 51-63).
/// Step 2: Save in a vector the results of dijkstra(G, n, source) (see line 65).

#include <bits/stdc++.h>

template < typename T >
    std :: vector < T > dijkstra(std :: vector < std :: pair < T, int > > G[], int n, int source) {
        const T INF = 1e9; /// The value of a big enough number.

        std :: priority_queue < std :: pair < T, int > > heap;
        std :: vector < T > dist(n + 1, INF);
        std :: vector < bool > f(n + 1, false);

        dist[source] = 0;
        heap.push({-dist[source], source});

        while (!heap.empty()) {
            int u = heap.top().second;

            if (!f[u]) {
                f[u] = true;

                for (int i = 0; i < G[u].size(); ++ i) {
                    int cost = G[u][i].first, v = G[u][i].second;

                    if (dist[v] > dist[u] + cost) { /// If working with doubles, use floating-point arithmetic.
                        dist[v] = dist[u] + cost;
                        heap.push({-dist[v], v});
                    }
                }
            }

            heap.pop();
        }

        return dist; /// If a vertex isn't reachable from the source, it will have the value of INF.
    }

std :: ifstream fin("dijkstra.in");
std :: ofstream fout("dijkstra.out");

const int NMAX = 1e5 + 5;

int main() {
    int n, m;
    std :: vector < std :: pair < long long, int > > G[NMAX];

    fin >> n >> m;

    for (int i = 0; i < m; ++ i) {
        int u, v;
        long long c;

        fin >> u >> v >> c;

        G[u].push_back({c, v});
    }

    std :: vector < long long > x = dijkstra(G, n, 0);

    for (int i = 0; i < n; ++ i)
        fout << x[i] << " ";

    return 0;
}

/**
Example:

Input:
5 6
0 2 1
1 0 2
3 1 1
3 2 8
4 2 5
4 3 2

Output:
3 1 4 0 1000000000
**/
