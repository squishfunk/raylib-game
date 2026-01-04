#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include "../ecs.h"

void health_system(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if(!ecs->entities[i].active) continue;
        if(!ecs->healths.active[i]) continue;
        
        if(ecs->healths.data[i].healthPoints <= 0){
            ecs->entities[i].active = false;
            printf("Entity %d died!\n", i);
        }
    }
}