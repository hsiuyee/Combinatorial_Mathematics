#include <algorithm>
#include <vector>
#include <iostream>
#include <chrono>
using namespace std;
using ll = long long;

// ---------- Hungarian Algorithm as a class ----------
class Hungarian {
private:
    int n;
    vector<vector<ll>> a;
    vector<ll> u, v, minv;
    vector<int> p, way, match;

public:
    Hungarian(int _n)
        : n(_n),
          a(n+1, vector<ll>(n+1)),
          u(n+1), v(n+1), minv(n+1),
          p(n+1), way(n+1), match(n+1) {}

    // set cost for 0-indexed i, j
    void setCost(int i, int j, ll cost) {
        a[i+1][j+1] = cost;
    }

    // solve returns minimal total cost
    ll solve() {
        const ll INF = (ll)1e18;
        for (int i = 1; i <= n; ++i) {
            p[0] = i;
            int j0 = 0;
            fill(minv.begin(), minv.end(), INF);
            vector<char> used(n+1, false);

            // replace do-while with while(true) + break
            while (true) {
                used[j0] = true;
                int i0 = p[j0];
                int j1 = 0;
                ll delta = INF;
                // find minimal slack
                for (int j = 1; j <= n; ++j) {
                    if (!used[j]) {
                        ll cur = a[i0][j] - u[i0] - v[j];
                        if (cur < minv[j]) {
                            minv[j] = cur;
                            way[j] = j0;
                        }
                        if (minv[j] < delta) {
                            delta = minv[j];
                            j1 = j;
                        }
                    }
                }
                // update potentials
                for (int j = 0; j <= n; ++j) {
                    if (used[j]) {
                        u[p[j]] += delta;
                        v[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
                if (p[j0] == 0) break;
            }

            // augmenting path: replace do-while with while
            while (j0 != 0) {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            }
        }

        // build match
        for (int j = 1; j <= n; ++j) {
            match[p[j]] = j;
        }

        // total cost is -v[0]
        return -v[0];
    }

    // getMatch returns vector of size n: match[i] = matched j (0-indexed)
    vector<int> getMatch() const {
        vector<int> res(n);
        for (int i = 1; i <= n; ++i) {
            res[i-1] = match[i] - 1;
        }
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    Hungarian hung(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            ll w;
            cin >> w;
            hung.setCost(i, j, w);
        }
    }

    ll totalCost = hung.solve();
    auto matching = hung.getMatch();

    cout << totalCost << '\n';
    for (int i = 0; i < n; ++i) {
        cout << i << " " << matching[i] << '\n';
    }
    return 0;
}
