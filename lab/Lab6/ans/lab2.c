// printf 单纯用以测试，不干扰其他代码的正常功能
// 预设 Q(N) 与 Q_array[N-1] 一一对应，且 1 <= N <= 100。

#include <stdio.h>
#include <stdint.h>

#define Q_ARRAY_SIZE 100

static int16_t cal_Q(int16_t N, int16_t* Q_array)
{
    int16_t temp1 = 1;  // 存 Q[n-1]
    int16_t temp2 = 1;  // 存 Q[n-2]
    for (int i = 0; i < N; i++)
    {
        if (i <= 1)
        {
            Q_array[i] = 1;
            continue;
        }

        Q_array[i] = Q_array[i - temp1] + Q_array[i - temp2];
        temp2 = temp1;
        temp1 = Q_array[i];
    }

    return temp1;
}

static void test_cal_Q(int16_t* Q_array)
{
    const int test_num = 3;
    const int16_t Input_N[] = {1, 5, 10};
    const int16_t expected_result[] = {1, 3, 6};

    for (int i = 0; i < test_num; i++)
    {
        int16_t result = cal_Q(Input_N[i], Q_array);
        printf("Q[%d] = %d / %d\n", Input_N[i], result, expected_result[i]);
    }
}

int main()
{
    int16_t Q_array[Q_ARRAY_SIZE] = {0};
    test_cal_Q(Q_array);
    return 0;
}