#include <iostream>
#include <vector>
#include <map>
#include <queue>


class Graph {
public:
    Graph(int V, int s, int t) : V(V), s(s), t(t) {
        matrix.resize(V, std::vector<int>(V, 0));
    }
    Graph (const Graph& g) {
        s = g.s;
        t = g.t;
        V = g.V;
        matrix = g.matrix;
    }

    void addEdge(int u, int v, int w) {
        if (matrix[u][v] != 0) {
            std::cerr << "Parallel edges are not allowed! skipped u: " << u << " , v: " << v << " ,w: " << w << std::endl;
            return;
        }
        matrix[u][v] = w;
    }

    void update(int u, int v, int w) {
        matrix[u][v] += w;
    }

    int get(int u, int v) const{
        return matrix[u][v];
    }
    int getSource() const{
        return s;
    }
    int getSink() const{
        return t;
    }

    std::vector<int> getParentPath(int s, int t) const{
        std::vector<int> parent(V, -1);
        std::queue<int> queue;
        std::vector<bool> visited(V, false);
        visited[s] = true;
        queue.push(s);

        bool found = false;
        while(!queue.empty()) {
            int u = queue.front();
            queue.pop();

            for (int v = 0; v <= V-1; ++v) {
                if (visited[v] == false && matrix[u][v] > 0) {

                    parent[v] = u;
                    queue.push(v);
                    visited[v] = true;

                    if (v == t) {
                        found = true;
                        break;    
                    }
                }
            }
        }

        if (found)
            return parent;
        
        return std::vector<int>();
    }

private:
    int s = 0;
    int t = 0;
    int V = 0;
    std::vector<std::vector<int>> matrix;
};

class FordFulkersonMaxFlow {
public:
    FordFulkersonMaxFlow(const Graph &g) :g(g) {}
    int compute( ) {
        int maxFlow = 0;
        Graph rGraph(g);

        int s = rGraph.getSource();
        int t = rGraph.getSink();

        std::vector<int> parent = rGraph.getParentPath(s,t);
        while(!parent.empty()) {
            int minPathFlow = std::numeric_limits<int>::max();
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                minPathFlow = std::min(minPathFlow, rGraph.get(u,v));
            }

            maxFlow += minPathFlow;

            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                rGraph.update(u,v, -minPathFlow);
                rGraph.update(v,u, minPathFlow);
            }

            parent = rGraph.getParentPath(s,t);
        }

        return maxFlow;
    }

private:
    Graph g;
 };

int main() {
    std::cout << "Ford Fulkerson Max Flow Algorithm" << std::endl;
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


    FordFulkersonMaxFlow maxFlow(g);
    std::cout << "MaxFlow: " << maxFlow.compute() << std::endl;


    /*
    Output: 

    The maximum possible flow is 23
    */
    return 0;
}