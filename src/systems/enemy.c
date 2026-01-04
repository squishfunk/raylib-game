#include "../ecs.h"
#include "../helpers.h"
#include <raymath.h>

void enemy_spawn_system(ECS *ecs){
    float currentTime = GetTime();
    static float lastSpawnTime = 0.0f;

    if(currentTime - lastSpawnTime > SPAWN_COOLDOWN){
        int enemyId = ecs_create_entity(ecs);
        if (enemyId >= 0){
            float radius = 20.0f;

            float randomX = (float)GetRandomValue(0 + radius, SCREEN_WIDTH - radius);
            float randomY = (float)GetRandomValue(0 + radius, SCREEN_HEIGHT - radius);

            ecs_add_tranform(ecs, enemyId, (Vector2){randomX, randomY});
            ecs_add_velocity(ecs, enemyId, (Vector2){0,0});
            ecs_add_renderable(ecs, enemyId, 20.0f, RED);
            ecs_add_health(ecs, enemyId, 100, 100);
            ecs->entities[enemyId].tags = TAG_ENEMY;
            lastSpawnTime = currentTime;
        }
    }
}

void enemy_movement_system(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if(!ecs->entities[i].active || !(ecs->entities[i].tags & TAG_ENEMY)) continue;

        int playerId = get_player_id(ecs); 
        
        Vector2 enemyPos = ecs->transforms.data[i].position;
        Vector2 playerPos = ecs->transforms.data[playerId].position;

        float speed = 200.0f;

        Vector2 direction = Vector2Subtract(playerPos, enemyPos);

        float length = Vector2Length(direction);
        if (length < 0.001f) {
            ecs->velocities.data[i].velocity = (Vector2){0, 0};
            continue;
        }

        Vector2 velocity = Vector2Scale(Vector2Normalize(direction), speed);


        ecs->velocities.data[i].velocity = velocity;
    }
}