#include "enemy_system.hpp"
#include "../ecs/ecs.hpp"
#include "../components/components.hpp"
#include "../utils/helpers.hpp"
#include <raymath.h>

void EnemySystem::update(ECS& ecs) {
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

