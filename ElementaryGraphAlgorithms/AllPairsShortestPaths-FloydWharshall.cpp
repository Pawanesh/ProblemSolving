#include <iostream>
#include <vector>

class FloydWarshall {
public:
    FloydWarshall (int V) : V(V) {
        adjMatrix.resize(V, std::vector<int>(V, 0));
        shortestPathWights.resize(V, std::vector<int>(V, INF));
        predecessors.resize(V, std::vector<int>(V, -1));
    }

    void addEdge(int u, int v, int w) {
        adjMatrix[u][v] = w;
    }

    void compute() {
        initialize();

        for (int k = 0; k <= V-1; ++k) {
            for (int i = 0; i <= V-1; ++i) {
                for (int j = 0; j <= V-1; ++j) {
                    if (shortestPathWights[i][k] + shortestPathWights[k][j] < shortestPathWights[i][j]) {
                        shortestPathWights[i][j] = shortestPathWights[i][k] + shortestPathWights[k][j];   
                        predecessors[i][j] = predecessors[k][j];
                    }
                    else {
                        shortestPathWights[i][j] = shortestPathWights[i][j];
                        predecessors[i][j] = predecessors[i][j];
                    }
                }
            }

        }

    }

    void print() {
        std::cout << "Shortest path wights: " << std::endl;
        for (int i = 0; i <= V-1; ++i) {
            for (int j = 0; j <= V-1; ++j) {
                std::cout << shortestPathWights[i][j] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "Shortest path predecessor: " << std::endl;
        for (int i = 0; i <= V-1; ++i) {
            for (int j = 0; j <= V-1; ++j) {
                std::cout << predecessors[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

     

private:
    void initialize() {
        for (int i = 0; i <= V-1; ++i) {
            for (int j = 0; j <= V-1; ++j) {
                if (adjMatrix[i][j]) {
                    shortestPathWights[i][j] = adjMatrix[i][j];
                    predecessors[i][j] = i;
                }
                if (i==j) {
                    shortestPathWights[i][j] = 0;
                }
            }
        }
    }

    int V = 0;
    std::vector<std::vector<int>> adjMatrix;
    std::vector<std::vector<int>> shortestPathWights;
    std::vector<std::vector<int>> predecessors;

    int INF = 9999;
};

int main () {
    std::cout << "All pairs shortest paths - Floyd Warshall Algorithm" << std::endl;
    FloydWarshall allPairsShortestPaths(5);
    allPairsShortestPaths.addEdge(0,1,3);
    allPairsShortestPaths.addEdge(0,4,-4);
    allPairsShortestPaths.addEdge(0,2,8);
    allPairsShortestPaths.addEdge(1,4,7);
    allPairsShortestPaths.addEdge(1,3,1);
    allPairsShortestPaths.addEdge(2,1,4);
    allPairsShortestPaths.addEdge(3,2,-5);
    allPairsShortestPaths.addEdge(3,0,2);
    allPairsShortestPaths.addEdge(4,3,6);

    allPairsShortestPaths.compute();
    allPairsShortestPaths.print();
}