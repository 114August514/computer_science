#include "../include/Game.h"

Game::Game() = default;

void Game::initGame() {
    gameBoard.reset();
    score = 0;
    moveCount = 0;
    historyGameBoards = std::stack<GameBoard>();
    historyScores = std::stack<int>(); // TODO 清空历史
    startTime = std::chrono::system_clock::now();
    historyGameBoards.push(gameBoard);  // Save initial state（为啥历史记录记录到当前状态啊）
    historyScores.push(score); // TODO 模仿上行代码写的
}

void Game::updateGame(Direction direction) {
    // 理论上来说，执行该程序前，需判断 direction 操作是有意义的（即会导致棋盘的改变）。
    // Save current state to history
    // TODO 1
    int temp_score = gameBoard.move(direction);
    if (temp_score == -1) return;

    historyGameBoards.push(gameBoard);

    // Get move score and update score
    // TODO 2
    score += temp_score;
    historyScores.push(score);

    // update move count
    // TODO 3
    moveCount++;
}

void Game::undoLastMove() {
    // 好像没有处理分数和步数的回档（沉默.jpg）
    if (historyGameBoards.size() > 1) {
        // Remove current state
        // TODO 1
        historyGameBoards.pop();
        historyScores.pop();

        // Restore previous state
        // TODO 2
        gameBoard = historyGameBoards.top();
        score = historyScores.top();

        moveCount--;
    } 

    // TODO：感觉逻辑不太好，遂删除
    // else {
    //     gameBoard.reset(); // 感觉这里应该修改成 initgame() 或者 把 else 逻辑直接删掉。
    // }
}

bool Game::hasWon() const {
    return gameBoard.hasWinningTile();
}

bool Game::hasLost() const {
    return gameBoard.isGameOver();
}

double Game::getElapsedTime() const {
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = endTime - startTime;
    return elapsedSeconds.count();
}

int Game::getScore() const {
    return score;
}

int Game::getMoveCount() const {
    return moveCount;
}

const GameBoard& Game::getGameBoard() const {
    return gameBoard;
}