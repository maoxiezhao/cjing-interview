#include <stack>
#include <vector>

// 算法思路就是利用DFS,在每次取出节点时，都优先先走非桥点

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
    bool **connected = nullptr;

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

    void RemoveEdge(int v1, int v2)
    {
    }

    bool CheckConnected(int v1, int v2)
    {
        return connected[v1][v2];
    }
};

void Dfs(std::stack<int>& vertexStack, Graph graph, std::vector<int> path, int vertex)
{
    vertexStack.push(vertex);
    for(int i = 0; i < graph.V; i++)
    {
        if (graph.CheckConnected(vertex, i))
        {
            graph.RemoveEdge(vertex, i);
            Dfs(vertexStack, graph, path, i);
            break;
        }
    }
}

void Fleury(std::stack<int>& vertexStack, Graph graph, std::vector<int> path)
{
    while (!vertexStack.empty())
    {
        int vertex = vertexStack.top();
        vertexStack.pop();

        bool isBridge = true;
        for(int i = 0; i < graph.V; i++)
        {
            if (graph.CheckConnected(vertex, i))
            {
                isBridge = false;
                break;
            }
        }

        if (isBridge)
            path.push_back(vertex);
        else
            Dfs(vertexStack, graph, path, vertex);
    }
}