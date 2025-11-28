#include "../include/ExpressionBinaryTree.h"
#include <stdexcept>

using namespace std;

// 构造函数，初始化节点的值
ExpressionBinaryTreeNode::ExpressionBinaryTreeNode(string val) : value(val) {}

// 获取节点的值
string ExpressionBinaryTreeNode::getValue() {
    return value;
}

// 从后缀表达式构建表达式树
// 参数:
//    postfix - 后缀表达式字符串
// 返回:
//    返回构建的表达式树根节点
ExpressionBinaryTreeNode* ExpressionBinaryTreeNode::buildFromPostfix(const string& postfix) {
    // TODO
    stack<ExpressionBinaryTreeNode*> operandStack; // 存储操作数的栈
    
    for (size_t i = 0; i < postfix.size(); i++)
    {
        char ch = postfix[i];

        if (isdigit(ch))
            // 1. 如果是数字，创造新节点并压入栈
            operandStack.push(new ExpressionBinaryTreeNode(string(1, ch)));
            
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
        {
            // 2. 处理运算符
            ExpressionBinaryTreeNode* node = new ExpressionBinaryTreeNode(string(1, ch));

            ExpressionBinaryTreeNode* right = operandStack.top();
            operandStack.pop();

            ExpressionBinaryTreeNode* left = operandStack.top();
            operandStack.pop();

            // 连接节点
            node->left = left;
            node->right = right;
            operandStack.push(node);
        }
    }

    return operandStack.empty() ? nullptr : operandStack.top();
}

// 从前缀表达式构建表达式树
// 参数:
//    prefix - 前缀表达式字符串
// 返回:
//    返回构建的表达式树根节点
ExpressionBinaryTreeNode* ExpressionBinaryTreeNode::buildFromPrefix(const string& prefix) {
    // TODO
    // 从右向左扫描字符串时与后缀表达式构建方式差不多
    stack<ExpressionBinaryTreeNode*> operandStack;

    for (int i = prefix.size() - 1; i >= 0; i--)
    {
        // 由于 size_t 是 unsigned，故改用 int
        char ch = prefix[i];

        if (isdigit(ch))
            operandStack.push(new ExpressionBinaryTreeNode(string(1, ch)));
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
        {
            ExpressionBinaryTreeNode* node = new ExpressionBinaryTreeNode(string(1, ch));
            ExpressionBinaryTreeNode* left = operandStack.top();
            operandStack.pop();
            ExpressionBinaryTreeNode* right = operandStack.top();
            operandStack.pop();

            node->left = left;
            node->right = right;
            operandStack.push(node);
        }
    }
    return operandStack.empty() ? nullptr : operandStack.top();
}

// 从中缀表达式构建表达式树
// 参数:
//    infix - 中缀表达式字符串
// 返回:
//    返回构建的表达式树根节点
// 由于上次的实验写过，为了减轻大家的负担，不用实现中缀表达式树的构建，以下代码供大家参考
ExpressionBinaryTreeNode* ExpressionBinaryTreeNode::buildFromInfix(const string& infix) {
    stack<ExpressionBinaryTreeNode*> operandStack;  // 存储操作数的栈
    stack<char> operatorStack;  // 存储运算符的栈
    
    for (size_t i = 0; i < infix.size(); ++i) {
        char ch = infix[i];
        
        if (isdigit(ch)) {
            // 如果是数字，创建新的操作数节点并压入操作数栈
            operandStack.push(new ExpressionBinaryTreeNode(string(1, ch))); // string(1, ch): 将 ch 重复 1 次
        } else if (ch == '(') {
            // 如果是左括号，直接入运算符栈
            operatorStack.push(ch);
        } else if (ch == ')') {
            // 如果是右括号，弹出运算符直到遇到左括号
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                char op = operatorStack.top();
                operatorStack.pop();
                ExpressionBinaryTreeNode* right = operandStack.top();
                operandStack.pop();
                ExpressionBinaryTreeNode* left = operandStack.top();
                operandStack.pop();
                ExpressionBinaryTreeNode* node = new ExpressionBinaryTreeNode(string(1, op));
                node->left = left;
                node->right = right;
                operandStack.push(node);
            }
            // 弹出左括号
            if (!operatorStack.empty() && operatorStack.top() == '(') {
                operatorStack.pop();
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            // 处理运算符
            while (!operatorStack.empty() && getPrecedence(operatorStack.top()) >= getPrecedence(ch)) {
                char op = operatorStack.top();
                operatorStack.pop();
                ExpressionBinaryTreeNode* right = operandStack.top();
                operandStack.pop();
                ExpressionBinaryTreeNode* left = operandStack.top();
                operandStack.pop();
                ExpressionBinaryTreeNode* node = new ExpressionBinaryTreeNode(string(1, op));
                node->left = left;
                node->right = right;
                operandStack.push(node);
            }
            operatorStack.push(ch);
        }
    }

    // 处理剩余的运算符
    while (!operatorStack.empty()) {
        char op = operatorStack.top();
        operatorStack.pop();
        ExpressionBinaryTreeNode* right = operandStack.top();
        operandStack.pop();
        ExpressionBinaryTreeNode* left = operandStack.top();
        operandStack.pop();
        ExpressionBinaryTreeNode* node = new ExpressionBinaryTreeNode(string(1, op));
        node->left = left;
        node->right = right;
        operandStack.push(node);
    }

    // 返回最终构建好的表达式树的根节点（TODO：多加了个是否为空的判断）
    return operandStack.empty()? nullptr : operandStack.top();
}

// 获取运算符的优先级
// 参数:
//    op - 运算符字符
// 返回:
//    返回运算符的优先级，数字越大优先级越高
int ExpressionBinaryTreeNode::getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;  // 加减优先级较低
    if (op == '*' || op == '/') return 2;  // 乘除优先级较高
    return INT_MAX;  // 不支持的运算符返回最大优先级
}

// 计算表达式树的值
// 返回:
//    返回表达式树的计算结果
int ExpressionBinaryTreeNode::evaluate() {
    // 如果是叶节点（没有子节点），则直接返回该节点的数字值
    if (!left && !right) {
        return stoi(value);  // 使用stoi将字符串转换为整数
    }
    
    // 否则递归计算左右子树的值（TODO：修改了动态转换，因为原本的动态转换没有验明转换是否成功）
    int leftVal = static_cast<ExpressionBinaryTreeNode*>(left)->evaluate();    
    int rightVal = static_cast<ExpressionBinaryTreeNode*>(right)->evaluate();
    
    // 根据当前节点的运算符计算值
    if (value == "+") return leftVal + rightVal;
    if (value == "-") return leftVal - rightVal;
    if (value == "*") return leftVal * rightVal;
    if (value == "/") 
    {
        if (rightVal == 0) throw runtime_error("Division by zero");
        return leftVal / rightVal;
    }
    
    // 如果是无效的运算符，抛出异常
    throw invalid_argument("Invalid operator");
}
