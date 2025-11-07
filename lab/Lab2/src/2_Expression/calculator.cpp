#include <ctype.h>
#include "calculator.h"

namespace DATA_STRUCTURE {

// TODO
static calculator::CharType get_char_type(char c)
{
    if (isdigit(c)) return calculator::CharType::Num;
    switch (c)
    {
        case '.':
            return calculator::CharType::Dot;
        
        case '(':
            return calculator::CharType::LeftBracket;

        case ')':
            return calculator::CharType::RightBracket;

        case '^':
            return calculator::CharType::FirstOperator;

        case '*':
        case '/':
            return calculator::CharType::SecondOperator;

        case '+':
        case '-':
            return calculator::CharType::ThirdOperator;

        default:
            return calculator::CharType::Error;
    }
}

std::string calculator::get_expr() {
    return expr;
}

bool calculator::legal() {
    int bracket = 0;
    // TODO
    const std::string& s = expr;
    CharType last_type = CharType::Start;

    for (size_t i = 0; i < s.size(); i++)
    {
        char c = s[i];
        if (isspace(c)) continue;

        CharType current_type = get_char_type(c);

        // 非法字符捕捉
        if (current_type == CharType::Error)
        {
            std::cout << "TypeError!" << std::endl;
            return false;
        }

        // 处理数字部分的逻辑（读取完）（我允许 .5 和 1. 这样的数字存在，但不允许仅 '.' ）
        if (current_type == CharType::Num || current_type == CharType::Dot)
        {
            if (last_type == CharType::RightBracket || last_type == CharType::Num)
                return false;

            int dot_count = 0;
            size_t start = i;

            while (i < s.size())
            {
                current_type = get_char_type(s[i]);
                if (current_type == CharType::Num);
                else if (current_type == CharType::Dot) dot_count++;
                else break; // 数字中间不允许空格

                i++;
            }

            if (dot_count > 1) return false;
            if (dot_count == 1 && i - start == 1) return false;

            i--; // 退出情况都会 i + 1
            last_type = CharType::Num;
            continue;
        }

        // 处理运算符部分的逻辑
        switch (current_type)
        {
            case CharType::LeftBracket:
                // 左括号前不能是 Num, RightBracket
                if (last_type == CharType::Num || 
                    last_type == CharType::RightBracket) return false;

                bracket++;
                last_type = CharType::LeftBracket;
                break;

            case CharType::RightBracket:
                // 右括号前只能是 Num, RightBracket
                if ((bracket == 0) || 
                    (last_type != CharType::Num && 
                     last_type != CharType::RightBracket)) return false;

                bracket--;
                last_type = CharType::RightBracket;
                break;

            case CharType::FirstOperator:
            case CharType::SecondOperator:
                // 二元运算符，前面只能是 Num, RightBracket
                if (last_type != CharType::Num &&
                    last_type != CharType::RightBracket) return false;

                last_type = current_type;
                break;
            
            case CharType::ThirdOperator:
                // 前面是 Start, LeftBracket 时，可以认为是一元运算符
                // 前面是 Num, RightBracket 认定为二元运算符
                if (last_type == CharType::Start || 
                    last_type == CharType::LeftBracket);
                else if (last_type == CharType::Num || 
                    last_type == CharType::RightBracket);
                else return false;

                last_type = CharType::ThirdOperator;
                break;

            default:
                return false;
        }
    }

    // 出串判断（仅允许 Num, RightBracket 结尾）
    if (bracket != 0) return false;
    return (last_type == CharType::Num || last_type == CharType::RightBracket);
}

int calculator::priority_regular(char c) {
    // TODO
    // 数字越大，优先级越高
    CharType current_type = get_char_type(c);
    switch (current_type)
    {
        case CharType::FirstOperator:
            return 3;
            
        case CharType::SecondOperator:
            return 2;

        case CharType::ThirdOperator:
            return 1;
        
        default:
            return 0;
    }
}

int calculator::priority(char c1, char c2) {
    // TODO
    // c1, c2 理论上来说应该是两个运算符
    // :return 的值为 1 时表示 c2 优先级更高（压入栈），为 0 时表示 c1 优先级更高（弹出栈）。
    if (c2 == ')') return 0; // 排除 ')' 的错误进入。

    int p1 = priority_regular(c1);
    int p2 = priority_regular(c2);
    // 排除特殊情况（以及防括号）
    if (p1 == 0 || p2 == 0) return 1;
    if (p1 != p2) return p1 < p2;

    // 进入此时 p1 = p2，此时要考虑结合性
    if (p1 == 3) return 1; // '^' 是右结合
    else return 0; // 其余四则运算认为是左结合
}

struct calculator::element calculator::read_num() {
    // 默认调用该函数时，是数字部分。
    // 调用结束后，不再指向数字部分。
    int num = 0, flag = 0;
    // TODO
    int expo = expr_index;
    const std::string& s = expr;

    // 跳过空格部分（保护程序）
    while (expo < expr.size() && isspace(s[expo])) expo++;

    // 读取整数部分
    while (expo < expr.size() && get_char_type(s[expo]) == CharType::Num)
    {
        num = num *10 + s[expo] - '0';
        expo++;
    }

    // 读取小数部分
    if (expo < expr.size() && get_char_type(s[expo]) == CharType::Dot)
    {
        flag = 1;
        expo++;
        double factor = 0.1;
        double decimal = (double) num;

        while (expo < expr.size() && get_char_type(s[expo]) == CharType::Num)
        {
            decimal += factor * (s[expo] - '0');
            factor *= 0.1;
            expo++;
        }

        expr_index = expo;
        return {flag, 0, decimal};
    }

    expr_index = expo;
    return {flag, num, 0.0};
}

struct calculator::element calculator::operate(struct calculator::element element1, char c, struct calculator::element element2) {
    // TODO
    // 只要有一个数是浮点型 或 除法有余数 或 乘方运算中指数位为负数 就返回浮点型
    int flag = 0;
    double v1 = (element1.flag == 0) ? (double) element1.num_int : element1.num_double;
    double v2 = (element2.flag == 0) ? (double) element2.num_int : element2.num_double;
    double result = 0.0;

    if (element1.flag || element2.flag) flag = 1;

    switch (c)
    {
        case '+':
            result = v1 + v2;
            break;

        case '-':
            result = v1 - v2;
            break;

        case '*':
            result = v1 * v2;
            break;

        case '/':
            // 排除除 0 错误
            // 借用 flag 的 负数部分表示错误码
            if (fabs(v2) < 1e-9)
            {
                std::cout << "DivideError!" << std::endl;
                return {-1, 0, 0.0};
            }
            result = v1 / v2;

            if (flag == 0) 
                flag = (element1.num_int % element2.num_int != 0);
            break;
        
        case '^':
            // 排除 0 的负数次幂
            if (fabs(v1) < 1e-9 && v2 < 0.0)
            {
                std::cout << "PowerError!" << std::endl;
                return {-1, 0, 0.0};
            }

            // 排除 负数 的非整数次幂
            if (v1 < 0.0 && floor(v2) != v2)
            {
                std::cout << "PowerError!" << std::endl;
                return {-2, 0, 0.0};
            }
            result = pow(v1, v2);

            if (flag == 0)
                flag = (element2.num_int < 0);
            break;

        default:
            std::cout << "OpError!" << std::endl;
            return {-3, 0, 0.0};
    }
    
    if (flag) return {flag, 0, result};
    return {flag, (int) result, 0.0};
}

struct calculator::element calculator::get_ans() {
    // TODO
    // 保护逻辑
    if (!legal())
    {
        std::cout << "ExpressionError!" << std::endl;
        return {-4, 0, 0.0};
    }

    // 初始化
    while (!num.empty()) num.pop();
    while (!op.empty()) op.pop();
    expr_index = 0;
    CharType last_type = CharType::Start;

    while (expr_index < expr.size())
    {
        char c = expr[expr_index];

        // 跳过空格
        if (isspace(c))
        {
            expr_index++;
            continue;
        }

        CharType current_type = get_char_type(c);
        
        // 处理数字部分的逻辑
        if (current_type == CharType::Num || current_type == CharType::Dot)
        {
            num.push(read_num());
            last_type = CharType::Num;
            continue;
        }

        // 左括号处理逻辑
        if (current_type == CharType::LeftBracket)
        {
            op.push(c);
            expr_index++;
            last_type = current_type;
            continue;
        }

        // 右括号处理逻辑（一直运算到前一个左括号）
        if (current_type == CharType::RightBracket)
        {
            while (!op.empty() && op.top() != '(')
            {
                struct element e2 = num.top();
                num.pop();
                struct element e1 = num.top();
                num.pop();
                char op_char = op.top();
                op.pop();
                struct element temp = operate(e1, op_char, e2);
                if (temp.flag < 0) return {temp.flag, 0, 0.0};
                num.push(temp);
            }
            if (!op.empty()) op.pop();
            expr_index++;
            last_type = current_type;
            continue;
        }

        // 一元运算符处理逻辑
        // 相当于前面缺 0 的 二元运算符
        if ((last_type == CharType::Start || 
             last_type == CharType::LeftBracket) && 
             current_type == CharType::ThirdOperator) num.push({0, 0, 0.0});

        // 二元运算符处理逻辑
        // 高阶入栈，低阶计算
        while (!op.empty() && priority(op.top(), c) == 0)
        {
            struct element e2 = num.top();
            num.pop();
            struct element e1 = num.top();
            num.pop();
            char op_char = op.top();
            op.pop();
            struct element temp = operate(e1, op_char, e2);
            if (temp.flag < 0) return {temp.flag, 0, 0.0};
            num.push(temp);
        }

        op.push(c);
        expr_index++;
        last_type = current_type;
    }
    
    // 结束逻辑（栈可能未清空）
    while(!op.empty())
    {
        struct element e2 = num.top();
        num.pop();
        struct element e1 = num.top();
        num.pop();
        char op_char = op.top();
        op.pop();
        struct element temp = operate(e1, op_char, e2);
        if (temp.flag < 0) return {temp.flag, 0, 0.0};
        num.push(temp);
    }

    struct element final_result{};
    if (num.empty()) return final_result;

    final_result = num.top();
    num.pop();

    // 保护程序
    while (!num.empty()) num.pop();
    while (!op.empty()) op.pop();

    return final_result;
}

}