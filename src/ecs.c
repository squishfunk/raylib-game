#include <raylib.h>
#include <stdio.h>
#include "ecs.h"

int ecs_create_entity(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if(!ecs->entities[i].active){
            ecs->entities[i] = (Entity){0};
            ecs->entities[i].active = true;
            return i;
        }
    }

    if(ecs->entityCount >= MAX_ENTITIES){
        return -1;
    }

    int entityId = ecs->entityCount++;
    ecs->entities[entityId] = (Entity){0};
    ecs->entities[entityId].active = true;
    return entityId;
}

int ecs_add_tranform(ECS *ecs, int entityId, Vector2 position){
    ecs->entities[entityId].transform = (TransformComponent){position};
    ecs->entities[entityId].hasTransform = true;
    return 0;
}

int ecs_add_velocity(ECS *ecs, int entityId, Vector2 velocity){
    ecs->entities[entityId].velocity.velocity = velocity;
    ecs->entities[entityId].hasVelocity = true;
    return 0;
}

int ecs_add_renderable(ECS *ecs, int entityId, float radius, Color color){
    ecs->entities[entityId].renderable.radius = radius;
    ecs->entities[entityId].renderable.color = color;
    ecs->entities[entityId].hasRenderable = true;
    return 0;
}

void movement_system(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if (!ecs->entities[i].active || !ecs->entities[i].hasTransform || !ecs->entities[i].hasVelocity) continue;
        
        ecs->entities[i].transform.position.x += ecs->entities[i].velocity.velocity.x;
        ecs->entities[i].transform.position.y += ecs->entities[i].velocity.velocity.y;
    }
}

void render_system(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if (!ecs->entities[i].active || !ecs->entities[i].hasRenderable) continue;

        DrawCircleV(
            ecs->entities[i].transform.position,
            ecs->entities[i].renderable.radius,
            ecs->entities[i].renderable.color
        );
    }
}

void player_input_system(ECS *ecs, int entityId){
    Vector2 velocity = {0,0};

    if(IsKeyDown(KEY_W)) velocity.y = -MOVEMENT_SPEED;
    if(IsKeyDown(KEY_S)) velocity.y = MOVEMENT_SPEED;
    if(IsKeyDown(KEY_A)) velocity.x = -MOVEMENT_SPEED;
    if(IsKeyDown(KEY_D)) velocity.x = MOVEMENT_SPEED;

    ecs->entities[entityId].velocity.velocity = velocity;
}

void bullet_cleanup_system(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if (!ecs->entities[i].active || !ecs->entities[i].hasTransform) continue;
        if (!(ecs->entities[i].tags & TAG_BULLET)) continue;
        
        Vector2 pos = ecs->entities[i].transform.position;
        if (pos.x < 0 || pos.x > SCREEN_WIDTH || 
            pos.y < 0 || pos.y > SCREEN_HEIGHT) {
            printf("Clear bullet \n");
            ecs->entities[i].active = false;
        }
    }
}

void shooting_system(ECS *ecs, int playerId, float currentTime, float *lastShootTime){
    bullet_cleanup_system(ecs);
    if (!(IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || 
          IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))) return;

    if (currentTime - *lastShootTime < SHOOT_COOLDOWN) return;
    if (!ecs->entities[playerId].hasTransform) return;


    Vector2 position = ecs->entities[playerId].transform.position;
    Vector2 velocity = {0,0};

    if(IsKeyDown(KEY_UP)) velocity.y = -BULLET_SPEED;
    if(IsKeyDown(KEY_DOWN)) velocity.y = BULLET_SPEED;
    if(IsKeyDown(KEY_LEFT)) velocity.x = -BULLET_SPEED;
    if(IsKeyDown(KEY_RIGHT)) velocity.x = BULLET_SPEED;

    int bulletId = ecs_create_entity(ecs);
    if(bulletId >= 0){
        ecs_add_tranform(ecs, bulletId, position);
        ecs_add_velocity(ecs, bulletId, velocity);
        ecs_add_renderable(ecs, bulletId, 5.0f, ORANGE);
        ecs->entities[bulletId].tags = TAG_BULLET;
    }


    *lastShootTime = currentTime;
}

void enemy_spawn_system(ECS *ecs){

    float currentTime = GetTime();
    static float lastSpawnTime = 0.0f;

    if(currentTime - lastSpawnTime > SPAWN_COOLDOWN){
        int enemyId = ecs_create_entity(ecs);
        if (enemyId >= 0){
            float radius = 20.0f;

            float randomX = (float)GetRandomValue(0 + radius, SCREEN_WIDTH - radius);
            float randomY = (float)GetRandomValue(0 + radius, SCREEN_HEIGHT - radius);

            printf("%d %d \n", 0 + (int)radius, SCREEN_WIDTH - (int)radius);

            ecs_add_tranform(ecs, enemyId, (Vector2){randomX, randomY});
            ecs_add_velocity(ecs, enemyId, (Vector2){0,0});
            ecs_add_renderable(ecs, enemyId, 20.0f, RED);
            lastSpawnTime = currentTime;
        }
    }
    
}