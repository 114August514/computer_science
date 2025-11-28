#include "../include/dijkstra.h"
#include <iostream>
void dijkstra::ssp(int s)
{
    // TODO: 实现以s为起始点的dijkstra算法求最短路径
    // 1. 初始化
    for (int i = 0; i < g.vertex_num; i++)
        vertex[i] = Vertex();

    vertex[s].dist = 0;

    // 2. 主循环
    for (int i = 0; i < g.vertex_num; i++)
    {
        // 找到距离最小的点
        int target = -1;
        int minDist = MAX_DIST;

        for (int j = 0; j < g.vertex_num; j++)
        {
            if (!vertex[j].sure && vertex[j].dist < minDist)
            {
                minDist = vertex[j].dist;
                target = j;
            }
        }

        if (target == -1) break; // 说明遍历结束
        vertex[target].sure = true;

        // 更换顶点
        graph::Edge* p = g.vertex[target].head;
        while (p != nullptr)
        {
            int _adj = p->adj;
            InfoType _weight = p->weight;

            if (!vertex[_adj].sure && _weight != MAX_DIST 
                && vertex[target].dist + _weight < vertex[_adj].dist)
            {
                vertex[_adj].dist = vertex[target].dist + _weight;
                vertex[_adj].path = target;
            }
            p = p->next;
        }
    }
}

void dijkstra::print(int u)
{
    // TODO: 用于输出最短路径的辅助函数，可以使用递归实现
    // 1. 递归退出条件
    if (vertex[u].path == -1)
    {
        // 若 dist == MAX_DIST 表示该节点够不到
        if (vertex[u].dist != MAX_DIST)
            std::cout << u;
        return;
    }

    // 2. 递归核心
    print(vertex[u].path);
    std::cout << " -> " << u;
}