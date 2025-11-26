#include "walk.h"

int main() {
    int steps = 22;
    int n = 7;

    // TODO:修改了数据类型。
    DATA_STRUCTURE::Point gold[501] = 
    {
        {1, 3},
        {10, -2},
        {7, 3},
        {-2, 8},
        {-9, -5},
        {-1, -6},
        {8, -7}
    };
    int value[501] = {4, 6, 8, 5, 5, 7, 10};
    //std::cin >> steps >> n;
    //for (int i = 0; i < n; i++)
        //scanf("%d%d%d", gold + 2 * i, gold + 2 * i + 1, value + i);
    DATA_STRUCTURE::walk p(steps, n, gold, value);
    p.print_para();
    std::cout << p.get_value() << std::endl;
    return 0;
}
