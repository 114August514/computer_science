#include "../include/graph.h"
#include <stdexcept>
#include <iostream>

void graph::init(int start[], int end[], InfoType weight[])
{
    // TODO: 完成邻接表存储图
    // 把带参数的构造函数一部分移到了这里（保证即使调用的是 default 的构造函数也可以使用该 init）
    for (int i = 0; i < vertex_num; i++)
    {
        vertex[i].visited = false;
        vertex[i].head = nullptr;
    }

    for (int i = 0; i < edge_num; i++)
    {
        int _start = start[i];
        Edge* current = new Edge(end[i], weight[i]);
        
        //  头插法
        current->next = vertex[_start].head;
        vertex[_start].head = current;
    }
}

// 辅助函数
void graph::dfs_recursive(int v, bool* isFirst)
{
    if (*isFirst)
    {
        std::cout << v;
        *isFirst = false;
    }
    else 
        std::cout << " -> " << v;

    vertex[v].visited = true;
    Edge* p = vertex[v].head;
    while (p != nullptr)
    {
        int target = p->adj;
        if (!vertex[target].visited)
            dfs_recursive(target, isFirst);
        p = p->next;
    }
}

void graph::dfs(int start_vertex)
{
    // TODO: 完成图的深度优先遍历，输出顶点编号
    // 1. 重置 visited，允许重复 dfs
    for (int i = 0; i < vertex_num; i++)
        vertex[i].visited = false;

    // 2. 判断参数有无问题
    if (start_vertex < 0 || start_vertex >= vertex_num)
        throw std::invalid_argument ("Start vertex index out of bounds");

    // 3. 开始递归
    bool isFirst = true;
    dfs_recursive(start_vertex, &isFirst);
    std::cout << std::endl;
}