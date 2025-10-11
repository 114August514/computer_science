#include <iostream>
#include "../include/Game.h"
#include "../include/GameRender.h"
#include "../include/InputHandler.h"

int main(){
    Game game;

    // Display welcome message
    TerminalRender::displayWelcomeMessage();

    // Wait for the start game command
    InputType command;
    while (true) {
        command = InputHandler::handleInput();
        if (command == InputType::START_GAME) {
            break;
        } else {
            std::cout << "Invalid input. Press 'E' to start the game.\n";
        }
    }

    // Initialize the game
    game.initGame();
    TerminalRender::render(game.getGameBoard(), game.getScore(), game.getMoveCount());

    // Game loop
    while (true) {
        // Handle user input
        InputType input = InputHandler::handleInput();

        if (input == InputType::UNDO) {
            // Implement undo functionality
            // TODO
            game.undoLastMove();
        } else if (input == InputType::QUIT_GAME) {
            // Think about how to handle quitting the game
            // TODO
            break;
        } else {
            Direction direction;
            switch (input) {
                case InputType::UP:
                    direction = Direction::UP;
                    break;
                case InputType::DOWN:
                    direction = Direction::DOWN;
                    break;
                case InputType::LEFT:
                    direction = Direction::LEFT;
                    break;
                case InputType::RIGHT:
                    direction = Direction::RIGHT;
                    break;
                default:
                    std::cout << "Invalid input. Use arrow keys to move, U to undo, or Q to quit.\n";
                    continue;
            }

            // Update the game state
            // TODO
            game.updateGame(direction);

        }

        // Render the updated game board
        // refer to how the game is initially rendered
        // TODO
        TerminalRender::clearScreen();
        TerminalRender::render(game.getGameBoard(), game.getScore(), game.getMoveCount());

        // Check for win or lose conditions
        if (game.hasWon()) {
            TerminalRender::displayWinMessage();
            break;
        } else if (game.hasLost()) {
            TerminalRender::displayLoseMessage();
            break;
        }
    }

    // Display elapsed time
    // TODO
    TerminalRender::displayElapsedTime(game.getElapsedTime());

    // 防止控制台窗口在运行结束时直接关闭。
    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}

