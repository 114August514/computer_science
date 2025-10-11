
#ifndef INC_2048_GAMERENDER_H
#define INC_2048_GAMERENDER_H

#include "GameBoard.h"

class TerminalRender { // 可视化界面（工具类）
public:
    TerminalRender() = default;
    static void render(const GameBoard& gameBoard, int score, int moveCount); // static：静态函数（无需创建对象，直接使用类名即可调用）
    static void clearScreen(); // 清空界面（你被调用了吗<疑问.jpg>）
    static void displayWelcomeMessage(); // 欢迎信息
    static void displayWinMessage(); // 胜利信息
    static void displayLoseMessage(); // 失败信息
    static void displayElapsedTime(double elapsedTime); // 当前游戏时间

private:
    static void displayHelpMessage(); // 帮助信息
};

#endif //INC_2048_GAMERENDER_H
