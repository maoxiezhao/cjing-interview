
#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <iostream>

using namespace std;

static const int V = 5;
int G[V][V] = {
  {0, 9, 75, 0, 0},
  {9, 0, 95, 19, 42},
  {75, 95, 0, 51, 66},
  {0, 19, 51, 0, 31},
  {0, 42, 66, 31, 0}
};

void InsertIntoTree(int row, int col);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Prim's algorithm
// 1.Initialize the minimum spanning tree with a vertex chosen at random.
// 2.Find all the edges that connect the tree to new vertices, find the minimum and add it to the tree
// 3.Keep repeating step 2 until we get a minimum spanning tree

void MinimumSpanningPrim()
{
    int selected[V];
    memset(selected, false, sizeof(bool) * V);
    
    // the number of egde in minimum spanning tree (V - 1)
    int edgeCount = 0; 
    // choose 0th vertex and make it true
    selected[0] = true;

    while (edgeCount < V - 1)
    {
        int min = INT_MAX;
        int x = 0;
        int y = 0;

        for (int i = 0; i < V; i++) 
        {
            if (selected[i])
            {
                for (int j = 0; j < V; j++) 
                {
                    if(selected[j] || G[i][j] == 0) {
                        continue;
                    }

                    if (min > G[i][j])
                    {
                        min = G[i][j];
                        x = i;
                        y = j;
                    }
                }
            }
        };

        InsertIntoTree(x, y);

        selected[y] = true;
        edgeCount++;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Kruskal's Algorithm
// 1.Sort all the edges from low weight to high
// 2.Take the edge with the lowest weight and add it to the spanning tree. If adding the edge created a cycle, then reject this edge.
// 3.Keep adding edges until we reach all vertices.

// Kruskal's algorithm in C++


#define edge pair<int, int>

class Graph
{
private:
    vector<pair<int, edge> > G;  // graph
    // number of vertices/nodes in graph
    int V; 
    int *parent;

public:
    Graph(int V);
    void AddWeightedEdge(int u, int v, int w);
    int  FindSet(int i);
    void UnionSet(int u, int v);
    void Kruskal(vector<pair<int, edge>>& T);
    void print();
};

Graph::Graph(int V) 
{
    parent = new int[V];

    //i 0 1 2 3 4 5
    //parent[i] 0 1 2 3 4 5
    for (int i = 0; i < V; i++) {
        parent[i] = i;
    }

    G.clear();
}

void Graph::AddWeightedEdge(int u, int v, int w) {
    G.push_back(make_pair(w, edge(u, v)));
}

int Graph::FindSet(int i) {
    // If i is the parent of itself
    if (i == parent[i])
        return i;
    else
    // Else if i is not the parent of itself
    // Then i is not the representative of his set,
    // so we recursively call Find on its parent
        return FindSet(parent[i]);
}

void Graph::UnionSet(int u, int v) {
    parent[u] = parent[v];
}

void Graph::Kruskal(vector<pair<int, edge>>& T)
{
    int i, uRep, vRep;
    std::sort(G.begin(), G.end());  // increasing weight
    for (i = 0; i < G.size(); i++) 
    {
        uRep = FindSet(G[i].second.first);
        vRep = FindSet(G[i].second.second);
        if (uRep != vRep) 
        {
            T.push_back(G[i]);      // add to tree
            UnionSet(uRep, vRep);
        }
    }
}
