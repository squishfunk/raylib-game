#include "game/Game.hpp"

constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 650;
constexpr const char* GAME_NAME = "GOWNO";

int main() {
    Game game(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    game.run();
    return 0;
}

