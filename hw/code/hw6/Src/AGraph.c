//
// Created by Admin on 25-11-24.
//

#include <stdlib.h>
#include <stdio.h>
#include "AGraph.h"

static AGraph* CreateGraphCore(int vexnum, EdgeDef* adjdata[], GraphType kind)
{
    // 输入的 adjdata 需要保证输入数据是完全的。
    if (adjdata == NULL) return NULL;
    if (vexnum <= 0 || vexnum >= MAX_VERTEX_NUM) return NULL;

    AGraph* G = (AGraph *)calloc(1, sizeof(AGraph));
    if (!G)
    {
        printf("CreateError!\n");
        return NULL;
    }

    G->vexnum = vexnum;
    G->kind = kind;
    G->arcnum = 0;

    for (int i = 0; i < vexnum; i++)
    {
        G->vertics[i].data = i; // 简化逻辑

        if (adjdata[i] == NULL) continue;

        // 约定：当 target 为 -1 时表示结束
        for (int j = 0; adjdata[i][j].target != -1; j++)
        {
            int target = adjdata[i][j].target;
            InfoType weight = adjdata[i][j].info;

            if (target < 0 || target >= vexnum)
            {
                printf("IndexError!\n");
                DestroyDG(G);
                return NULL;
            }

            ArcNode* p = (ArcNode*)malloc(sizeof(ArcNode));
            if (!p)
            {
                printf("CreateError!\n");
                DestroyDG(G);
                return NULL;
            }

            p->adjvex = target;
            p->info = weight;

            // 头插法
            p->nextarc = G->vertics[i].firstarc;
            G->vertics[i].firstarc = p;

            G->arcnum++;
        }
    }

    if (G->kind == UndirectedGraph || G->kind == UndirectedNetwork)
        G->arcnum /= 2;

    return G;
}

AGraph *CreateDG(int vexnum, EdgeDef* adjdata[])
{
    return CreateGraphCore(vexnum, adjdata, DirectedGraph);
}

AGraph *CreateUN(int vexnum, EdgeDef* adjdata[])
{
    return CreateGraphCore(vexnum, adjdata, UndirectedNetwork);
}

void DestroyDG(AGraph* G)
{
    if (G == NULL) return;
    for (int i = 0; i < G->vexnum; i++)
    {
        ArcNode* p = G->vertics[i].firstarc;

        while(p != NULL)
        {
            ArcNode* temp = p;
            p = p->nextarc;
            free(temp);
        }
    }
    free(G);
}

static void DFS_Recursive(AGraph* G, int v, bool visited[], bool* isFirst)
{
    // 访问当前顶点 v （简单逻辑，会随着 data 类型修正而修改）
    if (*isFirst)
    {
        printf("%d ", G->vertics[v].data);
        *isFirst = false;
    }
    else
        printf("-> %d ", G->vertics[v].data);

    visited[v] = true;

    // 遍历 v 的邻接表，对每个未访问的邻接点递归
    ArcNode* p = G->vertics[v].firstarc;
    while (p != NULL)
    {
        int target = p->adjvex;
        if (!visited[target])
            DFS_Recursive(G, target, visited, isFirst);
        p = p->nextarc;
    }
}

void DFS(AGraph* G, int v)
{
    if (G == NULL) return;
    if (v < 0 || v >= G->vexnum) return;

    bool* visited = (bool*)calloc(G->vexnum, sizeof(bool));
    if (!visited)
    {
        printf("CreateError!\n");
        return;
    }

    bool isFirst = true;

    printf("使用递归方式进行 DFS 中...\n");
    DFS_Recursive(G, v, visited, &isFirst);

    free(visited);
    printf("\n");
}

void DFS2(AGraph* G, int v)
{
    // 1. 判断是否输入合法
    if (G == NULL) return;
    if (v < 0 || v >= G->vexnum) return;

    // 2. 初始化
    bool* visited = (bool*)calloc(G->vexnum, sizeof(bool));
    if (!visited)
    {
        printf("CreateError!\n");
        return;
    }

    ArcNode** curArc = (ArcNode**)malloc(G->vexnum * sizeof(ArcNode*));
    if (!curArc)
    {
        printf("CreateError!\n");
        free(visited);
        return;
    }
    for (int i = 0; i < G->vexnum; i++) 
        curArc[i] = G->vertics[i].firstarc;

    struct Stack
    {
        int adjvex[MAX_VERTEX_NUM];
        int top;
    } stack;

    stack.top = -1;
    
    // 3. 核心逻辑
    printf("使用非递归方式进行 DFS 中...\n");

    printf("%d ", G->vertics[v].data);
    visited[v] = true;
    stack.adjvex[++stack.top] = v; // 栈的 push 操作

    while (stack.top != -1) // 栈不为空
    {
        int current = stack.adjvex[stack.top]; // 访问栈顶元素
        ArcNode* p = curArc[current];

        while (p != NULL && visited[p->adjvex])
            p = p->nextarc;

        curArc[current] = p;

        if (p != NULL)
        {
            int next = p->adjvex;
            printf("-> %d ", G->vertics[next].data);
            visited[next] = true;
            stack.adjvex[++stack.top] = next;
            curArc[current] = p->nextarc;
        }
        else stack.top--; // 出栈操作
    }

    free(visited);
    free(curArc);
    printf("\n");
}

static bool DFS_PathLenK(AGraph* G, int u, int v, InfoType k, bool visited[])
{
    if (k < 0) return false;
    if (u == v) return k == 0;

    visited[u] = true;

    ArcNode* p = G->vertics[u].firstarc;
    while(p != NULL)
    {
        int next = p->adjvex;
        InfoType weight = p->info;

        if (!visited[next])
        {
            if (DFS_PathLenK(G, next, v, k-weight, visited))
            {
                visited[u] = false;
                return true;
            }
        }
        p = p->nextarc;
    }

    // 回溯
    visited[u] = false;
    return false;
}

bool HasPathLenK(AGraph* G, int u, int v, InfoType k)
{
    // 参数检验
    if (G == NULL || u < 0 || u >= G->vexnum || v < 0 || v >= G->vexnum)
    {
        printf("InputError!\n");
        return false;
    }

    bool* visited = (bool*)calloc(G->vexnum, sizeof(bool));
    if (!visited)
    {
        printf("CreateError!\n");
        return false;
    }

    bool result = DFS_PathLenK(G, u, v, k, visited);

    free(visited);
    return result;
}