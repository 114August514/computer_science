//
// Created by Admin on 25-11-24.
//

#ifndef AGRAPH_H
#define AGRAPH_H

#include <stdbool.h>
#define MAX_VERTEX_NUM 100  // 最大顶点数

typedef int InfoType; // 边信息类型
typedef int VertexType; // 顶点信息（名字啥的）

typedef enum
{
    DirectedGraph, // 有向无权图
    DirectedNetwork, // 有向有权图
    UndirectedGraph, // 无向无权图
    UndirectedNetwork // 无向有权图
} GraphType;

// 用以辅助初始化
typedef struct 
{
    int target; // 目标顶点下标
    InfoType info; // 权值/信息
} EdgeDef;

typedef struct ArcNode
{
    int adjvex; // 邻接点在数组中的下标
    InfoType info; // 信息（权值等）
    struct ArcNode* nextarc; // 下一邻接点指针（头插法）
} ArcNode;

typedef struct VNode
{
    VertexType data; // 顶点信息
    ArcNode* firstarc; // 首邻接点
} VNode, AdjList[MAX_VERTEX_NUM];

typedef struct AGraph
{
    AdjList vertics; // 顶点集
    int vexnum, arcnum; // 顶点数和边数
    GraphType kind; // 图的类型
} AGraph;

AGraph* CreateDG(int vexnum, EdgeDef* adjdata[]);
AGraph* CreateUN(int vexnum, EdgeDef* adjdata[]);
void DestroyDG(AGraph* G);

void DFS(AGraph* G, int v);
void DFS2(AGraph *G, int v);

bool HasPathLenK(AGraph* G,int u, int v, InfoType k);

#endif // AGRAPH_H
