#include "collision_system.hpp"
#include "../ecs/ecs.hpp"
#include "../components/components.hpp"
#include "../utils/helpers.hpp"
#include <raylib.h>
#include <cstdio>

void CollisionSystem::update(ECS& ecs) {
    float currentTime = GetTime();
    
    int playerId = Helpers::getPlayerId(ecs);
    
    if (playerId < 0 || !ecs.getHealths().isActive(playerId)) return;
    if (!ecs.getTransforms().isActive(playerId) || !ecs.getRenderables().isActive(playerId)) return;
    
    const auto& playerTransform = ecs.getTransforms().get(playerId);
    const auto& playerRenderable = ecs.getRenderables().get(playerId);
    auto& playerHealth = ecs.getHealths().get(playerId);
    
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    const auto& transforms = ecs.getTransforms();
    const auto& renderables = ecs.getRenderables();
    
    for (int i = 0; i < entityCount; i++) {
        if (i == playerId) continue;
        if (!entities[i].active) continue;
        if ((entities[i].tags & EntityTag::ENEMY) != EntityTag::ENEMY) continue;
        if (!transforms.isActive(i) || !renderables.isActive(i)) continue;
        
        const auto& enemyTransform = transforms.get(i);
        const auto& enemyRenderable = renderables.get(i);
        
        if (Helpers::checkCircleCollision(
            playerTransform.position, playerRenderable.radius, 
            enemyTransform.position, enemyRenderable.radius)) {
            
            bool canDamage = true;
            if (currentTime - playerHealth.lastDamageTime < DAMAGE_COOLDOWN) {
                canDamage = false;
            }
            
            if (canDamage) {
                playerHealth.healthPoints -= ENEMY_DAMAGE;
                playerHealth.lastDamageTime = currentTime;
                printf("Player Health: %d \n", playerHealth.healthPoints);
            }
        }
    }
}

