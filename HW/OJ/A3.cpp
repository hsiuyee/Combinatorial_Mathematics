#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int MAXN = 505; // Maximum number of vertices

int n, m;
vector<int> g[MAXN];  // Adjacency list representation of the graph
int match[MAXN], p[MAXN], base[MAXN];
bool used[MAXN], blossom[MAXN];

// Find the lowest common ancestor (LCA) of two vertices in the alternating tree
int lca(int a, int b) {
    vector<bool> usedLocal(n, false);
    while (true) {
        a = base[a];
        usedLocal[a] = true;
        if (match[a] == -1) break;
        a = p[match[a]];
    }
    while (true) {
        b = base[b];
        if (usedLocal[b])
            return b;
        b = p[match[b]];
    }
}

// Mark the path for blossom contraction
void markPath(int v, int b, int x) {
    while (base[v] != b) {
        blossom[base[v]] = blossom[base[match[v]]] = true;
        p[v] = x;
        x = match[v];
        v = p[match[v]];
    }
}

// Search for an augmenting path starting from 'start'
// If an augmenting path is found, update the matching and return 1, otherwise return 0
int findPath(int start) {
    // Initialize auxiliary arrays
    for (int i = 0; i < n; i++) {
        used[i] = false;
        p[i] = -1;
        base[i] = i;
    }
    queue<int> q;
    q.push(start);
    used[start] = true;
    
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int u : g[v]) {
            // Skip if u and v belong to the same blossom or if u is the matched partner of v
            if (base[v] == base[u] || match[v] == u)
                continue;
            // If we found a cycle or the alternating tree already contains a path from u
            if (u == start || (match[u] != -1 && p[match[u]] != -1)) {
                int cur = lca(v, u);
                fill(blossom, blossom + n, false);
                markPath(v, cur, u);
                markPath(u, cur, v);
                for (int i = 0; i < n; i++) {
                    if (blossom[base[i]]) {
                        base[i] = cur;
                        if (!used[i]) {
                            used[i] = true;
                            q.push(i);
                        }
                    }
                }
            } else if (p[u] == -1) {
                p[u] = v;
                // If u is not matched, an augmenting path is found
                if (match[u] == -1) {
                    int cur = u;
                    while (cur != -1) {
                        int pv = p[cur];
                        int w = match[pv];
                        match[cur] = pv;
                        match[pv] = cur;
                        cur = w;
                    }
                    return 1;
                } else {
                    used[match[u]] = true;
                    q.push(match[u]);
                }
            }
        }
    }
    return 0;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> m;
    for (int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        // The vertices are numbered from 0 to n-1
        g[u].push_back(v);
        g[v].push_back(u);
    }
    
    // Initialize matching results
    fill(match, match + n, -1);
    int res = 0;
    // Try to find an augmenting path for each unmatched vertex
    for (int i = 0; i < n; i++){
        if (match[i] == -1)
            res += findPath(i);
    }
    
    // Output the size of the matching (each matching edge is counted once)
    cout << res << "\n";
    // Output each matching edge (print each edge only once)
    for (int i = 0; i < n; i++){
        if (match[i] != -1 && i < match[i])
            cout << i << " " << match[i] << "\n";
    }
    return 0;
}
