#ifndef GAME_HPP
#define GAME_HPP

#include "../ecs/ecs.hpp"
#include "../map/map.hpp"
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
    int playerId;
    
    float lastShootTime;
    Map map;
    
    void updateMenu();
    void updatePlaying();
    void updatePaused();
    void updateGameOver();
    
    void renderMenu() const;
    void renderPlaying() const;
    void renderPaused() const;
    void renderGameOver() const;
    
    void enterRoomSystem();
    
public:
    Game(int screenWidth, int screenHeight, const std::string &gameName);
    ~Game();
    
    void init();
    void run();
    void cleanup();
    
    ECS& getECS() { return ecs; }
    const ECS& getECS() const { return ecs; }
    Map& getMap() { return map; }
    const Map& getMap() const { return map; }
    int getPlayerId() const { return playerId; }
};

#endif // GAME_HPP
