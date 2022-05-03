#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>

struct Node {
    int id = 0;
    std::string name;
};
using Nodes = std::vector<Node>;

struct Edge {
    int u = 0;
    int v = 0;
};
using Edges = std::vector<Edge>;

class Graph {
public:
    Graph(int V): V(V) {
        nodes.resize(V, Node());
        adjList.resize(V, Edges());
    }

    void addNode(const std::string &name) {
        Node node {id, name};
        nodes[id] = node;
        nodeMap[name] = node;
        ++id;
    }

    void addEdge(const std::string& u, const std::string &v){
        addEdge(nodeMap[u].id, nodeMap[v].id);
    }

    void addEdge(int u, int v) {
        Edge edge{u ,v};
        adjList[u].emplace_back(edge);
        edgeVec.emplace_back(edge);
    }

    Edges& edges() { return edgeVec; }
    
    Edges& edges(int u) {
        return adjList[u];
    }

    Edges edges (const std::string &u) {
        return edges(nodeMap[u].id);
    }
    
    int getV () { return V; }

    Nodes& getNodes () {
        return nodes;
    }

    Node node(int u) {
        return nodes[u];
    }

    void printEdges() {
        for (auto &e: edgeVec) {
            std::cout << nodes[e.u].name << "->" << nodes[e.v].name << std::endl;
        }
    }
private:
    int V = 0;
    int id = 0;
    Nodes nodes;
    std::unordered_map<std::string, Node> nodeMap;
    Edges edgeVec;
    std::vector<Edges> adjList;
};
using Graphs = std::vector<Graph>;

struct DFS {
    enum class COLOR {
        WHITE = 0,
        GRAY = 1,
        BLACK = 2
    };

public:
    DFS(Graph &graph) :graph(graph) {
        int v = graph.getV();
        color.resize(v, COLOR::WHITE);
        tree.resize(v, -1);
        startTime.resize(v, 0);
        finishTime.resize(v, 0);
    }

    void operator() () {
        for (auto &node : graph.getNodes()) {
            int u  = node.id;
            if(color[u] == COLOR::WHITE) {
                dfs(u);
            }
        }
    }

    void dfs(int u) {
        stack.push(u);
        while(!stack.empty()) {
            int u = stack.top();
            if (color[u] == COLOR::WHITE) {
                color[u] = COLOR::GRAY;
                startTime[u] = ++time;
                for (auto &edge : graph.edges(u)) {
                    int v = edge.v;
                    if (color[v] == COLOR::WHITE) {
                        tree[v] = u;
                        stack.push(v);
                    }
                }
            }
            else {
                stack.pop();
                if (color[u] == COLOR::GRAY) {
                    color[u] = COLOR::BLACK;
                    finishTime[u] = ++time;
                }
            }
        }
    }


    Graph &graph;
    std::vector<COLOR> color;
    std::vector<int> tree;
    std::vector<int> startTime;
    std::vector<int> finishTime;
    std::stack<int> stack;
    int time = 0;
};

class StronglyConnectedComponents {
public: 
    StronglyConnectedComponents(Graph &graph) : graph(graph) {}
    std::vector<std::vector<Node>> compute() {
        DFS dfs(graph);
        dfs();

        Graph graphT = getGraphTranspose(graph);
        Nodes nodes = graphT.getNodes();
        std::vector<int> finishTime = dfs.finishTime;

        auto cmp = [&] (auto &n1, auto &n2) {
            return finishTime[n1.id] > finishTime[n2.id];
        };

        std::sort(std::begin(nodes), std::end(nodes), cmp);

        std::vector<std::vector<Node>> sccs;
        std::vector<bool> visited(graphT.getV(), false);
        for (auto &node : nodes) {
            int u = node.id;
            std::vector<Node> scc;
            if (!visited[u]) {
                dfsLoop(u, graphT, visited, scc);
                sccs.emplace_back(scc);
            };

        }

        return sccs;
    }

    void dfsLoop(int u, Graph& graphT, std::vector<bool>& visited, std::vector<Node> &scc) {
        std::stack<int> stack;
        stack.push(u);
        visited[u] = true;
        scc.emplace_back(graphT.node(u));
        while(!stack.empty()) {
            int u = stack.top();
            stack.pop();
            for (auto &edge: graphT.edges(u)) {
                int v = edge.v;
                if (!visited[v]) {
                    visited[v]= true;
                    stack.push(v);
                    scc.emplace_back(graphT.node(v));
                }
            }
        }
    }

private:
    Graph getGraphTranspose(Graph &graph) {
        Graph graphT(graph.getV());
        for (auto &node: graph.getNodes()) {
            graphT.addNode(node.name);
        }

        for (auto &edge : graph.edges()) {
            graphT.addEdge(edge.v, edge.u);
        }

        return graphT;
    }

    Graph &graph;
};

int main() {
    std::cout << "Strongly Connected Components" << std::endl;
    Graph graph(8);
    graph.addNode("a");
    graph.addNode("b");
    graph.addNode("c");
    graph.addNode("d");
    graph.addNode("e");
    graph.addNode("f");
    graph.addNode("g");
    graph.addNode("h");

    graph.addEdge("a", "b");
    graph.addEdge("b", "c");
    graph.addEdge("b", "f");
    graph.addEdge("b", "e");
    graph.addEdge("c", "d");
    graph.addEdge("c", "g");
    graph.addEdge("d", "c");
    graph.addEdge("d", "h");
    graph.addEdge("e", "a");
    graph.addEdge("e", "f");
    graph.addEdge("f", "g");
    graph.addEdge("g", "f");
    graph.addEdge("g", "h");
    graph.addEdge("h", "h");

    StronglyConnectedComponents scc(graph);
    std::vector<std::vector<Node>> sccs = scc.compute();
    for (auto &scc: sccs) {
        for (auto node: scc) {
            std::cout << node.name << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}