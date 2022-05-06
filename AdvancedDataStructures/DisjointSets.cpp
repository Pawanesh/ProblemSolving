#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

struct Set {
    using Ptr = std::shared_ptr<Set>;
    int id = 0;
    int rank = 0;
    Ptr parentPtr = nullptr;
};


class DisjointSet{
public:
    DisjointSet(int V) {
        sets.resize(V, Set::Ptr());
    }

    void makeSet(int u) {
        Set::Ptr ptr(new Set());
        ptr->id = u;
        ptr->parentPtr = ptr;
        sets[u] = ptr;
    }


    Set::Ptr findSet(int u) {
        return findSet(sets[u]);
    }

    Set::Ptr findSet(Set::Ptr ptr) {
        if (ptr->id != ptr->parentPtr->id) {
            ptr->parentPtr = findSet(ptr->parentPtr);
        }

        return ptr->parentPtr;
    }

    void unionSets(int u, int v) {
        Set::Ptr ptrU = findSet(u);
        Set::Ptr ptrV = findSet(v);
        
        if (ptrU->id == ptrV->id) 
            return;

        if (ptrU->rank > ptrV->rank) {
            ptrV->parentPtr = ptrU;
        }
        else if (ptrU->rank < ptrV->rank) {
            ptrU->parentPtr = ptrV;
        }
        else {
            ptrU->parentPtr = ptrV;
            ptrV->rank++;
        }
    }

private:
    std::vector<Set::Ptr> sets;
};


//Graph
struct Node {
    int id = 0;
    std::string data;
};

struct Edge {
    int u = 0;
    int v = 0;
};

class Graph {
public:
    Graph(int V) : V(V) {
        nodes.resize(V, Node());
        adjList.resize(V, std::vector<Edge>());
    }

    void addNode (const std::string &u) {
        Node n {id, u};
        nodes[id] = n;
        nodeMap[u] = n;
        ++id;
    }

    void addEdge(const std::string &u, const std::string &v) {
        addEdge(nodeMap[u], nodeMap[v]);
    }

    void addEdge(const Node &u, const Node &v) {
        Edge forward{u.id, v.id};
        adjList[u.id].emplace_back(forward);
        edges.emplace_back(forward);

        Edge backward{v.id, u.id};
        adjList[v.id].emplace_back(backward);
        edges.emplace_back(backward);
    }
    
    std::vector<Edge>& getEdges(const std::string &u) {
        return getEdges(nodeMap[u]);
    }

    std::vector<Edge>& getEdges(const Node &u) {
        return adjList[u.id];
    }

    std::vector<Edge>& getEdges() {
        return edges;
    }

    std::vector<Node>& getNodes() {
        return nodes;
    }

    Node getNode(const std::string &u) {
        return nodeMap[u];
    }

    Node getNode(int u) {
        return nodes[u];
    }

    int getV() {
        return V;
    }


private:
    int V = 0;
    int id = 0;
    std::vector<Node> nodes;
    std::unordered_map<std::string, Node> nodeMap;
    std::vector<std::vector<Edge>> adjList;
    std::vector<Edge> edges;
};

class ConnectedComponents {
public:
    ConnectedComponents(Graph& graph) : graph(graph), disjointSet(graph.getV()) {
        for (auto &node: graph.getNodes()) {
            disjointSet.makeSet(node.id);
        }

        for (auto &e: graph.getEdges()) {
            disjointSet.unionSets(e.u, e.v);
        }
    }

    bool sameComponent(const std::string &a, const std::string &b) {
        Node nodeA = graph.getNode(a);
        Node nodeB = graph.getNode(b);

        return disjointSet.findSet(nodeA.id)->id == disjointSet.findSet(nodeB.id)->id;
    }

private:
    Graph &graph;
    DisjointSet disjointSet;
    
};

int main() {
    std::cout << "DisjointSets" << std::endl;

    Graph graph(10);
    graph.addNode("a");
    graph.addNode("b");
    graph.addNode("c");
    graph.addNode("d");
    graph.addNode("e");
    graph.addNode("f");
    graph.addNode("g");
    graph.addNode("h");
    graph.addNode("i");
    graph.addNode("j");

    graph.addEdge("a", "b");
    graph.addEdge("a", "c");
    graph.addEdge("b", "c");
    graph.addEdge("b", "d");
    graph.addEdge("e", "f");
    graph.addEdge("g", "e");
    graph.addEdge("h", "i");

    ConnectedComponents cc(graph);
    std::cout << std::boolalpha << "a, b isSameComponent: " << cc.sameComponent("a", "b") << std::endl;
    std::cout << std::boolalpha << "h, i isSameComponent: " << cc.sameComponent("h", "i") << std::endl;
    std::cout << std::boolalpha << "a, i isSameComponent: " << cc.sameComponent("a", "i") << std::endl;
    std::cout << std::boolalpha << "i, j isSameComponent: " << cc.sameComponent("j", "i") << std::endl;

    return 0;
}