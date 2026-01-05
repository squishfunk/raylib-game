#include "game.hpp"
#include "../components/components.hpp"
#include "../systems/player_system.hpp"
#include "../systems/enemy_system.hpp"
#include "../systems/movement_system.hpp"
#include "../systems/collision_system.hpp"
#include "../systems/health_system.hpp"
#include "../systems/render_system.hpp"
#include "../systems/bullet_system.hpp"
#include "../map/map.hpp"
#include <raylib.h>
#include <cstdio>
#include <cstring>

Game::Game() : currentState(GameState::MENU), playerId(-1), lastShootTime(0.0f) {
}

Game::~Game() {
}

void Game::init() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(60);
}

void Game::cleanup() {
    CloseWindow();
}

void Game::run() {
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
        currentState = GameState::PLAYING;
        
        // Reset ECS
        ecs = ECS();
        
        // Initialize map
        map.init();
        map.generate();
        Map::generateRoom(*this);
        
        // Create player
        playerId = PlayerSystem::createPlayer(ecs, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        lastShootTime = 0.0f;
    }
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
    }
}

void Game::renderMenu() const {
    ClearBackground(DARKBLUE);
    
    char title[100];
    snprintf(title, sizeof(title), "%s GAME", GAME_NAME);
    
    int titleSize = 60;
    int titleX = SCREEN_WIDTH/2 - MeasureText(title, titleSize)/2;
    int titleY = SCREEN_HEIGHT/2 - 100;
    DrawText(title, titleX, titleY, titleSize, WHITE);
    
    const char *instruction = "Press ENTER to start";
    int instSize = 30;
    int instX = SCREEN_WIDTH/2 - MeasureText(instruction, instSize)/2;
    int instY = SCREEN_HEIGHT/2 + 50;
    DrawText(instruction, instX, instY, instSize, LIGHTGRAY);
    
    const char *exit = "Press ESC to exit";
    int exitSize = 20;
    int exitX = SCREEN_WIDTH/2 - MeasureText(exit, exitSize)/2;
    int exitY = SCREEN_HEIGHT/2 + 100;
    DrawText(exit, exitX, exitY, exitSize, GRAY);
}

void Game::enterRoomSystem() {
    auto* playerTransform = ecs.getTransform(playerId);
    if (!playerTransform) return;
    
    int currentX = map.getCurrentX();
    int currentY = map.getCurrentY();
    
    if (playerTransform->position.y < 50 && map.canMoveTo(currentX, currentY - 1)) {
        map.moveToRoom(*this, currentX, currentY - 1);
        playerTransform->position.y = SCREEN_HEIGHT - 100;
    }
    if (playerTransform->position.y > SCREEN_HEIGHT - 50 && map.canMoveTo(currentX, currentY + 1)) {
        map.moveToRoom(*this, currentX, currentY + 1);
        playerTransform->position.y = 100;
    }
    if (playerTransform->position.x < 50 && map.canMoveTo(currentX - 1, currentY)) {
        map.moveToRoom(*this, currentX - 1, currentY);
        playerTransform->position.x = SCREEN_WIDTH - 100;
    }
    if (playerTransform->position.x > SCREEN_WIDTH - 50 && map.canMoveTo(currentX + 1, currentY)) {
        map.moveToRoom(*this, currentX + 1, currentY);
        playerTransform->position.x = 100;
    }
}

void Game::updatePlaying() {
    float currentTime = GetTime();
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = GameState::PAUSED;
        return;
    }
    
    enterRoomSystem();
    PlayerSystem::handleInput(ecs, playerId);
    PlayerSystem::handleShooting(ecs, playerId, currentTime, lastShootTime);
    EnemySystem::updateMovement(ecs);
    MovementSystem::update(ecs);
    CollisionSystem::update(ecs);
    HealthSystem::update(ecs);
    
    Map::checkRoomCleared(*this);
    
    if (!ecs.getEntities()[playerId].active) {
        currentState = GameState::GAME_OVER;
    }
}

void Game::renderPlaying() const {
    ClearBackground(RAYWHITE);
    
    map.renderCurrentRoom();
    
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
    
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 150});
    
    const char *text = "PAUSED";
    int size = 60;
    int x = SCREEN_WIDTH/2 - MeasureText(text, size)/2;
    int y = SCREEN_HEIGHT/2 - 50;
    DrawText(text, x, y, size, WHITE);
    
    const char *resume = "Press ESC to resume";
    int resumeSize = 30;
    int resumeX = SCREEN_WIDTH/2 - MeasureText(resume, resumeSize)/2;
    int resumeY = SCREEN_HEIGHT/2 + 50;
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
    int x = SCREEN_WIDTH/2 - MeasureText(text, size)/2;
    int y = SCREEN_HEIGHT/2 - 50;
    DrawText(text, x, y, size, WHITE);
    
    const char *restart = "Press ENTER to return to menu";
    int restartSize = 30;
    int restartX = SCREEN_WIDTH/2 - MeasureText(restart, restartSize)/2;
    int restartY = SCREEN_HEIGHT/2 + 50;
    DrawText(restart, restartX, restartY, restartSize, LIGHTGRAY);
}
