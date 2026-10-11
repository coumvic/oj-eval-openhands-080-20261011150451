#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

vector<vector<int>> adj;
vector<int> color;
vector<bool> visited;
vector<int> degree;
vector<int> component;
vector<bool> in_non_bipartite_component;

bool is_bipartite_component(int start, int comp_id) {
    queue<int> q;
    q.push(start);
    color[start] = 0;
    component[start] = comp_id;
    bool bipartite = true;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v : adj[u]) {
            if (component[v] == -1) {
                component[v] = comp_id;
                color[v] = 1 - color[u];
                q.push(v);
            } else if (color[v] == color[u]) {
                bipartite = false;
            }
        }
    }
    
    return bipartite;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    adj.resize(n + 1);
    color.resize(n + 1, -1);
    visited.resize(n + 1, false);
    degree.resize(n + 1, 0);
    component.resize(n + 1, -1);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        degree[u]++;
        degree[v]++;
    }
    
    // Find connected components and check if each is bipartite
    vector<bool> component_is_bipartite;
    int comp_count = 0;
    
    for (int i = 1; i <= n; i++) {
        if (component[i] == -1) {
            bool bipartite = is_bipartite_component(i, comp_count);
            component_is_bipartite.push_back(bipartite);
            comp_count++;
        }
    }
    
    // Mark vertices in non-bipartite components
    in_non_bipartite_component.resize(n + 1, false);
    for (int i = 1; i <= n; i++) {
        if (!component_is_bipartite[component[i]]) {
            in_non_bipartite_component[i] = true;
        }
    }
    
    // Find 2-core for each non-bipartite component
    vector<bool> can_be_invited(n + 1, false);
    queue<int> q;
    
    // Start with vertices of degree 1 in non-bipartite components
    for (int i = 1; i <= n; i++) {
        if (in_non_bipartite_component[i] && degree[i] <= 1) {
            q.push(i);
        }
    }
    
    // Remove vertices of degree <= 1 iteratively
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        in_non_bipartite_component[u] = false;
        
        for (int v : adj[u]) {
            if (in_non_bipartite_component[v]) {
                degree[v]--;
                if (degree[v] <= 1) {
                    q.push(v);
                }
            }
        }
    }
    
    // Count vertices that cannot be invited
    int answer = 0;
    for (int i = 1; i <= n; i++) {
        if (!in_non_bipartite_component[i]) {
            answer++;
        }
    }
    
    cout << answer << endl;
    
    return 0;
}