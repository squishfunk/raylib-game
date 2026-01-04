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

void game_state_playing_update(Game *game){
    float currentTime = GetTime();
    
    if(IsKeyPressed(KEY_ESCAPE)){
        game->currentState = GAME_STATE_PAUSED;
        return;
    }
    
    // Sprawdź przejście między pokojami
    TransformComponent *playerTransform = ecs_get_transform(&game->ecs, game->playerId);
    if(playerTransform){
        // Przejście przez górne drzwi
        if(playerTransform->position.y < 50 && map_can_move_to(&game->map, game->map.currentX, game->map.currentY - 1)){
            map_move_to_room(&game->map, game->map.currentX, game->map.currentY - 1);
            playerTransform->position.y = SCREEN_HEIGHT - 100;  // teleport na dół
        }
        // Przejście przez dolne drzwi
        if(playerTransform->position.y > SCREEN_HEIGHT - 50 && map_can_move_to(&game->map, game->map.currentX, game->map.currentY + 1)){
            map_move_to_room(&game->map, game->map.currentX, game->map.currentY + 1);
            playerTransform->position.y = 100;  // teleport na górę
        }
        // Przejście przez lewe drzwi
        if(playerTransform->position.x < 50 && map_can_move_to(&game->map, game->map.currentX - 1, game->map.currentY)){
            map_move_to_room(&game->map, game->map.currentX - 1, game->map.currentY);
            playerTransform->position.x = SCREEN_WIDTH - 100;  // teleport na prawo
        }
        // Przejście przez prawe drzwi
        if(playerTransform->position.x > SCREEN_WIDTH - 50 && map_can_move_to(&game->map, game->map.currentX + 1, game->map.currentY)){
            map_move_to_room(&game->map, game->map.currentX + 1, game->map.currentY);
            playerTransform->position.x = 100;  // teleport na lewo
        }
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
    
    map_render_current_room(&game->map);
    
    render_system(&game->ecs);
    
    map_render_minimap(&game->map, 10, 10);
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