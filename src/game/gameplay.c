#include "game.h"

void game_state_playing_update(Game *game){
    float currentTime = GetTime();
    
    if(IsKeyPressed(KEY_ESCAPE)){
        game->currentState = GAME_STATE_PAUSED;
        return;
    }
    
    TransformComponent *playerTransform = ecs_get_transform(&game->ecs, game->playerId);
    if(playerTransform){
        if(playerTransform->position.y < 50 && map_can_move_to(&game->map, game->map.currentX, game->map.currentY - 1)){
            map_move_to_room(&game->map, game->map.currentX, game->map.currentY - 1);
            playerTransform->position.y = SCREEN_HEIGHT - 100;  
        }
        if(playerTransform->position.y > SCREEN_HEIGHT - 50 && map_can_move_to(&game->map, game->map.currentX, game->map.currentY + 1)){
            map_move_to_room(&game->map, game->map.currentX, game->map.currentY + 1);
            playerTransform->position.y = 100;  
        }
        if(playerTransform->position.x < 50 && map_can_move_to(&game->map, game->map.currentX - 1, game->map.currentY)){
            map_move_to_room(&game->map, game->map.currentX - 1, game->map.currentY);
            playerTransform->position.x = SCREEN_WIDTH - 100;  
        }
        if(playerTransform->position.x > SCREEN_WIDTH - 50 && map_can_move_to(&game->map, game->map.currentX + 1, game->map.currentY)){
            map_move_to_room(&game->map, game->map.currentX + 1, game->map.currentY);
            playerTransform->position.x = 100;  
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