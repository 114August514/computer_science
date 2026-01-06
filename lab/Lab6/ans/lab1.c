// printf 单纯用以测试，不干扰其他代码的正常功能

#include <stdio.h>
#include <stdint.h>

static int16_t judge(int16_t cond, int16_t val1, int16_t val2)
{
    // 暂时默认 cond 仅为 0 或 1
    if (cond == 0)
        return val2;

    return val1;
}

static void test_judge()
{
    const int test_num = 4;
    const int16_t cond_array[] = {0, 1,  1,  0};
    const int16_t val1_array[] = {5, 5, -3, -7};
    const int16_t val2_array[] = {9, 9,  4, -2};
    const int16_t expected_result[] = {9, 5, -3, -2};

    for (int i = 0; i < test_num; i++)
    {
        int16_t result = judge(cond_array[i], val1_array[i], val2_array[i]);
        printf("test %d: %d / %d\n", i, result, expected_result[i]);
    }
}

int main()
{
    test_judge();
    return 0;
}