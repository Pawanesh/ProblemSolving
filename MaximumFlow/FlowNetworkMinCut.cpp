#include <iostream>
#include <vector>
#include <stack>
#include <queue>

class FlowNetwork {
public:
    FlowNetwork (int V, int s, int t) : V(V), s(s), t(t) {
        graph.resize(V, std::vector<int> (V, 0));
    }

    void addEdge(int u, int v, int w) {
        graph[u][v] = w;
    }

    std::vector<std::pair<int, int>> computeMinCut() {
        std::vector<std::vector<int>> rGraph = graph;

        std::vector<int> parentPath = getBFSPath(rGraph, s, t);
        int maxFlow = 0;
        while (!parentPath.empty()) {
            int pathMinFlow = std::numeric_limits<int>::max();
            for (int v = t; v != s; v = parentPath[v]) {
                int u = parentPath[v];
                pathMinFlow = std::min(pathMinFlow, rGraph[u][v]);
            }

            for (int v = t; v != s; v = parentPath[v]) {
                int u = parentPath[v];
                rGraph[u][v] -= pathMinFlow;
                rGraph[v][u] += pathMinFlow;
            }

            maxFlow += pathMinFlow;
            parentPath = getBFSPath(rGraph, s, t);
        }

        std::vector<bool> visited = dfs(rGraph, s);

        std::vector<std::pair<int,int>> minCut;
        for (int u = 0; u <= V-1; ++u) {
            for (int v = 0; v <= V-1; ++v) {
                if (visited[u] && !visited[v] && graph[u][v]) {
                    minCut.emplace_back(u,v);
                }
            }
        }

        return minCut;

    }

private:
    std::vector<int> getBFSPath(const std::vector<std::vector<int>> &rGraph, int s, int t) {
        std::vector<bool> visisted(V, 0);
        std::vector<int> parent(V, -1);
        std::queue<int> queue;
        queue.push(s);
        visisted[s] = true;
        bool found = false;
        while(!queue.empty()) {
            int u = queue.front();
            queue.pop();

            for (int v = 0; v <= V-1; ++v) {
                if (rGraph[u][v] && !visisted[v]) {
                    parent[v] = u;
                    visisted[v] = true;
                    queue.push(v);
                    if (v == t) {
                        found = true;
                        break;
                    }
                }
            }
        }

        if (found) return parent;

        return std::vector<int>();
    }

    std::vector<bool> dfs(const std::vector<std::vector<int>> &rGraph, int s) {
        std::vector<bool> visisted(V , false);

        std::stack<int> stack;
        stack.push(s);
        visisted[s] = true;

        while(!stack.empty()) {
            int u = stack.top();
            stack.pop();
            for (int v = 0; v <= V-1; ++v) {
                if(!visisted[v] && rGraph[u][v]) {
                    visisted[v] = true;
                    stack.push(v);
                }
            }
        }

        return visisted;
    }

    int V = 0;
    int s = 0;
    int t = 0;
    std::vector<std::vector<int>> graph;
};

int main() {
    std::cout << "!!! Flow network min cut algorithm !!!" << std::endl;

    FlowNetwork flowNetwork(6, 0, 5);
    flowNetwork.addEdge(0,1,16);
    flowNetwork.addEdge(0,2,13);
    flowNetwork.addEdge(1,2,10);
    flowNetwork.addEdge(1,3,12);
    flowNetwork.addEdge(2,1,4);
    flowNetwork.addEdge(2,4,14);
    flowNetwork.addEdge(3,2,9);
    flowNetwork.addEdge(3,5,20);
    flowNetwork.addEdge(4,3,7);
    flowNetwork.addEdge(4,5,4);

    std::vector<std::pair<int,int>> minCut = flowNetwork.computeMinCut();
    std::cout << "Min Cut" << std::endl;
    for (auto &e: minCut) {
        std::cout << e.first << ", " << e.second << std::endl;
    }
    return 0; 
}