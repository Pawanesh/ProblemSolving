#include <iostream>
#include <vector>
#include <stack>
#include <limits>
#include <queue>

struct Edge {
    int u = 0;
    int v = 0;
    int c = 0;
    int f = 0;
};
using Edges = std::vector<Edge>;

class Graph {
public: 
    Graph(int V) : V(V) {
        adj.resize(V, Edges());
    }

    void addEdge(int u, int v, int c = 0, int f = 0) {
        Edge e{u,v,c,f};
        adj[u].emplace_back(e);
    }

    Edges& edges(int u) {
        return adj[u];
    }

    void updateFlow(int u, int v, int f) {
        for (auto &e : edges(u)) {
            if (e.v == v) {
                e.f += f;
                break;
            }
        }
    }

    int getEffectiveCapacity(int u, int v) {
        for (auto &e : edges(u)) {
            if (e.v == v) {
                return e.c - e.f;
            }
        }
        return 0;
    }

    int getEffectiveCapacity(Edge &e) {
        return e.c - e.f;
    }

    std::vector<int> bfs(int s, int t) {
        std::vector<int> path (V, -1);
        std::vector<bool> visisted(V, false);
        std::queue<int> queue;
        visisted[s] = true;
        queue.push(s);
        while(!queue.empty()) {
            int u = queue.front();
            queue.pop();
            for (auto &e : edges(u)) {
                int v = e.v;
                if (!visisted[v] && getEffectiveCapacity(e)) {
                    visisted[v] = true;
                    path[v] = u;
                    queue.push(v);

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
    FlowNetwork(int V, int s, int t) : s(s), t(t), g(V) {}

    void addEdge(int u, int v, int c = 0, int f = 0) {
        g.addEdge(u,v,c,f);
    }

    int computeMaxFlow() {
        Graph rGraph = g;

        int maxFlow = 0;
        std::vector<int> path = rGraph.bfs(s,t);
        while(!path.empty()) {
            int bottleNeck = getBottleNeck(rGraph, path);
            maxFlow += bottleNeck;
            update(rGraph, path, bottleNeck);

            path = rGraph.bfs(s,t);
        }

        return maxFlow;
    }

private:
    int getBottleNeck(Graph& rGraph, std::vector<int>& path) {
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

    void update(Graph & rGraph, std::vector<int> &path, int bottleNeck) {
        for (int v = t; v != s; v = path[v]) {
            int u = path[v];
            rGraph.updateFlow(u,v,bottleNeck);
            rGraph.addEdge(v,u,0,-bottleNeck);
        }
    }

    int s  = 0;
    int t = 0;
    Graph g;
};

class MaximumBipartiteMatching {
public:
    MaximumBipartiteMatching(int applicantSize, int jobSize) : applicantSize(applicantSize), jobSize(jobSize) {
        applicantJobList.resize(applicantSize, std::vector<int>());
    }

    void add(int applicantID, int jobID) {
        applicantJobList[applicantID].emplace_back(jobID);
    }

    int compute(){
        FlowNetwork flowNetwork = createFlowNetwork();
        return flowNetwork.computeMaxFlow();
    }


private:

    FlowNetwork createFlowNetwork() {
        FlowNetwork flowNetwork(applicantSize + jobSize + 2, applicantSize + jobSize, applicantSize + jobSize + 1);
        for (int i = 0; i <= applicantJobList.size()-1; ++i) {
            std::vector<int> jobs = applicantJobList[i];
            
            int currJobSize = jobs.size();

            for (int j = 0; j <= currJobSize-1; ++j) {
                int jobID = jobs[j] + applicantSize;
                flowNetwork.addEdge(i, jobID, 1);
            }
        }

        int superSource = applicantSize + jobSize;
        for (int i = 0; i <= applicantSize-1; ++i) {
            flowNetwork.addEdge(superSource, i, 1);
        }

        int superSink = superSource + 1;
        for (int i = 0; i <= jobSize-1; ++i) {
            flowNetwork.addEdge(applicantSize + i, superSink, 1);
        }

        return flowNetwork;
    }


    int applicantSize = 0; 
    int jobSize = 0;
    std::vector<std::vector<int>> applicantJobList;
};

int main() {
    std::cout << "Maximum Bipartite Matching" << std::endl;
    MaximumBipartiteMatching applicantsJobsMatching(6,6);
    applicantsJobsMatching.add(0,1);
    applicantsJobsMatching.add(0,2);
    applicantsJobsMatching.add(2,0);
    applicantsJobsMatching.add(2,3);
    applicantsJobsMatching.add(3,2);
    applicantsJobsMatching.add(4,2);
    applicantsJobsMatching.add(4,3);
    applicantsJobsMatching.add(5,5);

    std::cout << "Maximum applicants and jobs matching are: " << applicantsJobsMatching.compute() << std::endl;
    return 0;
}