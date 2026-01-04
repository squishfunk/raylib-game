#include <raylib.h>
#include "game.h"
#include "../ecs.h"
#include "../map.h"

void game_init(void){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(60);
}

void game_cleanup(void){
    CloseWindow();
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