#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <unordered_map>

class Graph {
public:
    struct Node {
        int id = 0;
        std::string name;
    };
    using Nodes = std::vector<Node>;

    struct Edge {
        int u  = 0;
        int v = 0;
        int w = 0;
    };
    using Edges = std::vector<Edge>;

    Graph(int V) : V(V) {
        nodes.resize(V, Node());
        adjList.resize(V, Edges());
    }

    void addNode(const std::string &u) {
        Node n {id, u};
        nodes[id] = n;
        nodeMap[u] = id;

        ++id;
    }

    void addEdge(const std::string& u, const std::string &v, int w) {
        Edge e{nodeMap[u], nodeMap[v], w};
        edges.emplace_back(e);
        adjList[nodeMap[u]].emplace_back(e);
    }

    const Edges& getEdges(const std::string &u)  {
        return adjList[nodeMap[u]];
    }

    const Edges& getEdges(int u)  {
        return adjList[u];
    }

    const Edges& getEdges() const {
        return edges;
    }

    int getV() const { return V; }

    const Node& getNode(const std::string& u) const {
        auto itr = nodeMap.find(u);
        if (itr == nodeMap.end()) {
            throw std::runtime_error("Invalid node");
        }
        return nodes[itr->second];
    }

    const Node& getNode(int u) const {
        return nodes[u];
    }

    const Nodes& getNodes() const {
        return nodes;
    }

private:
    int V = 0;
    int id = 0;
    Nodes nodes;
    Edges edges;

    std::vector<Edges> adjList;
    std::unordered_map<std::string, int> nodeMap;
};

class MinHeap {
    struct Node {
        using Ptr = std::shared_ptr<Node>;

        int id  = 0;
        int key = 0;
    };
public:  
    MinHeap () {
    }

    void push(int item, int key) {
        Node::Ptr ptr (new Node{item, key});
        items.emplace_back(ptr);
        map[item] = ptr;
        heapProperty = false;
    }

    bool empty() const {
        return items.empty();
    }

    void update(int item, int key) {
        map[item]->key = key;
        map[item]->id = item;
        heapProperty = false;
    }

    Node::Ptr extract() {
        Node::Ptr ptr;
        if (!empty()) {
            
            makeHeap();

            ptr = items.front();
            items.erase(items.begin());
        }
        return ptr;
    }

private:
    void makeHeap() {
        if (empty())
            return;

        if (!heapProperty) {
            auto cmp = [](auto a, auto b) {
                return a->key > b->key;
            };

            std::make_heap (std::begin(items), std::end(items), cmp);
            heapProperty = true;
        }
    }

    std::vector<Node::Ptr> items;
    std::unordered_map<int, Node::Ptr> map;
    bool heapProperty = false;
};

class DijkstraAlgorithm {
public:
    DijkstraAlgorithm(const Graph &graph, const std::string &source) : graph(graph), source(source) {
        parent.resize(graph.getV(), -1);
        distance.resize(graph.getV(), INF);
    }

    void operator() () {
        initialize();

        while(!minHeap.empty()) {
            int u = minHeap.extract()->id;
            for (const auto & e : graph.getEdges() ) {
                relax(e);
            }
        }
        
    }

    void print() {
        std::cout << "Single source[" << source << "] shortest paths edges:" << std::endl;
        for (int i = 0; i <= graph.getV()-1; ++i) {
            if (parent[i] != -1) {
                std::cout << graph.getNode(parent[i]).name << "-->" << graph.getNode(i).name << std::endl;
            }
        }

        std::cout << "Single source[" << source << "] shortest paths distance:" << std::endl;
        for (int i = 0; i <= graph.getV()-1; ++i) {
            std::cout << graph.getNode(i).name << ":" << distance[i] << std::endl;
        }
    }

private:
    void initialize() {
        for (const auto & node: graph.getNodes()) {
            minHeap.push(node.id, INF);
        }

        Graph::Node s = graph.getNode(source);
        minHeap.update(s.id, 0);
        distance[s.id] = 0;
    }

    void relax(const Graph::Edge &e) {
        if (distance[e.v] == INF || distance[e.v] > distance[e.u] + e.w) {
            int w = distance[e.u] +  e.w;
            distance[e.v] = w;
            minHeap.update(e.v, w);
            parent[e.v] = e.u;
        }
    }

    const Graph &graph;
    const std::string &source;
    std::vector<int> distance;
    std::vector<int> parent;
    int INF = std::numeric_limits<int>::max();
    MinHeap minHeap;

};

int main() {
    std::cout << "Single source shortest paths - Dijkstra algorithm" << std::endl;

    Graph graph(5);
    graph.addNode("s");
    graph.addNode("t");
    graph.addNode("y");
    graph.addNode("x");
    graph.addNode("z");

    graph.addEdge("s", "t", 10);
    graph.addEdge("s", "y", 5);
    graph.addEdge("t", "x", 1);
    graph.addEdge("t", "y", 2);
    graph.addEdge("x", "z", 4);
    graph.addEdge("y", "t", 3);
    graph.addEdge("y", "x", 9);
    graph.addEdge("y", "z", 2);
    graph.addEdge("z", "x", 6);
    graph.addEdge("z", "s", 7);

    DijkstraAlgorithm shortestPath(graph, "s");
    shortestPath();
    shortestPath.print();
    return 0;
}