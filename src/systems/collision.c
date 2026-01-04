#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include "../ecs.h"
#include "../helpers.h"

void collision_system(ECS *ecs){
    float currentTime = GetTime();

    int playerId = get_player_id(ecs);

    if(playerId < 0 || !ecs->healths.active[playerId]) return;
    if(!ecs->transforms.active[playerId] || !ecs->renderables.active[playerId]) return;

    TransformComponent *playerTransform = &ecs->transforms.data[playerId];
    RenderableComponent *playerRenderable = &ecs->renderables.data[playerId];
    HealthComponent *playerHealth = &ecs->healths.data[playerId];

    for(int i = 0; i < ecs->entityCount; i++){
        if(i == playerId) continue;
        if(!ecs->entities[i].active) continue;
        if(!(ecs->entities[i].tags & TAG_ENEMY)) continue;
        if(!ecs->transforms.active[i] || !ecs->renderables.active[i]) continue;

        TransformComponent *enemyTransform = &ecs->transforms.data[i];
        RenderableComponent *enemyRenderable = &ecs->renderables.data[i];
        
        if(check_circle_collision(
            playerTransform->position, playerRenderable->radius, 
            enemyTransform->position, enemyRenderable->radius))
        {
            bool canDamage = true;
            if(currentTime - playerHealth->lastDamageTime < DAMAGE_COOLDOWN){
                canDamage = false;
            }

            if(canDamage){
                playerHealth->healthPoints -= ENEMY_DAMAGE;
                playerHealth->lastDamageTime = currentTime;
                printf("Player Health: %d \n", playerHealth->healthPoints);
            }
        }
    }
}