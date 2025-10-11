#include "../include/GameBoard.h"
#include <algorithm> // cpp标准库，包含常用算法
#include <random> // cpp标准库，包含随机数生成器
#include <chrono> // cpp标准库，包含时间相关功能

GameBoard::GameBoard() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count(); // auto 表示自动类型推断
    generator = std::mt19937(seed); // 使用当前时间作为随机数生成器的种子，进而生成不同的随机数生成器
    reset(); // 初始化棋盘
}

void GameBoard::reset() {
    // Initialize the board to be 4x4 with all values set to 0
    board = Board(4, std::vector<int>(4, 0));

    // 创建随机数分布器（前者表示范围起点，后者表示范围终点，闭区间）
    std::uniform_int_distribution<int> dist(0, 3);  // [0, 3] uniform distribution
    std::uniform_int_distribution<int> value_dist(0, 1);

    // Add two initial tiles（游戏开始时，有两个初始方块）
    for (int i = 0; i < 2; ++i) {
        int x = dist(generator);
        int y = dist(generator);
        while (board[x][y] != 0) { // Ensure we don't place a tile on an occupied cell
            x = dist(generator);
            y = dist(generator);
        }
        board[x][y] = (value_dist(generator) + 1) * 2; // Place either a 2 or a 4
    }
}

int GameBoard::move(Direction direction) {
    // Slide and merge, anyother?
    // TODO 1
    int moveScore = 0;
    Board temp_board = board; // 用于判断移动是否有效

    slide(direction);
    moveScore = merge(direction);
    slide(direction);

    if (moveScore == 0 && temp_board == board) return -1; // 无效的移动不进行添加新块儿操作

    // Add a new tile after every move
    // You can refer to the reset() function to see how to add a new tile
    // TODO 2
    std::uniform_int_distribution<int> dist(0, 3);
    std::uniform_int_distribution<int> value_dist(0, 1);

    int x = dist(generator);
    int y = dist(generator);
    while (board[x][y] != 0)
    {
        x = dist(generator);
        y = dist(generator);
    }
    board[x][y] = (value_dist(generator) + 1) * 2;
    return moveScore;
}

void GameBoard::slide(Direction direction) {
    // 约定：棋盘的左下角为 (0, 0)，棋盘在第一象限，横轴为 col，纵轴为 row。
    switch (direction) {
        case Direction::UP:
            for (int col = 0; col < 4; ++col) {
                int target = 3;
                for (int row = 3; row >= 0; --row) {
                    if (board[row][col] != 0) {
                        std::swap(board[target][col], board[row][col]); // 交换
                        if (target != row) board[row][col] = 0; // 这行是不是多余的（疑问.jpg）
                        --target;
                    }
                }
            }
            break;
        case Direction::DOWN:
            // TODO
            for (int col = 0; col < 4; col++)
            {
                int target = 0;
                for (int row = 0; row < 4; row++)
                {
                    if (board[row][col] != 0)
                    {
                        std::swap(board[target][col], board[row][col]);
                        target++;
                    }
                }
            }
            break;
        case Direction::LEFT:
            // TODO
            for (int row = 0; row < 4; row++)
            {
                int target = 0;
                for (int col = 0; col < 4; col++)
                {
                    if (board[row][col] != 0)
                    {
                        std::swap(board[row][target], board[row][col]);
                        target++;
                    }
                }
            }
            break;
        case Direction::RIGHT:
            // TODO
            for (int row = 0; row < 4; row++)
            {
                int target = 3;
                for (int col = 3; col >= 0; col--)
                {
                    if (board[row][col] != 0)
                    {
                        std::swap(board[row][target], board[row][col]);
                        target--;
                    }
                }
            }
            break;
    }
}

int GameBoard::merge(Direction direction) {
    int moveScore = 0;
    switch (direction) {
        case Direction::UP:
            for (int col = 0; col < 4; ++col) {
                for (int row = 3; row > 0; --row) {
                    if (board[row][col] != 0 && board[row][col] == board[row - 1][col]) {
                        board[row][col] *= 2;
                        moveScore += board[row][col];
                        board[row - 1][col] = 0;
                    }
                }
            }
            break;
        case Direction::DOWN:
            // TODO
            for (int col = 0; col < 4; col++)
            {
                for (int row = 0; row < 3; row++)
                {
                    if (board[row][col] !=0 && board[row][col] == board[row+1][col])
                    {
                        board[row][col] *= 2;
                        moveScore += board[row][col];
                        board[row+1][col] = 0;
                    }
                }
            }
            break;
        case Direction::LEFT:
            // TODO
            for (int row = 0; row < 4; row++)
            {
                for (int col = 0; col < 3; col++)
                {
                    if (board[row][col] != 0 && board[row][col] == board[row][col+1])
                    {
                        board[row][col] *= 2;
                        moveScore += board[row][col];
                        board[row][col+1] = 0;
                    }
                }
            }
            break;
        case Direction::RIGHT:
            // TODO
            for (int row = 0; row < 4; row++)
            {
                for (int col = 3; col > 0; col--)
                {
                    if (board[row][col] != 0 && board[row][col] == board[row][col-1])
                    {
                        board[row][col] *= 2;
                        moveScore += board[row][col];
                        board[row][col-1] = 0;
                    }
                }
            }
            break;
    }

    return moveScore;
}

bool GameBoard::isGameOver() const {
    // Check if there are any empty tiles
    // TODO
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            if (board[row][col] == 0) return false;
        }
    }

    // Check if any adjacent tiles can merge
    // TODO
    // 检查竖直方向
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 3; row++)
        {
            if (board[row][col] == board[row+1][col]) return false;
        }
    }

    // 检查水平方向
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (board[row][col] == board[row][col+1]) return false;
        }
    }

    return true;
}

bool GameBoard::hasWinningTile() const {
    // Check if there is a tile with the value 2048
    // TODO
    for (int col = 0; col < 4; col++)
        for (int row = 0; row < 4; row++)
            if (board[row][col] == 2048) return true;
    
    return false;
}


const Board &GameBoard::getBoard() const {
    return board;
}