/// This heuristic finds with high frequency of success the longest path in relatively sparse and dense graphs.
/// The algorithm tries to extend a path until it becomes maximal. I also applied different tricks to improve the overall result returned by the algorithm.

/// Implementation for the O(|V| |E|) algorithm.

/// Relatively sparse -> O(|V| log |V|) edges
/// Dense -> O(|V|^2) edges

/// The complexity of the algorithm is O(|V||E|).

/// Old, but reliable source code.

/// VERY IMPORTANT! The input graph must be connected and its nodes are indexed from 0.

#include <fstream>
#include <iostream> /// debug
#include <iomanip>

#include <vector>
#include <bitset>
#include <queue>

#include <algorithm>
#include <random>
#include <chrono>

#include <cmath>

std :: mt19937 rng(std :: chrono :: steady_clock :: now().time_since_epoch().count());

const int dim = 1e4 + 5;

int T, n, m, s, lim, res, op;

std :: vector < int > path, maxPath, finalPath, G[dim], A[dim];
std :: vector < int > t(dim), dist(dim), degree(dim);
std :: bitset < dim > f, marked[dim];

std :: ifstream fin("hamilton.in");
std :: ofstream fout("hamilton.out");

void DFS(int node) {
    f[node] = true;

    for (int i = 0; i < G[node].size(); ++ i) {
        int u = G[node][i];

        if (f[u] == false) {
            t[u] = node;
            DFS(u);
        }
    }

    return;
}

bool isArtPoint(int node) {
    int cnt = 0;

    f[node] = true;

    for (int i = 0; i < n; ++ i)
        if (f[i] == false) {
            ++ cnt;
            DFS(i);
        }

    return (cnt > 1);
}

void DFSTree(int node) {
    f[node] = true;

    for (int i = 0; i < A[node].size(); ++ i) {
        int u = A[node][i];

        if (f[u] == false) {
            t[u] = node;
            dist[u] = 1 + dist[node];
            DFSTree(u);
        }
    }

    return;
}

void findDiameter() {
    for (int i = 0; i < n; ++ i)
        f[i] = false;

    for (int i = 0; i < n; ++ i)
        A[i].clear();

    for (int i = 0; i < n; ++ i) {
        if (t[i] != -1) {
            A[i].push_back(t[i]);
            A[t[i]].push_back(i);
        }
    }

    dist[0] = 1;
    DFSTree(0);

    int Max = 0, root, dest;

    for (int i = 0; i < n; ++ i)
        if (dist[i] > Max) {
            Max = dist[i];
            root = i;
        }

    for (int i = 0; i < n; ++ i)
        f[i] = false;

    for (int i = 0; i < n; ++ i)
        t[i] = -1;

    dist[root] = 1;
    DFSTree(root);

    Max = 0;

    for (int i = 0; i < n; ++ i)
        if (dist[i] > Max) {
            Max = dist[i];
            dest = i;
        }

    while (t[dest] != -1) {
        path.push_back(dest);

        dest = t[dest];
    }

    path.push_back(root);

    std :: reverse(path.begin(), path.end());

    return;
}

bool found;

void DFSUtil(int node, int x, int y) {
    if (node == x) {
        found = true;
        return;
    }
    else {
        if (found == false) {
            ++ op;

            f[node] = true;

            for (int i = 0; i < G[node].size(); ++ i) {
                int u = G[node][i];
                ++ op;

                if (f[u] == false && (node != y || u != x)) {
                    t[u] = node;

                    DFSUtil(u, x, y);
                }
            }
        }
    }
}

void BFSUtil(int node, int x, int y) {
    std :: queue < int > Q;

    f[node] = true;
    Q.push(node);

    while (!Q.empty()) {
        if (f[x] == true)
            break;

        int u = Q.front();

        for (int i = 0; i < G[u].size(); ++ i) {
            int v = G[u][i];

            if (f[v] == false && (u != y || v != x)) {
                f[v] = true;
                t[v] = u;
                Q.push(v);
            }
        }

        Q.pop();
    }

    return;
}

bool check(std :: vector < int > v) {
    std :: vector < int > freq(dim);

    for (int i = 0; i < n; ++ i)
        freq[i] = 0;

    for (int i = 0; i < v.size(); ++ i)
        freq[v[i]] ++;

    for (int i = 0; i < v.size(); ++ i)
        if (freq[i] > 1)
            return false;

    for (int i = 0; i + 1 < v.size(); ++ i) {
        bool q = false;

        for (int j = 0; j < G[v[i]].size(); ++ j) {
            int u = G[v[i]][j];

            if (u == v[i + 1]) {
                q = true;
                break;
            }
        }

        if (q == false)
            return false;
    }

    return true;
}

bool v;
int maxPriority;

void extendPath(int x, int y, int index) {
    for (int i = 0; i < n; ++ i)
        f[i] = false;

    for (int i = 0; i < path.size(); ++ i)
        f[path[i]] = true;

    for (int i = 0; i < n; ++ i)
        t[i] = -1;

    found = false;
    f[x] = false;
    DFSUtil(y, x, y);

    /*************/

    if (t[x] == -1)
        return;

    v = true;

    /*************/

    std :: vector < int > augm;

    while (t[x] != -1) {
        augm.push_back(x);

        x = t[x];
    }

    augm.push_back(y);

    std :: vector < int > newPath;

    for (int i = 0; i < index; ++ i)
        newPath.push_back(path[i]);

    for (int i = 0; i < augm.size(); ++ i)
        newPath.push_back(augm[i]);

    for (int i = index + 2; i < path.size(); ++ i)
        newPath.push_back(path[i]);

    path = newPath;

    return;
}

void solve(int x) {
    int k = 20;

    while (k --) {
        for (int i = 0; i < n; ++ i)
            std :: shuffle(G[i].begin(), G[i].end(), rng);

        for (int i = 0; i < n; ++ i)
            t[i] = -1;

        for (int i = 0; i < n; ++ i)
            f[i] = false;

        DFS(x);

        /***********************************/

        for (int dest = 0; dest < n; ++ dest)
            if (x != dest && t[dest] == -1)
                return;

        /***********************************/

        findDiameter();

        bool ok;

        do {
            ok = false;

            int old = path.size();

            for (int index = 0; index < path.size() - 1; ++ index) {
                int old = path.size();

                if (marked[path[index]][path[index + 1]] == 0)
                    extendPath(path[index], path[index + 1], index);

                if (old < path.size()) {
                    ok = true;

                    break;
                }
                else
                    marked[path[index]][path[index + 1]] = marked[path[index + 1]][path[index]] = 1;
            }
        } while (ok == true);

        if (res < path.size()) {
            res = path.size();

            finalPath = path;
        }

        for (int i = 0; i < path.size() - 1; ++ i)
            marked[path[i]][path[i + 1]] = marked[path[i + 1]][path[i]] = 0;

        path.clear();

        if (res == lim) return;
    }
}

double approximation_ratio;

int main() {
    fin >> T;

    while (T --) { /// This algorithm is adapted to work on multiple test-cases.
        fin >> n >> m;

        for (int i = 0, u, v; i < m; ++ i) {
            fin >> u >> v;

            G[u].push_back(v);
            G[v].push_back(u);

            degree[u] ++;
            degree[v] ++;
        }

        int artPoint = 0;

        for (int i = 0; i < n; ++ i) { /// Finding an articulation point can also be done in O(|V| + |E|).
            for (int j = 0; j < n; ++ j)
                f[j] = false;

            if (isArtPoint(i) == true) {
                artPoint = i;
                break;
            }
        }

        int cnt = 0;

        for (int i = 0; i < n; ++ i)
            cnt += (degree[i] == 1);

        if (cnt < 3)
            lim = n;
        else
            lim = n - cnt + 2;

        solve(artPoint);

        fout << res << "\n";

        for (int i = 0; i < finalPath.size(); ++ i) /// This path represents the longest path found by the algorithm.
            fout << finalPath[i] << " ";

        fout << "\n";

        if (check(finalPath) == true)
            fout << "OK!\n";
        else
            fout << "Nu reprezinta un lant valid.\n";

        approximation_ratio += (double(n) / res);

        std :: cout << double(n) / res << "\n";

        res = 0;
        finalPath.clear();

        for (int i = 0; i < n; ++ i) {
            degree[i] = 0;
            G[i].clear();
        }
    }

    return 0;
}

/**
Example:

Input:
1
5 5
0 1
0 2
1 2
3 4
0 4

Output:
5
1 2 0 4 3
OK!
**/
