#include "graph.h"

const int MAX_DIST = 0x7FFFFFFF;

class dijkstra{
    public:
        void ssp(int s);           //求解最短路径（single source shortest path）
        void print(int u);         //输出路径的辅助函数
    public:
        dijkstra() = default;
        dijkstra(graph& _g) : g(_g) {}
        ~dijkstra() = default;
    private:
        graph& g;
        struct Vertex{
            bool sure;              //最短路径是否确定，对应书中 S 数组
            int path;               //记录最短路径上的前驱，对应书中 Path 数组
            int dist;               //记录最短路径的长度，对应书中D数组
            Vertex() : sure(false), path(-1), dist(MAX_DIST) {}
        };
        Vertex vertex[MAX_VERTEX_NUM];
};