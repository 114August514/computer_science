//
// Created by Admin on 25-11-24.
//

#include <stdio.h>
#include <AGraph.h>

int main()
{
    EdgeDef vex0[] = {{1, 10}, {3, 100}, {-1, 0}};
    EdgeDef vex1[] = {{2, 20}, {-1, 0}};
    EdgeDef vex2[] = {{0, 10}, {3, 30}, {-1, 0}};
    EdgeDef vex3[] = {{4, 20}, {-1, 0}};
    EdgeDef vex4[] = {{-1, 0}};

    EdgeDef* adjData[] = {vex0, vex1, vex2, vex3, vex4};
    AGraph* G = CreateDG(sizeof(adjData) / sizeof(adjData[0]), adjData);
    if (G == NULL) return -1;

    struct TestCase
    {
        int u;         // 起点
        int v;         // 终点
        int k;         // 目标权值和
        bool expected; // 预期结果
    } tests[] = 
    {
        {0, 3, 60, true},  // 路径: 0->1->2->3
        {0, 3, 100, true}, // 路径: 0->3
        {0, 3, 50, false}, // 不存在
        {0, 2, 30, true},  // 路径: 0->1->2
        {0, 2, 10, false}, // 0->1 是 10，但没到终点 2
        {0, 0, 0, true},   // 原地不动，权值为 0
        {1, 3, 50, true},  // 路径: 1->2->3 (20+30=50)
        {3, 0, 10, false}  // 3 没有出边，无法到达 0
    };

    int numTests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < G->vexnum; i++)
    {
        DFS(G, i);
        DFS2(G, i);
    }

    printf("Start to test HasPathLenK!\n");
    for (int i = 0; i < numTests; i++)
    {
        bool result = HasPathLenK(G, tests[i].u, tests[i].v, tests[i].k);
        printf("Case %d: %d -> %d (k=%4d) | 预期: %-5s | 实际: %-5s | %s\n",
               i + 1,
               tests[i].u, tests[i].v, tests[i].k,
               tests[i].expected ? "True" : "False",
               result ? "True" : "False",
               (result == tests[i].expected) ? "PASS" : "FAIL"
        );
    }

    DestroyDG(G);
    printf("over!\n");

    return 0;
}