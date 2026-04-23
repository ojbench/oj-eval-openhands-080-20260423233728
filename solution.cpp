#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

const int MAXN = 100005;
vector<int> adj[MAXN];
int dfn[MAXN], low[MAXN], timer;
struct Edge {
    int u, v;
};
stack<Edge> st;
bool can_be_invited[MAXN];
int color[MAXN];
vector<int> current_bcc_nodes;
vector<int> bcc_adj[MAXN];

bool is_bipartite(int u, int c) {
    color[u] = c;
    for (int v : bcc_adj[u]) {
        if (color[v] == -1) {
            if (!is_bipartite(v, 1 - c)) return false;
        } else if (color[v] == c) {
            return false;
        }
    }
    return true;
}

void find_bccs(int u, int p = -1) {
    dfn[u] = low[u] = ++timer;
    for (int v : adj[u]) {
        if (v == p) continue;
        if (dfn[v] < dfn[u]) {
            if (dfn[v] == 0) {
                st.push({u, v});
                find_bccs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u]) {
                    current_bcc_nodes.clear();
                    vector<Edge> bcc_edges;
                    while (true) {
                        Edge e = st.top();
                        st.pop();
                        bcc_edges.push_back(e);
                        current_bcc_nodes.push_back(e.u);
                        current_bcc_nodes.push_back(e.v);
                        if (e.u == u && e.v == v) break;
                    }
                    sort(current_bcc_nodes.begin(), current_bcc_nodes.end());
                    current_bcc_nodes.erase(unique(current_bcc_nodes.begin(), current_bcc_nodes.end()), current_bcc_nodes.end());
                    
                    if (current_bcc_nodes.size() >= 3) {
                        for (int node : current_bcc_nodes) {
                            bcc_adj[node].clear();
                            color[node] = -1;
                        }
                        for (const auto& e : bcc_edges) {
                            bcc_adj[e.u].push_back(e.v);
                            bcc_adj[e.v].push_back(e.u);
                        }
                        
                        if (!is_bipartite(current_bcc_nodes[0], 0)) {
                            for (int node : current_bcc_nodes) {
                                can_be_invited[node] = true;
                            }
                        }
                    }
                }
            } else {
                st.push({u, v});
                low[u] = min(low[u], dfn[v]);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        if (u == v) continue;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int i = 1; i <= n; ++i) {
        if (dfn[i] == 0) {
            find_bccs(i);
        }
    }

    int count = 0;
    for (int i = 1; i <= n; ++i) {
        if (!can_be_invited[i]) {
            count++;
        }
    }
    cout << count << endl;

    return 0;
}
