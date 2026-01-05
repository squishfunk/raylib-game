#include "enemy_system.hpp"
#include "../ecs/ecs.hpp"
#include "../components/components.hpp"
#include "../utils/helpers.hpp"
#include <raymath.h>

const EnemyConfig EnemySystem::ENEMY_CONFIGS[] = {
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

int EnemySystem::createEnemy(ECS& ecs, Vector2 position, EnemyType type, int screenWidth, int screenHeight) {
    int enemyId = ecs.createEntity();
    if (enemyId < 0) return -1;
    
    int typeIndex = static_cast<int>(type);
    const EnemyConfig* config = &ENEMY_CONFIGS[typeIndex];
    
    // Fix position depending on radius
    position.x = (position.x + config->radius > screenWidth) ? screenWidth - config->radius : position.x;
    position.x = (position.x - config->radius < 0) ? config->radius : position.x;
    position.y = (position.y + config->radius > screenHeight) ? screenHeight - config->radius : position.y;
    position.y = (position.y - config->radius < 0) ? config->radius : position.y;
    
    ecs.addTransform(enemyId, position);
    ecs.addVelocity(enemyId, Vector2{0, 0});
    ecs.addRenderable(enemyId, config->radius, config->color);
    ecs.addHealth(enemyId, config->health, config->maxHealth);
    
    EnemyComponent enemyComp = {
        .type = type,
        .movementSpeed = config->movementSpeed,
        .damage = config->damage,
        .attackCooldown = config->attackCooldown,
        .lastAttackTime = 0.0f
    };
    
    ecs.addEnemy(enemyId, enemyComp);
    ecs.getEntities()[enemyId].tags = EntityTag::ENEMY;
    
    return enemyId;
}

void EnemySystem::generateRoomEnemies(ECS& ecs, int screenWidth, int screenHeight) {
    int enemiesCount = 3;
    
    for (int i = 0; i < enemiesCount; i++) {
        EnemyType randomEnemyType = static_cast<EnemyType>(GetRandomValue(0, 3));
        
        float randomX = static_cast<float>(GetRandomValue(0, screenWidth));
        float randomY = static_cast<float>(GetRandomValue(0, screenHeight));
        
        createEnemy(ecs, Vector2{randomX, randomY}, randomEnemyType, screenWidth, screenHeight);
    }
}
void EnemySystem::updateMovement(ECS& ecs) {
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    auto& transforms = ecs.getTransforms();
    auto& velocities = ecs.getVelocities();
    
    int playerId = Helpers::getPlayerId(ecs);
    if (playerId < 0) return;
    
    const auto& playerTransform = transforms.get(playerId);
    
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if ((entities[i].tags & EntityTag::ENEMY) != EntityTag::ENEMY) continue;
        
        Vector2 enemyPos = transforms.get(i).position;
        Vector2 playerPos = playerTransform.position;
        
        float speed = 200.0f;
        
        Vector2 direction = Vector2Subtract(playerPos, enemyPos);
        
        float length = Vector2Length(direction);
        if (length < 0.001f) {
            velocities.get(i).velocity = Vector2{0, 0};
            continue;
        }
        
        Vector2 velocity = Vector2Scale(Vector2Normalize(direction), speed);
        velocities.get(i).velocity = velocity;
    }
}

