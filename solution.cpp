#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <stack>
using namespace std;

vector<vector<int>> adj;
vector<int> disc, low;
vector<bool> visited;
vector<int> component;
vector<bool> can_be_invited;
int timer, n;

vector<int> vertex_stack;
vector<vector<int>> biconnected_components;

void dfs(int u, int parent) {
    visited[u] = true;
    disc[u] = low[u] = ++timer;
    vertex_stack.push_back(u);
    int children = 0;
    
    for (int v : adj[u]) {
        if (v == parent) continue;
        
        if (visited[v]) {
            low[u] = min(low[u], disc[v]);
        } else {
            children++;
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            
            if ((parent == -1 && children > 1) || (parent != -1 && low[v] >= disc[u])) {
                // u is an articulation point, extract biconnected component
                vector<int> component;
                while (true) {
                    int w = vertex_stack.back();
                    vertex_stack.pop_back();
                    component.push_back(w);
                    if (w == v) break;
                }
                component.push_back(u);
                biconnected_components.push_back(component);
            }
        }
    }
}

bool is_bipartite_subgraph(const vector<int>& vertices) {
    vector<int> color;
    vector<int> vertex_to_index;
    int k = vertices.size();
    
    color.resize(k, -1);
    vertex_to_index.resize(n + 1, -1);
    
    for (int i = 0; i < k; i++) {
        vertex_to_index[vertices[i]] = i;
    }
    
    for (int i = 0; i < k; i++) {
        if (color[i] == -1) {
            queue<int> q;
            q.push(i);
            color[i] = 0;
            
            while (!q.empty()) {
                int u_idx = q.front();
                q.pop();
                int u = vertices[u_idx];
                
                for (int v : adj[u]) {
                    if (vertex_to_index[v] == -1) continue;
                    
                    int v_idx = vertex_to_index[v];
                    if (color[v_idx] == -1) {
                        color[v_idx] = 1 - color[u_idx];
                        q.push(v_idx);
                    } else if (color[v_idx] == color[u_idx]) {
                        return false; // Not bipartite
                    }
                }
            }
        }
    }
    
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int m;
    cin >> n >> m;
    
    adj.resize(n + 1);
    disc.resize(n + 1, 0);
    low.resize(n + 1, 0);
    visited.resize(n + 1, false);
    can_be_invited.resize(n + 1, false);
    timer = 0;
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    // Find biconnected components using Tarjan's algorithm
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            dfs(i, -1);
            if (!vertex_stack.empty()) {
                biconnected_components.push_back(vertex_stack);
                vertex_stack.clear();
            }
        }
    }
    
    // For each biconnected component, check if it's non-bipartite
    for (const auto& comp : biconnected_components) {
        if (comp.size() >= 3 && !is_bipartite_subgraph(comp)) {
            // This component contains odd cycles
            for (int vertex : comp) {
                can_be_invited[vertex] = true;
            }
        }
    }
    
    // Count vertices that cannot be invited
    int answer = 0;
    for (int i = 1; i <= n; i++) {
        if (!can_be_invited[i]) {
            answer++;
        }
    }
    
    cout << answer << endl;
    
    return 0;
}