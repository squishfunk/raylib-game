#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include "../ecs.h"
#include "../helpers.h"

void bullet_system(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if (!ecs->entities[i].active) continue;
        if (!(ecs->entities[i].tags & TAG_BULLET)) continue;
        if (!ecs->transforms.active[i] || !ecs->renderables.active[i]) continue;

        TransformComponent *bulletTransform = &ecs->transforms.data[i];
        RenderableComponent *bulletRenderable = &ecs->renderables.data[i];
        
        Vector2 pos = bulletTransform->position;

        if (pos.x < 0 || pos.x > SCREEN_WIDTH || 
            pos.y < 0 || pos.y > SCREEN_HEIGHT) {
        }

        for (int j = 0; j < ecs->entityCount; j++){
            if (i == j) continue;
            if (!ecs->entities[j].active) continue;
            if (!(ecs->entities[j].tags & TAG_ENEMY)) continue;
            if (!ecs->transforms.active[j] || !ecs->renderables.active[j] || !ecs->healths.active[j]) continue;

            TransformComponent *enemyTransform = &ecs->transforms.data[j];
            RenderableComponent *enemyRenderable = &ecs->renderables.data[j];
            HealthComponent *enemyHealth = &ecs->healths.data[j];
            
            if(check_circle_collision(pos, bulletRenderable->radius, 
                                      enemyTransform->position, enemyRenderable->radius)){
                float currentTime = GetTime();

                enemyHealth->healthPoints -= BULLET_DAMAGE;
                enemyHealth->lastDamageTime = currentTime;
                ecs->entities[i].active = false;
                printf("Enemy id: %d Hit. Current health: %d \n", j, enemyHealth->healthPoints);
            }
        }
    }
}