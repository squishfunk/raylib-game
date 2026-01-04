#include "game.h"

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