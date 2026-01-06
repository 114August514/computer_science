// 使用 gatchar() 对标 IN
// 使用 putchar() 对标 OUT

#include <stdio.h>
#include <stdint.h>

static void IS_Y(int16_t result)
{
    putchar(result + '0');
}

static void hint_error()
{
    putchar('?');
    putchar('\n');
}

static void mod_7()
{
    int16_t result = 0;
    while (1)
    {
        int16_t input_char = getchar();
        switch (input_char)
        {
            case '\n':
            case '\r':
            case ' ':
                continue;

            case 'y':
                IS_Y(result);
                return;

            case '0':
                result += result;
                break;

            case '1':
                result += result;
                result += 1;
                break;

            default:
                hint_error();
                break;
        }

        if (result >= 7)
            result -= 7;
    }
}

int main()
{
    mod_7();
    return 0;
}