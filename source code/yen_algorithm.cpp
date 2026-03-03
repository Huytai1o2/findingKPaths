#include "yen_algorithm.h"

#define V 70
#define INF 1e6+5
using namespace std;

int minDistance(float dist[], bool sptSet[]) {
    int min = INF, min_index = -1;

    for (int v = 1; v < V; v++) {
        if (!sptSet[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

float dijkstra(float **graph, int src, int d, vector<int> &pathh) {
    float dist[V];
    int tback[V];
    bool sptSet[V];
    pathh.clear();

    for (int i = 1; i < V; i++) {
        dist[i] = INF;
        sptSet[i] = false;
        tback[i] = -1;
    }

    dist[src] = 0;

    for (int count = 1; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);

        if (u == -1) break;  // No more reachable vertices

        sptSet[u] = true;

        for (int v = 1; v < V; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                tback[v] = u;
            }
        }
    }
    // cout << "huong" << endl;
    if (dist[d] == INF) return INF;  // Destination not reachable
    // cout << pathh.size();
    int a = d;
    pathh.push_back(a);
    while (a != src)
    {
        a = tback[a];
        pathh.push_back(a);
    }
    reverse(pathh.begin(), pathh.end());
    // cout << dist[d];
    return dist[d];
}

Node** Yen(float **matrix, int s, int d, int &count)
{
    int size= count;
    vector<Node*> B;
    vector<int> initialPath;
    float temp[V][V];

    for (int i = 1; i < V; i++) {
        for (int j = 1; j < V; j++) {
            temp[i][j] = matrix[i][j];
        }
    }

    Node **A = (Node**)malloc((size+1) * sizeof(Node*));
    for (int i = 0; i < count; i++) {
        A[i] = new Node;
    }

    A[0]->dist = dijkstra(matrix, s, d, A[0]->trace);

    for (int k = 1; k < count; k++)
    {
        for (int i = 0; i <(int) A[k - 1]->trace.size() - 1; i++) {
            int spurNode = A[k - 1]->trace[i];
            vector<int> rootPath(A[k - 1]->trace.begin(), A[k - 1]->trace.begin() + i + 1);
            // for (auto u:rootPath) cout << u << " ";
            // cout << endl;

            float rootDist = 0;

            for (int i = 0; i < (int)rootPath.size() - 1; i++)
            {
                // qDebug() << temp[rootPath[i]][rootPath[i+1]];
                rootDist += temp[rootPath[i]][rootPath[i+1]];
            }

            for (int p = 0; p < k; p++)
            {
                if (rootPath == vector<int>(A[p]->trace.begin(), A[p]->trace.begin() + rootPath.size()))
                {
                    matrix[A[p]->trace[i]][A[p]->trace[i + 1]] = INF;
                }
            }

            for (auto rootPathNode : rootPath) {
                for (int node = 1; node < V; node++) {
                    if (rootPathNode!=spurNode && node !=spurNode)
                    {
                        matrix[rootPathNode][node] = INF;
                        matrix[node][rootPathNode] = INF;
                    }
                }
            }

            vector<int> spurPath;
            float spurDist = dijkstra(matrix, spurNode, d, spurPath);

            if (spurDist < INF)
            {
                Node *total = new Node;
                total->trace = rootPath;
                total->trace.insert(total->trace.end(), spurPath.begin() + 1, spurPath.end());
                // total->dist = spurDist + A[k - 1]->dist;
                total->dist = spurDist + rootDist;
                if (B.size()==0) B.push_back(total);
                else
                    for (int p = 0; p < (int)B.size(); p++)
                    {
                        if (total->trace == vector<int>(B[p]->trace.begin(), B[p]->trace.end()))
                        {
                            break;
                        }
                        if (p==(int)B.size()-1) B.push_back(total);
                    }

            }

            for (int x = 1; x < V; x++)
            {
                for (int y = 1; y < V; y++)
                {
                    matrix[x][y] = temp[x][y];
                }
            }
        }

        if (B.empty()) break;

        sort(B.begin(), B.end(), [](const Node* lhs, const Node* rhs) {
            return lhs->dist < rhs->dist;
        });

        A[k] = B.front();
        B.erase(B.begin());
    }
    for (int i = 0; i <(int)count; i++)
    {
        if (A[i]->trace.empty())
        {
            count = i;
            break;
        }
    }
    return A ;
}
