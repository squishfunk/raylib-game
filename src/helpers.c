#include <raylib.h>
#include <math.h>
#include "helpers.h"
#include "ecs.h"

bool check_circle_collision(Vector2 pos1, float radius1, Vector2 pos2, float radius2){
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    float distance = sqrtf(dx*dx + dy*dy);
    float minDistance = radius1 + radius2;
    return distance < minDistance;
}

int get_player_id(ECS *ecs){
    int playerId = -1;
    for(int i = 0; i < ecs->entityCount; i++){
        if(ecs->entities[i].active && (ecs->entities[i].tags & TAG_PLAYER)){
            playerId = i;
            break;
        }
    }
    return playerId;
}