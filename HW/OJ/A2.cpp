#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <functional>

using namespace std;

class BipartiteMatchingKonig {
private:
    int n;                      // Number of vertices
    vector<vector<int>> graph;  // Adjacency list
    vector<int> color;          // Bipartite coloring: -1 (not visited), 0 (left/X), 1 (right/Y)
    vector<int> X, Y;           // Left and right vertex sets
    vector<int> matching;       // Matching information: matching[v] = u means v is matched to u
    set<int> min_vertex_cover;  // Minimum vertex cover

    // DFS function for finding augmenting paths
    std::function<bool(int, vector<bool>&, vector<bool>&)> dfs;
    
    // Function to mark reachable vertices
    std::function<void(int)> markReachable;

    // Determine bipartite partitioning (X and Y sets)
    void determineBipartiteSets() {
        color.assign(n, -1); // -1: not visited, 0: left set (X), 1: right set (Y)
        
        for (int start = 0; start < n; ++start) {
            if (color[start] != -1 || graph[start].empty()) continue;
            
            queue<int> q;
            q.push(start);
            color[start] = 0; // Start with left set
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                
                for (int v : graph[u]) {
                    if (color[v] == -1) {
                        color[v] = 1 - color[u]; // Assign opposite color
                        q.push(v);
                    }
                }
            }
        }
        
        // Collect vertices in each set
        X.clear();
        Y.clear();
        for (int i = 0; i < n; ++i) {
            if (color[i] == 0) {
                X.push_back(i);
            } else if (color[i] == 1) {
                Y.push_back(i);
            }
        }
    }
    
    // Find maximum bipartite matching
    void findMaximumMatching() {
        matching.assign(n, -1);
        
        // Define DFS function for finding augmenting paths
        dfs = [&](int u, vector<bool>& visited, vector<bool>& inQueue) -> bool {
            visited[u] = true;
            inQueue[u] = true;
            
            for (int v : graph[u]) {
                // Only consider edges from X to Y
                if (color[u] == color[v]) continue;
                
                // If v is not matched or we can find an augmenting path from the vertex matched to v
                if (matching[v] == -1 || (!inQueue[matching[v]] && dfs(matching[v], visited, inQueue))) {
                    matching[v] = u;
                    matching[u] = v;
                    return true;
                }
            }
            
            return false;
        };
        
        // Find augmenting paths
        for (int i : X) {
            vector<bool> visited(n, false);
            vector<bool> inQueue(n, false);
            dfs(i, visited, inQueue);
        }
    }
    
    // Find minimum vertex cover using König's theorem
    void findMinimumVertexCover() {
        // Step 1: Find a set Z of unmatched vertices in X
        set<int> Z;
        for (int u : X) {
            if (matching[u] == -1) {
                Z.insert(u);
            }
        }
        
        // Step 2: Find all vertices reachable from Z via alternating paths
        vector<bool> reachable(n, false);
        
        // Declare markReachable function
        markReachable = [&](int u) -> void {
            reachable[u] = true;
            for (int v : graph[u]) {
                if (color[v] == 1 && !reachable[v]) { // v is in Y
                    reachable[v] = true;
                    if (matching[v] != -1 && !reachable[matching[v]]) {
                        markReachable(matching[v]);
                    }
                }
            }
        };
        
        for (int u : Z) {
            if (!reachable[u]) {
                markReachable(u);
            }
        }
        
        // Step 3: Construct minimum vertex cover
        // Take all vertices in X that are not reachable and all vertices in Y that are reachable
        min_vertex_cover.clear();
        for (int u : X) {
            if (!reachable[u]) {
                min_vertex_cover.insert(u);
            }
        }
        for (int v : Y) {
            if (reachable[v]) {
                min_vertex_cover.insert(v);
            }
        }
    }

public:
    // Constructor
    BipartiteMatchingKonig(int num_vertices) : n(num_vertices), graph(num_vertices) {}
    
    // Add an edge to the graph
    void addEdge(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    
    // Build graph from edge list
    void buildGraph(const vector<pair<int, int>>& edges) {
        for (const auto& edge : edges) {
            addEdge(edge.first, edge.second);
        }
    }
    
    // Solve the matching and vertex cover problems
    void solve() {
        determineBipartiteSets();
        findMaximumMatching();
        findMinimumVertexCover();
    }
    
    // Get maximum matching size
    int getMaxMatchingSize() {
        int size = 0;
        for (int i : X) {
            if (matching[i] != -1) {
                size++;
            }
        }
        return size;
    }
    
    // Get the matched edges
    vector<pair<int, int>> getMatchedEdges() {
        vector<pair<int, int>> matched_edges;
        for (int u : X) {
            if (matching[u] != -1) {
                matched_edges.push_back({u, matching[u]});
            }
        }
        return matched_edges;
    }
    
    // Get minimum vertex cover size
    int getMinVertexCoverSize() {
        return min_vertex_cover.size();
    }
    
    // Get minimum vertex cover
    set<int> getMinVertexCover() {
        return min_vertex_cover;
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    
    vector<pair<int, int>> edges;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        edges.push_back({u, v});
    }
    
    BipartiteMatchingKonig solver(n);
    solver.buildGraph(edges);
    solver.solve();
    
    int max_matching_size = solver.getMaxMatchingSize();
    vector<pair<int, int>> matched_edges = solver.getMatchedEdges();
    int min_cover_size = solver.getMinVertexCoverSize();
    set<int> min_cover = solver.getMinVertexCover();
    
    // Output the results
    cout << max_matching_size << endl;
    for (const auto& edge : matched_edges) {
        cout << edge.first << " " << edge.second << endl;
    }
    cout << min_cover_size << endl;
    for (int vertex : min_cover) {
        cout << vertex << " ";
    }
    cout << endl;
    
    return 0;
}