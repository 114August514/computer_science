#include <cstdio>
#define MAX_VERTEX_NUM 15 // 最大顶点数（改了下命名风格）

typedef int InfoType;

class graph{
    public:
        int vertex_num, edge_num;                               // 记录图的顶点数和边数
        struct Edge{
            int adj;                                            // 邻接点在数组的下标
            InfoType weight;                                    // 权值
            Edge *next;
            Edge(int _adj, InfoType _weight, Edge* _next=nullptr)
                : adj(_adj), weight(_weight), next(_next) {}
        };                                                      // 记录边
        struct Vertex{
            Edge *head;
            bool visited;                                       // 是否被访问，可以用于深度优先遍历
        };                                                      // 记录顶点
        Vertex vertex[MAX_VERTEX_NUM];

    public:
        graph() = default;
        graph(int _vertex, int _edge)
        {
            vertex_num = _vertex;
            edge_num = _edge;
        }
        ~graph() // 避免内存泄露
        {
            for (int i = 0; i < vertex_num; i++)
            {
                Edge* current = vertex[i].head;
                while (current != nullptr)
                {
                    Edge* temp = current;
                    current = current->next;
                    delete temp;
                }
            }
        }

        void init(int start[], int end[], InfoType weight[]);   // 初始化
        void dfs(int start_vertex);                                   // 深度优先遍历

    private:
        void dfs_recursive(int v, bool* isFirst); // 用于递归的辅助函数
};