#include <iostream>
#include <vector>
#include <queue>
#include <limits>

struct Edge {
    int u = 0;
    int v = 0;
    int c = 0;
    int f = 0;
    friend std::ostream& operator << (std::ostream &os, Edge& e);
};

std::ostream& operator << (std::ostream &os, Edge& e) {
    os << "u: " << e.u << ", v: " << e.v << ", c: " << e.c << ", f: " << e.f;
    return os;
}

using Edges = std::vector<Edge>;

class Graph {
public:
    Graph(int V): V(V) {
        adj.resize(V, Edges());
    }

    void addEdge(int u, int v, int c, int f) {
        Edge e{u,v,c,f};
        adj[u].emplace_back(e);
    }

    Edges& edges(int u) {
        return adj[u];
    }

    int effectiveCapacity(int u, int v) {
        for (auto &e: edges(u)) {
            if (v == e.v) {
                return e.c - e.f;
            }
        }
        return 0;
    }

    void updateFlow(int u, int v, int f) {
        for (auto &e: edges(u)) {
            if (v == e.v) {
                e.f += f;
            }
        }
    }

    std::vector<int> bfs(int s, int t) {
        std::vector<int> path(V, -1);
        std::vector<bool> visisted(V, false);
        std::queue<int> queue;

        queue.push(s);
        visisted[s] = true;

        while(!queue.empty()) {
            int u = queue.front();
            queue.pop();

            for (auto & e: edges(u) ) {
                int v = e.v;
                if (!visisted[v]) {
                    int effCap = effectiveCapacity(u, v);
                    if (effCap > 0) {
                        path[v] = u;
                        visisted[v] = true;
                        queue.push(v);

                        if (v == t) {
                            return path;
                        }
                    }
                }
            }
        }

        return std::vector<int>();
    }

    void print(){
        for (auto & edgesList : adj) {
            for (auto & edge : edgesList ) {
                std::cout << edge << std::endl;
            }
        }
    }

private:
    int V = 0;
    std::vector<Edges> adj;
};

class FlowNetwork{
public:
    FlowNetwork(int V, int s, int t): s(s), t(t), graph(V) {

    }

    void addEdge(int u, int v, int c = 0, int f = 0) {
        graph.addEdge(u,v,c,f);
    }

    std::pair<int, Graph> maximumFLow() {
        Graph rGraph = graph;
        int maxFlow = 0;
        auto path = rGraph.bfs(s, t);
        while(!path.empty()) {
            int bottleNeck = getBottleNeck(rGraph, path);

            update(rGraph, path, bottleNeck);

            maxFlow += bottleNeck;

            path = rGraph.bfs(s, t);
        }

        return std::make_pair(maxFlow, rGraph);
    }


private:
    int getBottleNeck(Graph &rGraph, std::vector<int>& path) {
        int bottleNeck = std::numeric_limits<int>::max();
        for (int v = t; v != s; v = path[v]) {
            int u = path[v];
            int val = rGraph.effectiveCapacity(u,v);
            if (val < bottleNeck) {
                bottleNeck = val;
            }
        }
        return bottleNeck;
    }

    void update(Graph &rGraph, std::vector<int>& path, int bottleNeck) {
        for (int v = t; v != s; v = path[v]) {
            int u = path[v];
            rGraph.updateFlow(u,v,bottleNeck);
            rGraph.addEdge(v,u,0,-bottleNeck);
        }
    }

    int s = 0;
    int t = 0;
    Graph graph;
};

int main() {

    FlowNetwork flowNetwork(8, 0, 7);
    flowNetwork.addEdge(0,1,1);
    flowNetwork.addEdge(0,2,1);
    flowNetwork.addEdge(0,3,1);
    flowNetwork.addEdge(1,2,1);
    flowNetwork.addEdge(2,3,1);
    flowNetwork.addEdge(2,6,1);
    flowNetwork.addEdge(3,6,1);
    flowNetwork.addEdge(4,2,1);
    flowNetwork.addEdge(4,7,1);
    flowNetwork.addEdge(5,4,1);
    flowNetwork.addEdge(5,1,1);
    flowNetwork.addEdge(5,7,1);
    flowNetwork.addEdge(6,5,1);
    flowNetwork.addEdge(6,7,1);

    std::pair<int, Graph> flow = flowNetwork.maximumFLow();
    std::cout << "Maximum Edge Disjoint paths: " <<  flow.first << std::endl;
    std::cout << "Residual Graph" << std::endl;
    flow.second.print();



}