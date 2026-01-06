#include "game.hpp"
#include "../systems/player_system.hpp"
#include "../systems/enemy_system.hpp"
#include "../systems/movement_system.hpp"
#include "../systems/collision_system.hpp"
#include "../systems/health_system.hpp"
#include "../systems/render_system.hpp"
#include "../systems/bullet_system.hpp"
#include "../systems/door_system.hpp"
#include "../systems/room_system.hpp"
#include "../factories/player_factory.hpp"
#include "../map/map.hpp"
#include "../map/dungeon.hpp"
#include <cassert>
#include <raylib.h>
#include <cstdio>

Game::Game(int screenWidth, int screenHeight, const std::string &gameName) : 
screenWidth(screenWidth), 
screenHeight(screenHeight), 
currentState(GameState::MENU), 
playerId(-1),
dungeonManager(nullptr)
{
    InitWindow(screenWidth, screenHeight, gameName.c_str());

    if (!IsWindowReady()) {
        fprintf(stderr, "Error: Failed to initialize window. Check DISPLAY variable.\n");
        return;
    }

    SetTargetFPS(60);
}

Game::~Game() {
    delete dungeonManager;

    if (IsWindowReady()) {
        CloseWindow();
    }
}

void Game::run() {
    if (!IsWindowReady()) {
        fprintf(stderr, "Error: Window not ready. Cannot run game.\n");
        return;
    }

    while (!WindowShouldClose()) {
        switch (currentState) {
            case GameState::MENU:
                updateMenu();
                break;
            case GameState::PLAYING:
                updatePlaying();
                break;
            case GameState::PAUSED:
                updatePaused();
                break;
            case GameState::GAME_OVER:
                updateGameOver();
                break;
        }
        
        BeginDrawing();
        switch (currentState) {
            case GameState::MENU:
                renderMenu();
                break;
            case GameState::PLAYING:
                renderPlaying();
                break;
            case GameState::PAUSED:
                renderPaused();
                break;
            case GameState::GAME_OVER:
                renderGameOver();
                break;
        }
        EndDrawing();
    }
}

void Game::updateMenu() {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        initGame();
    }
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
    }
}

void Game::renderMenu() const {
    ClearBackground(DARKBLUE);
    
    char title[100];
    snprintf(title, sizeof(title), "%s GAME", this->gameName.c_str());
    
    int titleSize = 60;
    int titleX = this->screenWidth / 2 - MeasureText(title, titleSize)/2;
    int titleY = this->screenHeight / 2 - 100;
    DrawText(title, titleX, titleY, titleSize, WHITE);
    
    const char *instruction = "Press ENTER to start";
    int instSize = 30;
    int instX = this->screenWidth / 2 - MeasureText(instruction, instSize)/2;
    int instY = this->screenHeight / 2 + 50;
    DrawText(instruction, instX, instY, instSize, LIGHTGRAY);
    
    const char *exit = "Press ESC to exit";
    int exitSize = 20;
    int exitX = this->screenWidth / 2 - MeasureText(exit, exitSize)/2;
    int exitY = this->screenHeight / 2 + 100;
    DrawText(exit, exitX, exitY, exitSize, GRAY);
}

void Game::initGame(){
    ecs = ECS();
    
    map.init();
    map.generate();

    playerId = PlayerFactory::create(ecs, {static_cast<float>(screenWidth / 2.0f), static_cast<float>(screenHeight / 2.0f)});

    int startX = map.getStartX();
    int startY = map.getStartY();

    const Room& startRoom = map.getRoom(startX, startY);
    assert(startRoom.type == RoomType::START && "Error: Start room is EMPTY!");

    dungeonManager = new Dungeon(ecs, map, playerId, eventBus);
    dungeonManager->loadRoom(startRoom, DoorFlags::NONE);

    currentState = GameState::PLAYING;
}

void Game::updatePlaying() {
    if (IsKeyPressed(KEY_R)) {
        initGame();
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = GameState::PAUSED;
        return;
    }
    
    PlayerSystem::update(ecs);
    BulletSystem::update(ecs, screenHeight, screenWidth);
    EnemySystem::update(ecs);
    MovementSystem::update(ecs);
    CollisionSystem::update(ecs);
    HealthSystem::update(ecs);
    RoomSystem::update(ecs, map);
    DoorSystem::update(ecs, eventBus);
    
    if (!ecs.getEntities()[playerId].active) {
        currentState = GameState::GAME_OVER;
    }
}

void Game::renderPlaying() const {
    ClearBackground(RAYWHITE);

    RenderSystem::render(ecs);
    
    map.renderMinimap(10, 10);
}

void Game::updatePaused() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = GameState::PLAYING;
    }
    if (IsKeyPressed(KEY_Q)) {
        currentState = GameState::MENU;
    }
}

void Game::renderPaused() const {
    ClearBackground((Color){200, 200, 200, 255});
    RenderSystem::render(ecs);
    
    DrawRectangle(0, 0, this->screenWidth, this->screenHeight, (Color){0, 0, 0, 150});
    
    const char *text = "PAUSED";
    int size = 60;
    int x = this->screenWidth/2 - MeasureText(text, size)/2;
    int y = this->screenHeight/2 - 50;
    DrawText(text, x, y, size, WHITE);
    
    const char *resume = "Press ESC to resume";
    int resumeSize = 30;
    int resumeX = this->screenWidth/2 - MeasureText(resume, resumeSize)/2;
    int resumeY = this->screenHeight/2 + 50;
    DrawText(resume, resumeX, resumeY, resumeSize, LIGHTGRAY);
}

void Game::updateGameOver() {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        currentState = GameState::MENU;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
    }
}

void Game::renderGameOver() const {
    ClearBackground(RED);
    
    const char *text = "GAME OVER";
    int size = 60;
    int x = this->screenWidth/2 - MeasureText(text, size)/2;
    int y = this->screenHeight/2 - 50;
    DrawText(text, x, y, size, WHITE);
    
    const char *restart = "Press ENTER to return to menu";
    int restartSize = 30;
    int restartX = this->screenWidth/2 - MeasureText(restart, restartSize)/2;
    int restartY = this->screenHeight/2 + 50;
    DrawText(restart, restartX, restartY, restartSize, LIGHTGRAY);
}
