#include "../include/dijkstra.h"
#include <iostream>
#define MAX_EDGE_NUM 105

int main()
{
    
    int n = 6; // 顶点数
    int m = 8; // 边数
    int s = 0; // source
    int u[MAX_EDGE_NUM] = {0, 0, 0, 1, 2, 3, 4, 4}; // start[]
    int v[MAX_EDGE_NUM] = {2, 4, 5, 2, 3, 5, 5, 3}; // end[]
    int w[MAX_EDGE_NUM] = {10, 30, 100, 5, 50, 10, 60, 20}; // weight[]
    //std::cin >> n >> m >> s;
    //for(int i = 0; i < m; i++)
        //std::cin >> u[i] >> v[i] >> w[i];

    std::cout << "--------------------------" << std::endl;
    std::cout << "travelsal" << std::endl;
    std::cout << "--------------------------" << std::endl;
    graph g(n, m);
    g.init(u, v, w);
    g.dfs(s);
    std::cout << std::endl << std::endl;

    dijkstra dij(g);
    std::cout <<"--------------------------" << std::endl;
    std::cout << "dijkstra" << std::endl;
    std::cout << "--------------------------" << std::endl;
    dij.ssp(s);
    for (int i = 0; i < n; i++)
    {
        std::cout << "Path to " << i << ": ";
        dij.print(i); // 调用你写的 print 函数
        std::cout << std::endl;
    }
}