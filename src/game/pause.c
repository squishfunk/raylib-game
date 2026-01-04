#include "game.h"

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