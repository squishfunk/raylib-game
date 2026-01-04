#include "ecs.h"
#include <stdio.h>

int ecs_create_entity(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if(!ecs->entities[i].active){
            ecs->entities[i] = (Entity){0};
            ecs->entities[i].active = true;

            ecs->transforms.active[i] = false;
            ecs->velocities.active[i] = false;
            ecs->renderables.active[i] = false;
            ecs->healths.active[i] = false;
            return i;
        }
    }

    if(ecs->entityCount >= MAX_ENTITIES){
        return -1;
    }

    int entityId = ecs->entityCount++;
    ecs->entities[entityId] = (Entity){0};
    ecs->entities[entityId].active = true;
    
    ecs->transforms.active[entityId] = false;
    ecs->velocities.active[entityId] = false;
    ecs->renderables.active[entityId] = false;
    ecs->healths.active[entityId] = false;
    return entityId;
}

void ecs_destroy_entity(ECS *ecs, int entityId){
    if(entityId < 0 || entityId >= ecs->entityCount) return;
    
    ecs->entities[entityId].active = false;
    
    ecs->transforms.active[entityId] = false;
    ecs->velocities.active[entityId] = false;
    ecs->renderables.active[entityId] = false;
    ecs->healths.active[entityId] = false;
}

void ecs_add_tranform(ECS *ecs, int entityId, Vector2 position){
    ecs->transforms.data[entityId].position = position;
    ecs->transforms.active[entityId] = true;
}

void ecs_add_velocity(ECS *ecs, int entityId, Vector2 velocity){
    ecs->velocities.data[entityId].velocity = velocity;
    ecs->velocities.active[entityId] = true;
}

void ecs_add_renderable(ECS *ecs, int entityId, float radius, Color color){
    ecs->renderables.data[entityId].radius = radius;
    ecs->renderables.data[entityId].color = color;
    ecs->renderables.active[entityId] = true;
}

void ecs_add_health(ECS *ecs, int entityId, int initialHealthPoints, int maxHealthPoints){
    ecs->healths.data[entityId].healthPoints = initialHealthPoints;
    ecs->healths.data[entityId].maxHealthPoints = maxHealthPoints;
    ecs->healths.data[entityId].lastDamageTime = 0.0f;
    ecs->healths.active[entityId] = true;
}

void ecs_add_enemy(ECS *ecs, int entityId, EnemyComponent *enemyComponent){
    ecs->enemies.data[entityId] = *enemyComponent;
    ecs->enemies.active[entityId] = true;
}

// void ecs_remove_transform(ECS *ecs, int entityId){
//     ecs->transforms.active[entityId] = false;
// }

// void ecs_remove_velocity(ECS *ecs, int entityId){
//     ecs->velocities.active[entityId] = false;
// }

// void ecs_remove_renderable(ECS *ecs, int entityId){
//     ecs->renderables.active[entityId] = false;
// }

// void ecs_remove_health(ECS *ecs, int entityId){
//     ecs->healths.active[entityId] = false;
// }

TransformComponent* ecs_get_transform(ECS *ecs, int entityId){
    if(entityId < 0 || entityId >= ecs->entityCount) return NULL;
    if(ecs->transforms.active[entityId])
        return &ecs->transforms.data[entityId];
    return NULL;
}

VelocityComponent* ecs_get_velocity(ECS *ecs, int entityId){
    if(entityId < 0 || entityId >= ecs->entityCount) return NULL;
    if(ecs->velocities.active[entityId])
        return &ecs->velocities.data[entityId];
    return NULL;
}

RenderableComponent* ecs_get_renderable(ECS *ecs, int entityId){
    if(entityId < 0 || entityId >= ecs->entityCount) return NULL;
    if(ecs->renderables.active[entityId])
        return &ecs->renderables.data[entityId];
    return NULL;
}

HealthComponent* ecs_get_health(ECS *ecs, int entityId){
    if(entityId < 0 || entityId >= ecs->entityCount) return NULL;
    if(ecs->healths.active[entityId])
        return &ecs->healths.data[entityId];
    return NULL;
}
