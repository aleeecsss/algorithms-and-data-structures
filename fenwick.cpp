/// This data structures applies the following updates / queries online on a vector, in O(log n) time:

/// [1] : Efficiently adds a value C to all elements situated on a contiguous subarray [x, y].
/// [2] : Queries what's the sum of all elements on a contiguous subarray [x, y].

/// How to use:

/// See line 100-131.

/// VERY IMPORTANT! Only works if you are working on 1-indexed vector.

#include <bits/stdc++.h>

template < typename T >
    class Fenwick {
        private:
            int n;
            std :: vector < T > AIB1, AIB2;
        public:
            T query1(int pos) {
                if (pos == 0)
                    return 0;

                T ret = 0;

                for (int i = pos; i >= 1; i -= (i & (- i)))
                    ret += AIB1[i];

                return ret;
            }

            T query2(int pos) {
                if (pos == 0)
                    return 0;

                T ret = 0;

                for (int i = pos; i >= 1; i -= (i & (- i)))
                    ret += AIB2[i];

                return ret;
            }

            void update1(int pos, T val) {
                for (int i = pos; i <= n; i += (i & (- i)))
                    AIB1[i] += val;

                return;
            }

            void update2(int pos, T val) {
                for (int i = pos; i <= n; i += (i & (- i)))
                    AIB2[i] += val;

                return;
            }

            void Update(int x, int y, T val) {
                if (x > y)
                    std :: swap(x, y);

                update1(x, +val);
                update1(y + 1, -val);

                update2(x, +x * val);
                update2(y + 1, -(y + 1) * val);

                return;
            }

            T Query(int x, int y) {
                if (x > y)
                    std :: swap(x, y);

                T A = (y + 1) * query1(y) - query2(y);
                T B = (x) * query1(x - 1) - query2(x - 1);

                return (A - B);
            }

            Fenwick (std :: vector < T > v) {
                n = v.size();

                for (int i = 0; i <= n; ++ i) {
                    AIB1.push_back(0);
                    AIB2.push_back(0);
                }

                for (int i = 0; i < n; ++ i)
                    Update(i + 1, i + 1, v[i]);

                return;
            }
    };

std :: ifstream fin("fenwick.in");
std :: ofstream fout("fenwick.out");

int main() {
    int n, Q;
    std :: vector < long long > a;

    fin >> n;

    for (int i = 0, x; i < n; ++ i) {
        fin >> x;

        a.push_back(x);
    }

    Fenwick < long long > T(a); /// This data structure transforms the 0-indexed vector into a 1-indexed vector.

    fin >> Q;

    while (Q --) {
        char c; /// Can take the following symbols: "+", "?".
        int x, y, C;

        fin >> c;

        if (c == '+') {
            fin >> x >> y >> C;

            T.Update(x, y, C);
        }
        else {
            fin >> x >> y;

            fout << T.Query(x, y) << "\n";
        }
    }

    return 0;
}

/**
Example:

Input:
5
1 2 3 4 5
6
+ 1 3 3
+ 2 5 6
+ 1 5 2
? 2 3
+ 2 4
? 1 5

Output:
27
61
**/
