#include <raylib.h>
#include <stdio.h>
#include "game.h"
#include "ecs.h"

#define GAME_NAME "GOWNO"

void game_init(void){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(60);
}

void game_cleanup(void){
    CloseWindow();
}

void game_state_menu_update(Game *game){
    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
        game->currentState = GAME_STATE_PLAYING;
        
        game->ecs = (ECS){0};
        game->playerId = player_create(&game->ecs, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        game->lastShootTime = 0.0f;
    }
    
    if(IsKeyPressed(KEY_ESCAPE)){
        CloseWindow();
    }
}

void game_state_menu_render(void){
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

void game_state_playing_update(Game *game){
    float currentTime = GetTime();
    
    if(IsKeyPressed(KEY_ESCAPE)){
        game->currentState = GAME_STATE_PAUSED;
        return;
    }
    
    player_input_system(&game->ecs, game->playerId);
    player_shooting_system(&game->ecs, game->playerId, currentTime, &game->lastShootTime);
    enemy_spawn_system(&game->ecs);
    enemy_movement_system(&game->ecs);
    movement_system(&game->ecs);
    collision_system(&game->ecs);
    health_system(&game->ecs);
    
    if(!game->ecs.entities[game->playerId].active){
        game->currentState = GAME_STATE_GAME_OVER;
    }
}

void game_state_playing_render(Game *game){
    ClearBackground(RAYWHITE);
    render_system(&game->ecs);
}

void game_state_game_over_update(Game *game){
    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
        game->currentState = GAME_STATE_MENU;
    }
    if(IsKeyPressed(KEY_ESCAPE)){
        CloseWindow();
    }
}

void game_state_game_over_render(void){
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

void game_state_paused_update(Game *game){
    if(IsKeyPressed(KEY_ESCAPE)){
        game->currentState = GAME_STATE_PLAYING;
    }
    if(IsKeyPressed(KEY_Q)){
        game->currentState = GAME_STATE_MENU;
    }
}

void game_state_paused_render(Game *game){
    ClearBackground((Color){200, 200, 200, 255});
    render_system(&game->ecs);
    
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

void game_run(void){
    Game game = {0};
    game.currentState = GAME_STATE_MENU;
    
    while (!WindowShouldClose())
    {
        switch(game.currentState){
            case GAME_STATE_MENU:
                game_state_menu_update(&game);
                break;
            case GAME_STATE_PLAYING:
                game_state_playing_update(&game);
                break;
            case GAME_STATE_PAUSED:
                game_state_paused_update(&game);
                break;
            case GAME_STATE_GAME_OVER:
                game_state_game_over_update(&game);
                break;
        }
        
        BeginDrawing();
        switch(game.currentState){
            case GAME_STATE_MENU:
                game_state_menu_render();
                break;
            case GAME_STATE_PLAYING:
                game_state_playing_render(&game);
                break;
            case GAME_STATE_PAUSED:
                game_state_paused_render(&game);
                break;
            case GAME_STATE_GAME_OVER:
                game_state_game_over_render();
                break;
        }
        EndDrawing();
    }
}