#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdio.h>
#include "ecs.h"


bool _check_circle_collision(Vector2 pos1, float radius1, Vector2 pos2, float radius2){
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    float distance = sqrtf(dx*dx + dy*dy);
    float minDistance = radius1 + radius2;
    return distance < minDistance;
}

int _get_player_id(ECS *ecs){

    int playerId = -1;
    for(int i = 0; i < ecs->entityCount; i++){
        if(ecs->entities[i].active && (ecs->entities[i].tags & TAG_PLAYER)){
            playerId = i;
            break;
        }
    }
    return playerId;
}

void movement_system(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if (!ecs->entities[i].active) continue;
        if (!ecs->transforms.active[i] || !ecs->velocities.active[i]) continue;

        ecs->transforms.data[i].position.x += ecs->velocities.data[i].velocity.x;
        ecs->transforms.data[i].position.y += ecs->velocities.data[i].velocity.y;
    }
}

void render_system(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if (!ecs->entities[i].active) continue;
        if (!ecs->transforms.active[i] || !ecs->renderables.active[i]) continue;

        TransformComponent *transform = &ecs->transforms.data[i];
        RenderableComponent *renderable = &ecs->renderables.data[i];

        DrawCircleV(transform->position, renderable->radius, renderable->color);

        if ((ecs->entities[i].tags & TAG_ENEMY || ecs->entities[i].tags & TAG_PLAYER) && ecs->healths.active[i]){
            HealthComponent *health = &ecs->healths.data[i];
            
            int width = (int)(renderable->radius * 2.5);
            int height = SCREEN_WIDTH / 100;

            int x = (int)(transform->position.x - width / 2.0f);
            int y = (int)(transform->position.y + renderable->radius * 1.5);

            DrawRectangle(x, y, width, height, DARKGRAY);
            DrawRectangle(x, y, (int)(width * health->healthPoints / health->maxHealthPoints), height, RED);
            DrawRectangleLines(x, y, width, height, BLACK);
        }
    }
}

void player_input_system(ECS *ecs, int entityId){
    if (!ecs->velocities.active[entityId]) return;
    
    Vector2 velocity = {0,0};

    if(IsKeyDown(KEY_W)) velocity.y = -MOVEMENT_SPEED;
    if(IsKeyDown(KEY_S)) velocity.y = MOVEMENT_SPEED;
    if(IsKeyDown(KEY_A)) velocity.x = -MOVEMENT_SPEED;
    if(IsKeyDown(KEY_D)) velocity.x = MOVEMENT_SPEED;

    ecs->velocities.data[entityId].velocity = velocity;
}


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
            
            if(_check_circle_collision(pos, bulletRenderable->radius, 
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

void shooting_system(ECS *ecs, int playerId, float currentTime, float *lastShootTime){
    bullet_system(ecs);
    if (!(IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || 
          IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))) return;

    if (currentTime - *lastShootTime < SHOOT_COOLDOWN) return;
    if (!ecs->transforms.active[playerId]) return;

    TransformComponent *playerTransform = &ecs->transforms.data[playerId];
    Vector2 position = playerTransform->position;
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

    int playerId = _get_player_id(ecs);

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
        
        if(_check_circle_collision(
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

void enemy_movement_system(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if(!ecs->entities[i].active || !(ecs->entities[i].tags & TAG_ENEMY)) continue;

        int playerId = _get_player_id(ecs); 
        
        Vector2 enemyPos = ecs->transforms.data[i].position;
        Vector2 playerPos = ecs->transforms.data[playerId].position;

        printf("Player pos: X: %f Y: %f", playerPos.x, playerPos.y);
        printf("Enemy pos: X: %f Y: %f", enemyPos.x, enemyPos.y);

        float speed = 1.0f;

        Vector2 direction = Vector2Subtract(playerPos, enemyPos);

        float length = Vector2Length(direction);
        if (length < 0.001f) {
            ecs->velocities.data[i].velocity = (Vector2){0, 0};
            continue;
        }

        Vector2 velocity = Vector2Scale(Vector2Normalize(direction), speed);

        printf("Direction pos: X: %f Y: %f", enemyPos.x, enemyPos.y);


        ecs->velocities.data[i].velocity = velocity;
    }
}