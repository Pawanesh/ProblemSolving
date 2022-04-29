#include <iostream>
#include <vector>
#include <limits>
#include <queue>

struct Edge {
    int u = 0;
    int v = 0;
    int c = 0;
    int f = 0;

    int effCap () {
        return c-f;
    }
};
using Edges = std::vector<Edge>;

class Graph {
public:
    Graph(int V) : V(V) {
        adj.resize(V, Edges());
    }

    void addEdge(int u, int v, int c , int f) {
        Edge e{u,v,c,f};
        adj[u].emplace_back(e);
    }

    Edges& edges(int u) {
        return adj[u];
    }

    int getEffectiveCapacity(int u, int v) {
        for (auto &e: edges(u)) {
            if (e.v == v) {
                return e.effCap();
            }
        }

        return 0;
    }

    void updateflow(int u, int v, int f) {
        for (auto &e: edges(u)) {
            if (e.v == v) {
                e.f += f;
                return;
            }
        }

        addEdge(u,v,0,f);
    }

    std::vector<int> bfs(int s , int t) {
        std::vector<int> path(V, -1);
        std::vector<bool> visited(V, false);
        std::queue<int> queue;
        queue.push(s);
        visited[s] = true;

        while(!queue.empty()) {
            int u = queue.front();

            queue.pop();

            for (auto &e : edges(u)) {
                int v = e.v;
                if(!visited[v] && e.effCap() > 0) {
                    visited[v] = true;
                    queue.push(v);
                    path[v] = u;
                    if (v == t) {
                        return path;
                    }
                }
            }
        }

        return std::vector<int>();
    }

    

private:
    int V = 0;
    std::vector<Edges> adj;
};

class FlowNetwork {
public:    
    FlowNetwork(int V, int s, int t) : s(s), t(t), graph(V){
    }

    void addEdge(int u, int v, int c = 0, int f = 0){
        
        graph.addEdge(u,v,c,f);
    }

    Graph maxFlowResidualGraph() {
        Graph rGraph = graph;
        int maxFlow = 0;

        std::vector<int> path = rGraph.bfs(s,t);

        while (!path.empty()) {
            int bottleNeck = getBottleNeck(rGraph, path);
            maxFlow += bottleNeck;
            updateflow(rGraph, path, bottleNeck);
            
            path = rGraph.bfs(s,t);

        }

        std::cout << "Maxflow: " << maxFlow << std::endl;
        return rGraph;  

    }

private:
    int getBottleNeck(Graph& rGraph, std::vector<int> &path) {
        int bottleNeck = std::numeric_limits<int>::max();

        for (int v = t; v != s; v = path[v]) {
            int u = path[v];
            int effCap = rGraph.getEffectiveCapacity(u,v);
            if (effCap < bottleNeck) {
                bottleNeck = effCap;
            }

        }

        return bottleNeck;
    }

    void updateflow(Graph& rGraph, std::vector<int>& path, int bottleNeck) {
        for (int v = t; v != s; v = path[v]) {
            int u = path[v];
            rGraph.updateflow(u,v,bottleNeck);
            rGraph.updateflow(v,u,-bottleNeck);
        }


    }

    int s = 0;
    int t = 0;
    Graph graph;

};

class MaximumChannelAssignments {
public:
    MaximumChannelAssignments(int txSize, int rxSize) : txSize(txSize), rxSize(rxSize) {
        inputs.resize(txSize, std::vector<int>());
    }

    void setInputs(std::vector<std::vector<int>> inputs){
        this->inputs = inputs;
    }

    void add(int txID, int rxID) {
        inputs[txID].emplace_back(rxID);
    }

    std::vector<std::pair<int,int>> getMatching() {
        FlowNetwork flowNetwork = createFlowNetwork();
        Graph rGraph = flowNetwork.maxFlowResidualGraph();

        std::vector<std::pair<int,int>> matching;
        for (int i  = 0; i <= txSize-1; ++i) {
            for (auto &e: rGraph.edges(i)) {
                if (e.f > 0) {
                    int v = e.v;
                    int j = v - txSize;
                    matching.emplace_back(std::make_pair(i,j));
                }
            }
        }

        return matching;
    }


private:
    FlowNetwork createFlowNetwork() {
        int V = txSize + rxSize + 2;
        int s = txSize + rxSize;
        int t = s + 1;

        FlowNetwork flowNetwork(V, s, t);

        for (int i = 0; i <= inputs.size()-1; ++i) {
            std::vector<int> rxs = inputs[i];
            
            if (rxs.empty()) continue;

            for (int j = 0; j <= rxs.size()-1; ++j) {
                int c = rxs[j];
                if (c > 0) {
                    int rxID = j + txSize;
                    flowNetwork.addEdge(i,rxID,c);
                }
            }
        }

        for (int i = 0; i <= txSize-1; ++i) {
            flowNetwork.addEdge(s,i,1);
        }

        for (int j = 0; j <= rxSize-1; ++j) {
            int rxID = j + txSize;
            flowNetwork.addEdge(rxID,t,1);
        }

        return flowNetwork;
    }

    int txSize = 0;
    int rxSize = 0;
    std::vector<std::vector<int>> inputs;

};


int main() {
    std::cout << "Maximize channel assignments" << std::endl;
    MaximumChannelAssignments maximumChannelAssignments(3,3);
    maximumChannelAssignments.setInputs({{0, 2, 0}, {3, 0, 1}, {2, 4, 0}});
    std::vector<std::pair<int,int>> matching = maximumChannelAssignments.getMatching();

    for (auto &match : matching) {
        std::cout << "T" << match.first+1 << ": R" << match.second+1 << std::endl;
    }


    /*
    The output should be:

    The number of maximum packets sent in the time slot is 3
    T1 -> R2
    T2 -> R3
    T3 -> R1 

    */

    return 0;
}