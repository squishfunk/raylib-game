#ifndef GAME_HPP
#define GAME_HPP

#include "../ecs/Ecs.hpp"
#include "../map/Map.hpp"
#include "../map/Dungeon.hpp"
#include "../events/EventBus.hpp"
#include <string>


enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
private:
    int screenWidth;
    int screenHeight;
    std::string gameName;

    GameState currentState;
    ECS ecs;
    EventBus eventBus;
    int playerId;
    Dungeon *dungeonManager;
    int currentLevel = 1;
    
    Map map;
    Camera2D camera;
    
    void updateMenu();
    void updatePlaying();
    void updatePaused();
    void updateGameOver();
    
    void renderMenu() const;
    void renderPlaying() const;
    void renderPaused() const;
    void renderGameOver() const;

    void initGame();
    
    void nextLevel();
public:
    Game(int screenWidth, int screenHeight, const std::string &gameName);
    ~Game();
    
    void run();
    
    ECS& getECS() { return ecs; }
    const ECS& getECS() const { return ecs; }
    Map& getMap() { return map; }
    const Map& getMap() const { return map; }
    int getPlayerId() const { return playerId; }
};

#endif // GAME_HPP
