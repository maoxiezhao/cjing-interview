#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

using namespace std;

// https://www.programiz.com/dsa/graph

// Struct for the edges of the graph
struct Node
{
    int key;
};


struct Edge 
{
    Node* u;  //start vertex of the edge
    Node* v;  //end vertex of the edge
    int w;  //w of the edge (u,v)
};

// Graph - it consists of edges
struct Graph 
{
    int V;        // Total number of vertices in the graph
    int E;        // Total number of edges in the graph
    struct Edge* edge;  // Array of edges
    struct Node* nodes;

    Graph(int v, int e)
    {
        V = v;
        E = e;
        edge = new Edge[e];
        nodes = new Node[v];
    }
    ~Graph()
    {
        delete[] edge;
        delete[] nodes;
    }

    int Distance(Node* n1, Node* n2)const
    {
        for(int i = 0; i < E; i++)
        {
            Edge& e = edge[i];
            if ((n1 == e.u && n2 == e.v) ||
                (n1 == e.v && n2 == e.u)) {
                return e.w;
            }
        }
        return -1;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////
// bellman-ford
// https://www.programiz.com/dsa/bellman-ford-algorithm
void BellmanFord(const Graph& graph, int u)
{
    const int V = graph.V;
    const int E = graph.E;
    vector<int> dist(V);

    // Step 1: fill the distance array and predecessor array
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
    }

    // Step 2: relax edges |V| - 1 times
    for (int i = 1; i < V; i++) 
    {
        for (int j = 0; j < E; j++) 
        {
            int u = graph.edge[j].u->key;
            int v = graph.edge[j].v->key;
            int w = graph.edge[j].w;

            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    // Step 3: detect negative cycle
    // if value changes then we have a negative cycle in the graph
    // and we cannot find the shortest distances
    for (int i = 0; i < E; i++) {
        int u = graph.edge[i].u->key;
        int v = graph.edge[i].v->key;
        int w = graph.edge[i].w;
        if (dist[u] != INT_MAX && dist[u] + w < dist[v])
         {
            printf("Graph contains negative w cycle");
            return;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
// Dijkstras
// https://www.programiz.com/dsa/dijkstra-algorithm
Node* ExtractSmallest(vector<Node*>& nodes, const vector<int>& distanceFromStart)
{
    if (nodes.empty()) {
        return nullptr;
    }

    Node* smallest = nodes[0];
    int smallestPos = 0;
    for(int i = 1; i < nodes.size(); i++)
    {
        Node* node = nodes[i];
        if (distanceFromStart[node->key] < distanceFromStart[smallest->key])
        {
            smallest = node;
            smallestPos = i;
        }
    }

    nodes.erase(nodes.begin() + smallestPos);
    return smallest;
}

vector<Node*> AdjacentRemainingNodes(const Graph& graph, vector<Node*>& nodes, Node* node)
{   
    vector<Node*> adjacentNodes;
    const int E = graph.E;
    for(int i = 0; i < E; i++)
    {
        Node* adjacent = nullptr;
        Edge& edge = graph.edge[i];
        if (edge.u == node) {
            adjacent = edge.v;
        } 
        else if (edge.v == node) {
            adjacent = edge.u;
        }

        if (adjacent && std::find(nodes.begin(), nodes.end(), adjacent) != nodes.end()) {
            adjacentNodes.push_back(adjacent);
        }
    }

    return adjacentNodes;
}

void Dijkstras(const Graph& graph, int start)
{
    // base on greedy algorithm

    // setup
    const int V = graph.V;
    vector<Node*> nodes(V);
    vector<int> distanceFromStart(V);
    vector<int> prevNodes(V);
    for(int i = 0; i < V; i++) 
    {
        nodes[i] = &graph.nodes[i];
        distanceFromStart[i] = INT_MAX;
        prevNodes[i] = -1;
    }
    distanceFromStart[start] = 0;

    while (nodes.size() > 0) 
    {
        // 1. 从剩余节点中选出最近的节点
        Node* smallest = ExtractSmallest(nodes, distanceFromStart);

        // 2. 获取该节点所有的邻接节点（从剩余节点中选）
        vector<Node*> adjacentNodes = AdjacentRemainingNodes(graph, nodes, smallest);
        if (adjacentNodes.empty()) {
            continue;
        }  

        // 3. 遍历所有邻接，更新他们的最近距离和最近的前置节点
        const int size = adjacentNodes.size();
        for (int i = 0; i < size; ++i) 
        {
            Node* adjacent = adjacentNodes.at(i);
            int distance = graph.Distance(smallest, adjacent) + distanceFromStart[smallest->key];

            if (distance < distanceFromStart[adjacent->key]) 
            {
                distanceFromStart[adjacent->key] = distance;
                prevNodes[adjacent->key] = smallest->key;
            }
        }
    }
}