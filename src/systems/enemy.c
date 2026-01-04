#include "../ecs.h"
#include "../helpers.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

static const EnemyConfig ENEMY_CONFIGS[] = {
    // ENEMY_TYPE_NORMAL
    {
        .radius = 20.0f,
        .color = RED,
        .health = 100,
        .maxHealth = 100,
        .movementSpeed = 200.0f,
        .damage = 10,
        .attackCooldown = 1.0f
    },
    // ENEMY_TYPE_FAST
    {
        .radius = 18.0f,
        .color = ORANGE,
        .health = 80,
        .maxHealth = 80,
        .movementSpeed = 300.0f,
        .damage = 8,
        .attackCooldown = 0.8f
    },
    // ENEMY_TYPE_TANK
    {
        .radius = 25.0f,
        .color = DARKBROWN,
        .health = 250,
        .maxHealth = 250,
        .movementSpeed = 100.0f,
        .damage = 15,
        .attackCooldown = 1.5f
    },
    // ENEMY_TYPE_BOSS
    {
        .radius = 30.0f,
        .color = PURPLE,
        .health = 500,
        .maxHealth = 500,
        .movementSpeed = 150.0f,
        .damage = 25,
        .attackCooldown = 0.5f
    }
};

int enemy_create(ECS *ecs, Vector2 position, EnemyType type){
    int enemyId = ecs_create_entity(ecs);
    if(enemyId < 0) return -1;
    
    
    const EnemyConfig *config = &ENEMY_CONFIGS[type];

    /* FIX POSITION DEPENDING ON RADIUS */
    position.x = (position.x + config->radius > SCREEN_WIDTH) ? SCREEN_WIDTH - config->radius : position.x;
    position.x = (position.x - config->radius < 0) ? config->radius : position.x;
    position.y = (position.y + config->radius > SCREEN_HEIGHT) ? SCREEN_HEIGHT - config->radius : position.y;
    position.y = (position.y - config->radius < 0) ? config->radius : position.y;
    
    ecs_add_tranform(ecs, enemyId, position);
    ecs_add_velocity(ecs, enemyId, (Vector2){0, 0});
    ecs_add_renderable(ecs, enemyId, config->radius, config->color);
    ecs_add_health(ecs, enemyId, config->health, config->maxHealth);

    EnemyComponent enemyComp = {
        .type = type,
        .movementSpeed = config->movementSpeed,
        .damage = config->damage,
        .attackCooldown = config->attackCooldown,
        .lastAttackTime = 0.0f
    };

    ecs_add_enemy(ecs, enemyId, &enemyComp);

    ecs->entities[enemyId].tags = TAG_ENEMY;

    return enemyId;
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