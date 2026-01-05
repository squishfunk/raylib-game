#ifndef GAME_HPP
#define GAME_HPP

#include "../ecs/ecs.hpp"
#include "../map/map.hpp"

constexpr const char* GAME_NAME = "GOWNO";

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
private:
    GameState currentState;
    ECS ecs;
    int playerId;
    float lastShootTime;
    Map map;
    
    // State update methods
    void updateMenu();
    void updatePlaying();
    void updatePaused();
    void updateGameOver();
    
    // State render methods
    void renderMenu() const;
    void renderPlaying() const;
    void renderPaused() const;
    void renderGameOver() const;
    
    // Gameplay helper
    void enterRoomSystem();
    
public:
    Game();
    ~Game();
    
    void init();
    void run();
    void cleanup();
    
    // Getters
    ECS& getECS() { return ecs; }
    const ECS& getECS() const { return ecs; }
    Map& getMap() { return map; }
    const Map& getMap() const { return map; }
    int getPlayerId() const { return playerId; }
    GameState getState() const { return currentState; }
    void setState(GameState state) { currentState = state; }
};

#endif // GAME_HPP
