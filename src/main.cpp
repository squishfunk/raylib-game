#include "game/game.hpp"

int main() {
    Game game;
    game.init();
    game.run();
    game.cleanup();
    return 0;
}

