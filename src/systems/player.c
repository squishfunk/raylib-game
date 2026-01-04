
#include "../ecs.h"
#include <raymath.h>

void player_input_system(ECS *ecs, int entityId){
    if (!ecs->velocities.active[entityId]) return;

    float deltaTime = GetFrameTime();
    float acceleration = 15.0f;
    float friction = 8.0f; 

    VelocityComponent *velocity = &ecs->velocities.data[entityId];
    Vector2 targetVelocity = {0, 0};


    if(IsKeyDown(KEY_W)) targetVelocity.y = -MOVEMENT_SPEED;
    if(IsKeyDown(KEY_S)) targetVelocity.y = MOVEMENT_SPEED;
    if(IsKeyDown(KEY_A)) targetVelocity.x = -MOVEMENT_SPEED;
    if(IsKeyDown(KEY_D)) targetVelocity.x = MOVEMENT_SPEED;

    Vector2 currentVel = velocity->velocity;

    if(targetVelocity.x != 0 || targetVelocity.y != 0){
        velocity->velocity.x = Lerp(currentVel.x, targetVelocity.x, acceleration * deltaTime);
        velocity->velocity.y = Lerp(currentVel.y, targetVelocity.y, acceleration * deltaTime);
    } else {
        velocity->velocity.x = Lerp(currentVel.x, 0.0f, friction * deltaTime);
        velocity->velocity.y = Lerp(currentVel.y, 0.0f, friction * deltaTime);
    }
}

void player_shooting_system(ECS *ecs, int playerId, float currentTime, float *lastShootTime){
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
