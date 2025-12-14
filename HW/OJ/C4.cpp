#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <tuple>
#include <algorithm>

using namespace std;
using ll = long long;

// Residual‐edge structure
struct Edge {
    int to;       // endpoint
    ll cap;       // remaining capacity
    int rev;      // index of reverse edge in G[to]
};

class MaxFlow {
    int n;                          // number of vertices
    vector<vector<Edge>> G;         // adjacency list of residual graph

public:
    // initialize with n vertices (0…n-1)
    MaxFlow(int _n) : n(_n), G(n) {}

    // add a directed edge u→v with capacity c
    void addEdge(int u, int v, ll c) {
        G[u].push_back({v, c, static_cast<int>(G[v].size())});
        G[v].push_back({u, 0, static_cast<int>(G[u].size()) - 1});
    }

    // compute max-flow from s to t
    ll maxFlow(int s, int t) {
        ll flow = 0;
        const ll INF = LLONG_MAX;
        while (true) {
            vector<int> prev_v(n, -1), prev_e(n, -1);
            queue<int> q;
            prev_v[s] = s;
            q.push(s);
            // BFS to find shortest augmenting path
            while (!q.empty() && prev_v[t] == -1) {
                int u = q.front(); q.pop();
                for (int i = 0; i < static_cast<int>(G[u].size()); i++) {
                    Edge &e = G[u][i];
                    if (e.cap > 0 && prev_v[e.to] == -1) {
                        prev_v[e.to] = u;
                        prev_e[e.to] = i;
                        q.push(e.to);
                        if (e.to == t) break;
                    }
                }
            }
            if (prev_v[t] == -1) break;   // no more augmenting path

            // find bottleneck capacity
            ll aug = INF;
            for (int v = t; v != s; v = prev_v[v]) {
                Edge &e = G[prev_v[v]][prev_e[v]];
                aug = min(aug, e.cap);
            }
            // apply flow
            for (int v = t; v != s; v = prev_v[v]) {
                Edge &e = G[prev_v[v]][prev_e[v]];
                e.cap -= aug;
                G[v][e.rev].cap += aug;
            }
            flow += aug;
        }
        return flow;
    }

    // after maxFlow, find reachable vertices from s in residual graph
    // returns a bool array `cut` where cut[u] = true ⇔ u is on the s‐side
    vector<bool> minCut(int s) {
        vector<bool> seen(n, false);
        queue<int> q;
        seen[s] = true;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : G[u]) {
                if (e.cap > 0 && !seen[e.to]) {
                    seen[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        return seen;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    int s, t;
    cin >> s >> t;

    MaxFlow mf(n);
    vector<tuple<int,int,ll>> edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        ll c;
        cin >> u >> v >> c;
        // undirected edge: add both directions
        mf.addEdge(u, v, c);
        mf.addEdge(v, u, c);
        edges.emplace_back(u, v, c);
    }

    ll max_flow = mf.maxFlow(s, t);
    cout << max_flow << "\n";

    auto cut = mf.minCut(s);
    // any original edge crossing the cut is in the min‐cut
    for (auto &e : edges) {
        int u, v;
        ll c;
        tie(u, v, c) = e;
        if (cut[u] && !cut[v]) {
            cout << u << " " << v << "\n";
        } else if (cut[v] && !cut[u]) {
            cout << v << " " << u << "\n";
        }
    }

    return 0;
}
