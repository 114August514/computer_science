#ifndef INC_2048_GAME_H
#define INC_2048_GAME_H

#include "GameBoard.h"
#include <stack> // 栈容器
#include <chrono>

class Game {
public:
    Game();
    void initGame(); // 重启游戏以及第一次开始游戏都用到找个函数
    void updateGame(Direction direction);
    void undoLastMove();
    bool hasWon() const;
    bool hasLost() const;
    double getElapsedTime() const; // 返回游戏时长
    int getScore() const;
    int getMoveCount() const;

    const GameBoard& getGameBoard() const;

private:
    GameBoard gameBoard;
    int score{}; // {} 初始化
    int moveCount{};
    std::stack<GameBoard> historyGameBoards; // 存放GameBoard类型的栈变量
    std::stack<int> historyScores; // TODO 新增分数的栈（因为我不会用两个棋盘直接判断产生的分数）
    std::chrono::time_point<std::chrono::system_clock> startTime;
};

// 栈的主要成员函数
// 1. push(element): 将一个元素压入栈顶（向栈中添加数据的唯一方式）
// 2. pop(): 弹出并移除栈顶的元素（无返回值，只负责移除）
// 3. top(): 返回对栈顶元素的引用（但不移除）
// 4. empty(): 检查栈是否为空
// 5. size(): 返回栈中元素的数量

#endif //INC_2048_GAME_H
