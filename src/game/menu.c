#include "game.h"
#include <stdio.h>

void game_state_menu_update(Game *game){
    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
        game->currentState = GAME_STATE_PLAYING;
        
        game->ecs = (ECS){0};
        map_init(&game->map);
        map_generate(&game->map);
        room_generate(game);
        
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