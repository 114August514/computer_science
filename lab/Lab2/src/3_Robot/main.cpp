#include "walk.h"

int main() {
    int steps = 6;
    int n = 5;

    // TODO:修改了数据类型。
    DATA_STRUCTURE::Point gold[501] = 
    {
        {1, 2}, 
        {3, 3}, 
        {5, 0}, 
        {1, 1},
        {4, 5}
    };
    int value[501] = {3, 2, 1, 4, 10};
    //std::cin >> steps >> n;
    //for (int i = 0; i < n; i++)
        //scanf("%d%d%d", gold + 2 * i, gold + 2 * i + 1, value + i);
    DATA_STRUCTURE::walk p(steps, n, gold, value);
    p.print_para();
    std::cout << p.get_value() << std::endl;
    return 0;
}
