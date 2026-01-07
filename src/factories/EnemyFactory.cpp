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
        .health = 100,
        .maxHealth = 100,
        .movementSpeed = 240.0f,
        .damage = 10,
        .attackCooldown = 1.0f
    },
    // ENEMY_TYPE_FAST
    {
        .radius = 18.0f,
        .color = ORANGE,
        .health = 80,
        .maxHealth = 80,
        .movementSpeed = 400.0f,
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
        .radius = 50.0f,
        .color = PURPLE,
        .health = 1000,
        .maxHealth = 1000,
        .movementSpeed = 240.0f,
        .damage = 80,
        .attackCooldown = 0.5f
    }
};

int EnemyFactory::create(ECS& ecs, const EnemySpawnData& data) {
    int enemyId = ecs.createEntity();
    if (enemyId < 0) return -1;
    
    int typeIndex = static_cast<int>(data.type);
    assert(typeIndex >= 0 && typeIndex < 4 && "Player transform not found!");

    const EnemyConfig* config = &ENEMY_CONFIGS[typeIndex];
    
    Vector2 position = data.position;
    float screenWidth = data.bounds.width;
    float screenHeight = data.bounds.height;
    
    // Fix position depending on radius
    position.x = (position.x + config->radius > screenWidth) ? screenWidth - config->radius : position.x;
    position.x = (position.x - config->radius < 0) ? config->radius : position.x;
    position.y = (position.y + config->radius > screenHeight) ? screenHeight - config->radius : position.y;
    position.y = (position.y - config->radius < 0) ? config->radius : position.y;
    
    ecs.addTransform(enemyId, position);
    ecs.addVelocity(enemyId, Vector2{0, 0}, config->movementSpeed);
    ecs.addRenderable(enemyId, config->radius, config->color);
    ecs.addCircleCollider(enemyId, config->radius, false);
    ecs.addHealth(enemyId, config->health, config->maxHealth);
    
    float currentTime = GetTime();
    float randomDelay = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 4.0f));
    
    EnemyComponent enemyComp = {
        .type = data.type,
        .damage = config->damage,
        .attackCooldown = config->attackCooldown,
        .lastAttackTime = 0.0f,
        .lastSoundTime = 0.0f,
        .nextSoundTime = currentTime + randomDelay
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