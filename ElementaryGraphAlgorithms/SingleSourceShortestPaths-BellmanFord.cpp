#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <limits>

struct Node {
    int id = 0;
    std::string data;
};
using Nodes = std::vector<Node>;

struct Edge {
    int u = 0;
    int v = 0;
    int w = 0;
};
using Edges = std::vector<Edge>;

class Graph {
public:
    Graph(int V) : V(V) {
        nodes.resize(V, Node());
        adjList.resize(V, Edges());
        
    }

    void addNode(const std::string &name) {
        Node n{id, name};
        nodeMap[name] = id;
        nodes[id] = n;
        ++id;
    }

    void addEdge(const std::string &u, const std::string &v, int w) {
        Edge e {nodeMap[u], nodeMap[v], w};
        adjList[nodeMap[u]].emplace_back(e);
        edges.emplace_back(e);
    }

    const Edges& getEdges() const {
        return edges;
    }

    const Edges& getEdges(const std::string &u) const {
        auto itr = nodeMap.find(u);
        if (itr == nodeMap.end())
            throw std::runtime_error("Invalid node " + u);
        
        return adjList[itr->second];
    }

    const Edges& getEdges(int u) const {
        return adjList[u];
    }

    const Node& getNode(const std::string &u) const {
        auto itr = nodeMap.find(u);
        if (itr == nodeMap.end())
            throw std::runtime_error("Invalid node " + u);

        return nodes[itr->second];
    }

    const Node& getNode(int u) const {
        return nodes[u];
    }

    int getV() const { return V; }

private:
    int V = 0;
    int id = 0;
    Nodes nodes;
    std::unordered_map<std::string, int> nodeMap;

    Edges edges;
    std::vector<Edges> adjList;
};


class BellmanFord {
public:
    BellmanFord(const Graph &graph, const std::string &s): graph(graph), s(s) {}

    bool compute() {
        initialize();

        for (int i = 1; i <= graph.getV()-1; ++i) {
            for (const auto &e: graph.getEdges()) {
                relax(e);
            }
        }

        for (const auto &e: graph.getEdges()) {
            if (distance[e.v] > distance[e.u] + e.w)
                return false;
        }

        return true;
    }

    void print() {
        std::cout << "Single source[" << s << "] shortest paths edges:" << std::endl;
        for (int i = 0; i <= graph.getV()-1; ++i) {
            if (parent[i] != -1) {
                std::cout << graph.getNode(parent[i]).data << "-->" << graph.getNode(i).data << std::endl;
            }
        }

        std::cout << "Single source[" << s << "] shortest paths distance:" << std::endl;
        for (int i = 0; i <= graph.getV()-1; ++i) {
            std::cout << graph.getNode(i).data << ":" << distance[i] << std::endl;
        }
    }

private:
    void initialize() {
        distance.resize(graph.getV(), MAX);
        parent.resize(graph.getV(), -1);

        distance[graph.getNode(s).id] = 0;
    }

    void relax(const Edge& e) {
        if (distance[e.u] == MAX)
            return;

        if (distance[e.v] == MAX || distance[e.v] > distance[e.u] + e.w ) {
            distance[e.v] = distance[e.u] +  e.w;
            parent[e.v] = e.u;
        }
    }

    const std::string &s;
    const Graph &graph;
    std::vector<int> distance;
    std::vector<int> parent;
    int MAX = std::numeric_limits<int>::max();;
};

int main() {
    std::cout << "Single Source Shorted Paths - BellmanFord Algorithm" << std::endl;

    Graph graph(5);
    graph.addNode("s");
    graph.addNode("t");
    graph.addNode("y");
    graph.addNode("x");
    graph.addNode("z");

    graph.addEdge("s", "t", 6);
    graph.addEdge("s", "y", 7);
    graph.addEdge("t", "x", 5);
    graph.addEdge("t", "y", 8);
    graph.addEdge("t", "z", -4);
    graph.addEdge("x", "t", -2);
    graph.addEdge("y", "x", -3);
    graph.addEdge("y", "z", 9);
    graph.addEdge("z", "x", 7);
    graph.addEdge("z", "s", 2);

    BellmanFord bellmanFord(graph, "s");
    std::cout << "Shortest path exists from source: " << bellmanFord.compute() << std::endl;
    bellmanFord.print();
    return 0;
}