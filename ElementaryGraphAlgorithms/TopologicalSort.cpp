#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

struct Node {
    int id;
    std::string name;
};
using Nodes = std::vector<Node>;


template <typename Graph>
struct DFS {
    enum class COLOR {
        WHITE = 0,
        GRAY = 1,
        BLACK = 2
    };

    DFS(Graph& graph) : graph(graph) {
        V = graph.getV();
        color.resize(V, COLOR::WHITE);
        startTime.resize(V, 0);
        finishTime.resize(V, 0);
        tree.resize(V, -1);
    }

    void operator() () {
        for (int i = 0; i <= V-1; ++i) {
            if (color[i] == COLOR::WHITE)
                dfs(i);
        }
    }

    void dfs(int i) {
        stack.push(i);
        
        while(!stack.empty()) {
            int u = stack.top();
            if (color[u] == COLOR::WHITE) {
                color[u] = COLOR::GRAY;
                ++time;
                startTime[u] = time;
                for (auto &v: graph.edges(u)) {
                    if (color[v] == COLOR::WHITE) {
                        color[v] = COLOR::GRAY;
                        tree[v] = u;
                        stack.push(v);
                    }
                }
            }
            else {
                stack.pop();
                if (color[u] == COLOR::GRAY) {
                    color[u] = COLOR::BLACK;
                    ++time;
                    finishTime[u] = time;
                }
            }
        }
    }

    int V = 0;
    int time = 0;
    std::vector<COLOR> color;
    std::vector<int> startTime;
    std::vector<int> finishTime;
    std::vector<int> tree;
    std::stack<int> stack;
    Graph &graph;
};

class Graph {
public:
    Graph(int V): V(V) {
        adjList.resize(V, std::vector<int>());
    }
    
    int getV() {
        return V;
    }

    std::vector<int>& edges(int u) {
        return adjList[u];
    }

    void addNode(const std::string &name) {
        Node n{id, name};
        nodes.emplace_back(n);
        nameIDMap[name] = id;
        ++id;
    }

    void addEdge(const std::string &u, const std::string &v) {
        adjList[nameIDMap[u]].emplace_back(nameIDMap[v]);
    }

    Nodes topologicalOrder() {
        DFS<Graph> dfsGraph(*this);
        dfsGraph();

        Nodes orderedNodes = nodes;
        auto &finishTime = dfsGraph.finishTime;
        
        auto cmp = [&] (auto &n1, auto &n2) {
            int f1 = finishTime[n1.id];
            int f2 = finishTime[n2.id];

            return f1 > f2;
        };

        std::sort(std::begin(orderedNodes), std::end(orderedNodes), cmp);
        return orderedNodes;
    }

private:


    int id  = 0;
    int V = 0;
    Nodes nodes;

    std::unordered_map<std::string, int> nameIDMap;
    std::vector<std::vector<int>> adjList;

};

int main() {
    std::cout << "Topological odering" << std::endl;
    Graph graph(9);
    
    graph.addNode("undershorts");
    graph.addNode("pants");
    graph.addNode("belt");
    graph.addNode("shirt");
    graph.addNode("tie");
    graph.addNode("jacket");
    graph.addNode("socks");
    graph.addNode("shoes");
    graph.addNode("watch");


    graph.addEdge("undershorts", "pants");
    graph.addEdge("undershorts", "shoes");
    graph.addEdge("pants", "shoes");
    graph.addEdge("pants", "belt");
    graph.addEdge("belt", "jacket");
    graph.addEdge("tie", "jacket");
    graph.addEdge("shirt", "tie");
    graph.addEdge("shirt", "belt");
    graph.addEdge("socks", "shoes");

    Nodes nodes = graph.topologicalOrder();

    std::cout << "Topological oreder: " << std::endl;

    for (auto &n : nodes) {
        std::cout << n.name << " ";
    }

    std::cout << std::endl;

    return 0;
}