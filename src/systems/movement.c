#include "../ecs.h"
#include <raymath.h>

void movement_system(ECS *ecs){
    float deltaTime = GetFrameTime(); 
    for(int i = 0; i < ecs->entityCount; i++){
        if (!ecs->entities[i].active) continue;
        if (!ecs->transforms.active[i] || !ecs->velocities.active[i]) continue;

        ecs->transforms.data[i].position.x += ecs->velocities.data[i].velocity.x * deltaTime;
        ecs->transforms.data[i].position.y += ecs->velocities.data[i].velocity.y * deltaTime;
    }
}