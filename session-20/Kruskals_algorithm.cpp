// 24BCS11540 (Krissh Chhabra)

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge
{
    int u, v, weight;
};

bool compare(Edge a, Edge b)
{
    return a.weight < b.weight;
}

int findParent(vector<int> &parent, int i)
{
    if (parent[i] == -1) return i;
    return findParent(parent, parent[i]);
}

void unionSets(vector<int> &parent, int x, int y)
{
    parent[x] = y;
}

int main()
{
    int V, E;
    cout << "Enter the number of vertices and edges: ";
    cin >> V >> E;

    vector<Edge> edges(E);
    cout << "Enter the edges (u v weight):" << endl;
    for (int i = 0; i < E; i++)
    {
        cin >> edges[i].u >> edges[i].v >> edges[i].weight;
    }

    sort(edges.begin(), edges.end(), compare);

    vector<int> parent(V, -1);
    vector<Edge> mstEdges;

    for (const auto &edge : edges)
    {
        int x = findParent(parent, edge.u);
        int y = findParent(parent, edge.v);

        if (x != y) {
            mstEdges.push_back(edge);
            unionSets(parent, x, y);
        }
    }

    cout << "Edges in the Minimum Spanning Tree:" << endl;
    for (int i = 0; i < mstEdges.size(); i++) {
        Edge edge = mstEdges[i];
        cout << edge.u << " - " << edge.v << " (Weight: " << edge.weight << ")" << endl;
    }

    return 0;
}