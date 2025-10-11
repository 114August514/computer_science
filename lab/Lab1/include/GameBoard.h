#ifndef INC_2048_GAMEBOARD_H
#define INC_2048_GAMEBOARD_H

#include <vector>
#include <random>

// 使用 enum class 定义枚举类型（相比传统枚举更安全）
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// 创建类型别名
// 声明一个空的、存放 int 的 vector
// std::vector<int> vec1;
// 声明并初始化，包含 10, 20, 30
// std::vector<int> vec2 = {10, 20, 30};
// 声明一个包含 5 个元素的 vector，所有元素都默认初始化为 0
// std::vector<int> vec3(5);
// 声明一个包含 5 个元素的 vector，所有元素都初始化为 100
// std::vector<int> vec4(5, 100);
// std::vector<int>(5, 100) 创建了一个包含 5 个元素的 vector，所有元素都初始化为 100（临时量）
using Board = std::vector<std::vector<int>>;

class GameBoard {
public:
    GameBoard(); // 构造函数：要求与类名相同且无返回类型
    void reset();
    int move(Direction direction);
    bool isGameOver() const; // const 成员函数，表示不会修改对象（const置于最后表示是常量函数，内部不会修改成员变量）
    bool hasWinningTile() const;
    const Board& getBoard() const; // const置于前表示返回的是常量引用（外部无法修改）（&是引用声明符，可认为创建别名）

private: // 私有成员变量和函数（只能在类的内部访问）
    Board board; // 实际的游戏棋盘
    std::mt19937 generator; // Mersenne Twister 随机数生成器（梅森旋转算法）
    int merge(Direction direction);
    void slide(Direction direction);
};

#endif //INC_2048_GAMEBOARD_H
