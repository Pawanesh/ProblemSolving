#include <iostream>
#include <vector>
#include <limits>

struct Vertex {
    int height = 0;
    int excessFlow = 0;
};

struct Edge {
    int u = 0;
    int v = 0;
    int capacity = 0;
    int flow = 0;
};

class Graph {
public:
    Graph(int V, int s, int t) :V(V), s(s), t(t){
        capacity.resize(V, std::vector<int>(V,0));
        flow.resize(V, std::vector<int>(V,0));
        height.resize(V,0);
        excessFlow.resize(V,0);

    }

    void addEdge(int u, int v, int c) {
        capacity[u][v] = c;
    }

    int getMaxFlow() {
        preFlow();

        int u = getOverFlowVertex();
        while(u != -1) {
            if(!push(u)) {
                relable(u);
            }
            u = getOverFlowVertex();
        }

        return excessFlow[t];
    }

    int getOverFlowVertex() {
        for (int u = 1; u <= V-2; ++u) {
            if (excessFlow[u] > 0 ) {
                return u;
            }
        }

        return -1;
    }

    void preFlow() {
        rGraph = capacity;
        
        height[s] = V;
        for (int v = 0; v <= V-1; ++v) {
            if (capacity[s][v]) {
                flow[s][v] = capacity[s][v];
                excessFlow[v] = capacity[s][v];
                excessFlow[s] -= capacity[s][v];

                rGraph[s][v] -= flow[s][v];
                rGraph[v][s] += flow[s][v];
            }
        }

    }

    bool push(int u) {
        for (int v = 0; v <= V-1; ++v) {
            if (rGraph[u][v]) {
                if (height[u] > height[v]) {
                    int f = std::min(rGraph[u][v], excessFlow[u]);
                    excessFlow[u] -= f;
                    excessFlow[v] += f;

                    flow[u][v] += f;
                    flow[v][u] -= f;

                    rGraph[u][v] -= f;
                    rGraph[v][u] += f;

                    return true;
                }
            }
        }
        
        return false;
    }


    void relable(int u) {
        int minHeight = std::numeric_limits<int>::max();
        for (int v = 0; v <= V-1; ++v) {
            if (rGraph[u][v] && flow[u][v] < capacity[u][v] ) {
                if (height[v] < minHeight) {
                    minHeight = height[v];
                    height[u] = minHeight + 1;
                }
            }
        }
    }


private:
    int V = 0;
    int s = 0;
    int t = 0;

    std::vector<std::vector<int>> capacity;
    std::vector<std::vector<int>> flow;
    std::vector<std::vector<int>> rGraph;
    std::vector<int> height;
    std::vector<int> excessFlow;
};


int main() {
    std::cout << "PushRelableMaxFLowAlgorithm" << std::endl;

    Graph g(6, 0, 5);
    g.addEdge(0,1,16);
    g.addEdge(0,2,13);
    g.addEdge(1,2,10);
    g.addEdge(1,3,12);
    g.addEdge(2,1,4);
    g.addEdge(2,4,14);
    g.addEdge(3,2,9);
    g.addEdge(3,5,20);
    g.addEdge(4,3,7);
    g.addEdge(4,5,4);

    std::cout << "Max flow: " << g.getMaxFlow() << std::endl;
    return 0;
}