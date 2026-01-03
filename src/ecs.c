#include <raylib.h>
#include <math.h>
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

int ecs_add_health(ECS *ecs, int entityId, int initialHealthPoints, int maxHealthPoints){
    ecs->entities[entityId].health.healthPoints = initialHealthPoints;
    ecs->entities[entityId].health.maxHealthPoints = maxHealthPoints;
    ecs->entities[entityId].health.lastDamageTime = 0.0f;
    ecs->entities[entityId].hasHealth = true;
    return 0;
}

int ecs_add_damage_cooldown(ECS *ecs, int entityId){
    ecs->entities[entityId].damageCooldown.lastDamageTime = 0.0f;
    ecs->entities[entityId].hasDamageCooldown = true;
    return 0;
}





bool _check_circle_collision(Vector2 pos1, float radius1, Vector2 pos2, float radius2){
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    float distance = sqrtf(dx*dx + dy*dy);
    float minDistance = radius1 + radius2;
    return distance < minDistance;
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

        if (ecs->entities[i].tags & TAG_ENEMY || ecs->entities[i].tags & TAG_PLAYER){
            
            Entity *enemy = &ecs->entities[i];
            Vector2 position = enemy->transform.position;

            int width = enemy->renderable.radius * 2.5;
            int height = SCREEN_WIDTH / 100;

            int x = position.x - width / 2;
            int y = position.y + enemy->renderable.radius * 1.5;


            DrawRectangle(x, y, width, height, DARKGRAY);

            DrawRectangle(x, y, (int)(width * enemy->health.healthPoints / enemy->health.maxHealthPoints), height, RED);

            DrawRectangleLines(x, y, width, height, BLACK);
        }
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

void bullet_system(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if (!ecs->entities[i].active || !ecs->entities[i].hasTransform) continue;
        if (!(ecs->entities[i].tags & TAG_BULLET)) continue;

        Entity *bullet = &ecs->entities[i];
        
        Vector2 pos = bullet->transform.position;

        if (pos.x < 0 || pos.x > SCREEN_WIDTH || 
            pos.y < 0 || pos.y > SCREEN_HEIGHT) {
            
        }

        for (int j = 0; j < ecs->entityCount; j++){
            if (i == j) continue;
            if (!(ecs->entities[j].tags & TAG_ENEMY)) continue;

            Entity *enemy = &ecs->entities[j];
            if(_check_circle_collision(pos, bullet->renderable.radius, enemy->transform.position, enemy->renderable.radius)){
                bool canDamage = true;
                float currentTime = GetTime();

                enemy->health.healthPoints -= BULLET_DAMAGE;
                enemy->health.lastDamageTime = currentTime;
                ecs->entities[i].active = false;
                printf("Enemy id: %d Hit. Current health: %d \n", j, enemy->health.healthPoints);
            }
        }
    }
}

void shooting_system(ECS *ecs, int playerId, float currentTime, float *lastShootTime){
    bullet_system(ecs);
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

            ecs_add_tranform(ecs, enemyId, (Vector2){randomX, randomY});
            ecs_add_velocity(ecs, enemyId, (Vector2){0,0});
            ecs_add_renderable(ecs, enemyId, 20.0f, RED);
            ecs_add_health(ecs, enemyId, 100, 100);
            ecs->entities[enemyId].tags = TAG_ENEMY;
            lastSpawnTime = currentTime;
        }
    }
}

void collision_system(ECS *ecs){
    float currentTime = GetTime();

    int playerId = -1;
    for(int i = 0; i < ecs->entityCount; i++){
        if(ecs->entities[i].active && (ecs->entities[i].tags & TAG_PLAYER)){
            playerId = i;
            break;
        }
    }

    if(playerId < 0 || !ecs->entities[playerId].hasHealth) return;

    Entity *player = &ecs->entities[playerId];

    for(int i = 0; i < ecs->entityCount; i++){
        if(i == playerId) continue;
        if(!ecs->entities[i].active) continue;
        if(!(ecs->entities[i].tags & TAG_ENEMY)) continue;
        if(!ecs->entities[i].hasTransform || !ecs->entities[i].hasRenderable) continue;

        Entity *enemy = &ecs->entities[i];
        
        if(_check_circle_collision(
        player->transform.position, player->renderable.radius, 
        enemy->transform.position, enemy->renderable.radius))
        {
            bool canDamage = true;
            if(currentTime - player->health.lastDamageTime < DAMAGE_COOLDOWN){
                canDamage = false;
            }

            if(canDamage){
                player->health.healthPoints -= ENEMY_DAMAGE;
                player->health.lastDamageTime = currentTime;
                printf("Player Health: %d \n", player->health.healthPoints);
            }
        }

    }
}

void health_system(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if(!ecs->entities[i].active || !ecs->entities[i].hasHealth) continue;
        
        if(ecs->entities[i].health.healthPoints <= 0){
            ecs->entities[i].active = false;
            printf("Entity %d died!\n", i);
        }
    }
}