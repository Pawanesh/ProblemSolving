#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

//Graph
struct Node {
    int id  = 0;
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

    void addNode(const std::string& name) {
        Node n{id, name};
        nodes[id] = n;
        nodeMap[name] = n;
        ++id;
    }

    void addEdge(const std::string &u, const std::string &v, int w) {
        addEdge(nodeMap[u].id, nodeMap[v].id, w);
    }

    void addEdge(int u, int v, int w) {
        Edge forward{u, v, w};
        edges.emplace_back(forward);
        adjList[u].emplace_back(forward);

        Edge backword{v, u, w};
        edges.emplace_back(backword);
        adjList[v].emplace_back(backword);
    }

    Edges& getEdges () {
        return edges;
    }

    Edges& getEdges(const std::string &u) {
        return getEdges(nodeMap[u].id);
    }

    Edges& getEdges(int u) {
        return adjList[u];
    }

    Node getNode(const std::string& u) {
        return getNode(nodeMap[u].id);
    }

    Node getNode(int u) {
        return nodes[u];
    }

    Nodes& getNodes() {
        return nodes;
    }

    int getV() {
        return V;
    }

private:
    int V = 0;
    int id = 0;
    
    Nodes nodes;
    std::unordered_map<std::string, Node> nodeMap;

    Edges edges;
    std::vector<Edges> adjList;

};

//DisjointSet

struct Set {
    using SharedPtr = std::shared_ptr<Set>;
    using WeakPtr = std::weak_ptr<Set>;

    int id = 0;
    int rank = 0;
    WeakPtr parentPtr;
};

class DisjointSet {
public:    
    DisjointSet(Graph &graph) : graph(graph) {
        sets.resize(graph.getV(), Set::SharedPtr());
    }

    void makeSet(int u) {
        Set::SharedPtr setPtr = std::make_shared<Set>();
        setPtr->id = u;
        setPtr->parentPtr = setPtr;
        sets[u] = setPtr;
    }

    Set::SharedPtr findSet(int u) {
        return findSet(sets[u]);
    }

    Set::SharedPtr findSet (Set::SharedPtr setPtr) {
        if (!setPtr) return setPtr;

        Set::SharedPtr parentPtr = setPtr->parentPtr.lock();
        if (!parentPtr) return parentPtr;

        if (setPtr->id != parentPtr->id) {
            setPtr->parentPtr = findSet(setPtr->parentPtr.lock());
        }

        return setPtr->parentPtr.lock();
    }

    void unionSets(int u, int v) {
        Set::SharedPtr ptru = findSet(u);
        if (!ptru) {
            return;
        }

        Set::SharedPtr ptrv = findSet(v);
        if(!ptrv) {
            return;
        }

        if (ptru->id == ptrv->id) return;

        if (ptru->rank > ptrv->rank) {
            ptrv->parentPtr = ptru;
        }
        else if (ptrv->rank > ptru->rank) {
            ptru->parentPtr = ptrv;
        }
        else {
            ptru->parentPtr = ptrv;
            ptrv->rank++;
        }
    }

private:
    Graph &graph;
    std::vector<Set::SharedPtr> sets;
};

//MST_KRUSKAL
class MST_KRUSKAL {
public:    
    MST_KRUSKAL(Graph &graph) : graph(graph), disjointSet(graph) {

    }

    void compute(){
        for (auto &node : graph.getNodes()) {
            disjointSet.makeSet(node.id);
        }

        Edges edges = graph.getEdges();
        std::sort(std::begin(edges), std::end(edges), [](auto &a, auto &b) { return a.w < b.w; });

        for (auto &e: edges) {
            int u = e.u;
            int v = e.v;
  
            Set::SharedPtr ptru = disjointSet.findSet(u);
            Set::SharedPtr ptrv = disjointSet.findSet(v);

            if (ptru->id != ptrv->id) {
                mst.emplace_back(e);
                disjointSet.unionSets(u,v);
            }
        }
    }

    void print(){
        std::cout << "MST Edges" << std::endl;
        int mstWeight = 0;
        for (auto &e: mst) {
            std::cout << "u: " << graph.getNode(e.u).data << ", v: " << graph.getNode(e.v).data << ", w: " << e.w << std::endl;
            mstWeight += e.w;
        }

        std::cout << "MST Weight: " << mstWeight << std::endl;
    }

private:
    Graph &graph;
    DisjointSet  disjointSet;
    Edges mst;

};
int main() {
    std::cout << "MST-KRUSKAL" << std::endl;
    Graph graph(9);
    graph.addNode("a");
    graph.addNode("b");
    graph.addNode("c");
    graph.addNode("d");
    graph.addNode("e");
    graph.addNode("f");
    graph.addNode("g");
    graph.addNode("h");
    graph.addNode("i");

    graph.addEdge("a", "b", 4);
    graph.addEdge("a", "h", 8);
    graph.addEdge("b", "c", 8);
    graph.addEdge("b", "h", 11);
    graph.addEdge("c", "d", 7);
    graph.addEdge("c", "i", 2);
    graph.addEdge("c", "f", 4);
    graph.addEdge("d", "e", 9);
    graph.addEdge("d", "f", 14);
    graph.addEdge("e", "f", 10);
    graph.addEdge("f", "g", 2);
    graph.addEdge("g", "h", 1);
    graph.addEdge("g", "i", 6);
    graph.addEdge("h", "i", 7);

    MST_KRUSKAL mst(graph);
    mst.compute();

    mst.print();

    return 0;
}