#include "EnemyFactory.hpp"
#include "../components/Components.hpp"
#include <cassert>
#include <raylib.h>
#include <cstdlib>
#include <ctime>

static const EnemyConfig ENEMY_CONFIGS[] = {
    // ENEMY_TYPE_NORMAL
    {
        .radius = 20.0f,
        .color = RED,
        .health = 3,
        .maxHealth = 3,
        .movementSpeed = 240.0f,
        .damage = 1,
        .attackCooldown = 1.0f,
        .ranged = false,
    },
    // ENEMY_TYPE_FAST
    {
        .radius = 18.0f,
        .color = ORANGE,
        .health = 2,
        .maxHealth = 2,
        .movementSpeed = 400.0f,
        .damage = 1,
        .attackCooldown = 0.8f,
        .ranged = false,
    },
    // ENEMY_TYPE_TANK
    {
        .radius = 25.0f,
        .color = DARKBROWN,
        .health = 8,
        .maxHealth = 8,
        .movementSpeed = 100.0f,
        .damage = 1,
        .attackCooldown = 1.5f,
        .ranged = false,
    },
    // ENEMY_TYPE_BOSS
    {
        .radius = 50.0f,
        .color = PURPLE,
        .health = 50,
        .maxHealth = 50,
        .movementSpeed = 240.0f,
        .damage = 1,
        .attackCooldown = 0.5f,
        .ranged = false,
    },
    // RANGED_NORMAL
    {
        .radius = 20.0f,
        .color = GREEN,
        .health = 3,
        .maxHealth = 3,
        .movementSpeed = 230.0f,
        .damage = 1,
        .attackCooldown = 1.0f,
        .ranged = true,
    },
    // RANGED_TANK
    {
        .radius = 30.0f,
        .color = BLUE,
        .health = 6,
        .maxHealth = 6,
        .movementSpeed = 230.0f,
        .damage = 1,
        .attackCooldown = 1.5f,
        .ranged = true,
    },
};

int EnemyFactory::create(ECS& ecs, const EnemySpawnData& data) {
    int enemyId = ecs.createEntity();
    if (enemyId < 0) return -1;
    
    int typeIndex = static_cast<int>(data.type);
    assert(typeIndex >= 0 && typeIndex < 6 && "Player transform not found!");

    const EnemyConfig* config = &ENEMY_CONFIGS[typeIndex];
    
    Vector2 position = data.position;
    float screenWidth = data.bounds.width;
    float screenHeight = data.bounds.height;
    
    position.x = (position.x + config->radius > screenWidth) ? screenWidth - config->radius : position.x;
    position.x = (position.x - config->radius < 0) ? config->radius : position.x;
    position.y = (position.y + config->radius > screenHeight) ? screenHeight - config->radius : position.y;
    position.y = (position.y - config->radius < 0) ? config->radius : position.y;
    
    ecs.addTransform(enemyId, position);
    ecs.addVelocity(enemyId, Vector2{0, 0}, config->movementSpeed);
    ecs.addRenderable(enemyId, config->radius, config->color);
    ecs.addCircleCollider(enemyId, config->radius, false);
    ecs.addHealth(enemyId, config->health, config->maxHealth);

    if(config->ranged){
        float shootingRange = 10; /*  TODO */
        float shootingSpeed = 800.0f; /*  TODO */

        ecs.addShootable(enemyId, shootingRange, shootingSpeed, config->attackCooldown);
    }
    
    float currentTime = GetTime();
    float randomDelay = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 4.0f));
    
    EnemyComponent enemyComp = {
        .type = data.type,
        .damage = config->damage,
        .attackCooldown = config->attackCooldown,
        .lastSoundTime = 0.0f,
        .nextSoundTime = currentTime + randomDelay,
        .spawnTime = currentTime,
        .spawnIdleDuration = 1.0f,
        .ranged = config->ranged,
    };
    
    ecs.addEnemy(enemyId, enemyComp);
    Sound idle = LoadSound("resources/sounds/enemy_idle.wav");
    Sound hit = LoadSound("resources/sounds/enemy_hit.wav");
    Sound die = LoadSound("resources/sounds/enemy_die.wav");
    ecs.addAudio(enemyId, "IDLE_SOUND", idle);
    ecs.addAudio(enemyId, "HIT_SOUND", hit);
    ecs.addAudio(enemyId, "DIE_SOUND", die);
    ecs.getEntities()[enemyId].tags = EntityTag::ENEMY;
    
    return enemyId;
}