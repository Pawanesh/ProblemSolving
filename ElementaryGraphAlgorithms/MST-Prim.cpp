#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <limits>
#include <unordered_set>

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
        Node n {id, name};
        nodes[id] = n;
        nodeMap[name] = n;
        ++id;
    }

    void addEdge(const std::string &u, const std::string &v, int w) {
        Edge forward {nodeMap[u].id, nodeMap[v].id, w};
        adjList[nodeMap[u].id].emplace_back(forward);

        Edge backward {nodeMap[v].id, nodeMap[u].id, w};
        adjList[nodeMap[v].id].emplace_back(backward);
    }

    Edges& getEdges(const std::string &u) {
        return getEdges(nodeMap[u].id);
    }

    Edges& getEdges(int u) {
        return adjList[u];
    }

    Node& getNode(const std::string &u) {
        return nodeMap[u];
    }

    Node& getNode(int u) {
        return nodes[u];
    }

    Nodes& getNodes() { return nodes; }
     

    int getV() { return V; }

private:
    int V = 0;
    int id  = 0;
    Nodes nodes;
    std::unordered_map<std::string, Node> nodeMap;
    Edges edges;
    std::vector<Edges> adjList;

};


class MinHeap{
public:
    struct Data {
        using Ptr = std::shared_ptr<Data>;
        
        Data(int id, int key) : id(id), key(key) {}

        int id = 0;
        int key = 0;


    };

    MinHeap() {
    }

    void insert(int id, int key) {
        Data::Ptr dataPtr = std::make_shared<Data>(id, key);
        dataPtrVector.emplace_back(dataPtr);
        dataPtrMap[id] = dataPtr;
    }

    Data::Ptr extract() {
        auto cmp = [](auto a, auto b) {
            return a->key > b->key;
        };

        std::make_heap (std::begin(dataPtrVector), std::end(dataPtrVector), cmp);

        Data::Ptr min = dataPtrVector.front();
        dataPtrVector.erase(dataPtrVector.begin());
        return min;
    }

    void key(int id, int key) {
        dataPtrMap[id]->key = key;
    }

    int key(int id) {
        return dataPtrMap[id]->key;
    }

    bool empty() {
        return dataPtrVector.empty();
    }

private:
    std::vector<Data::Ptr> dataPtrVector;
    std::unordered_map<int, Data::Ptr> dataPtrMap;
};

class MST_Prim {
public:
    MST_Prim(Graph &graph) : graph(graph) {
        mst.resize(graph.getV(), -1);
    }

    void compute() {
        int max = std::numeric_limits<int>::max();
        for (auto &node : graph.getNodes() ) {
            minHeap.insert(node.id, max);
        }

        int s = graph.getNodes()[0].id;
        minHeap.key(s, 0);

        std::vector<bool> visited(graph.getV(), false); 
        visited[s] = true;

        while(!minHeap.empty()) {
            MinHeap::Data::Ptr dataPtr = minHeap.extract();
            int u = dataPtr->id;
            visited[u] = true;

            for (auto & edge: graph.getEdges(u)) {
                int v = edge.v;
                int w = edge.w;
                                
                if (!visited[v]) {
                    int vKey = minHeap.key(v);
                    if (w < vKey) {
                        mst[v] = u;
                        minHeap.key(v, w);
                    }
                }
            }
        }
    }

    void print () {
        std::cout << "MST Edges:" << std::endl;
        int mstWeight = 0;
        
        for (int i = 0; i <= mst.size()-1; ++i) {
            if (mst[i] != -1) {
                int w = 0;
                for (auto &edge: graph.getEdges(i)) {
                    if (edge.v == mst[i]) {
                        w = edge.w;
                        break;
                    }
                }
                std::cout << graph.getNode(i).data << "--" << graph.getNode(mst[i]).data << ": " << w << std::endl;
                mstWeight += w;
            }
        }
        std::cout << "MST Weight: " << mstWeight << std::endl;
    }
private:
    Graph &graph;
    MinHeap minHeap;
    std::vector<int> mst;
    Edges mstEdges;
};

int main() {
    std::cout << "MST Prim's Algorithm" << std::endl;

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

    MST_Prim mst(graph);
    mst.compute();
    mst.print();

    return 0;
}