#include "bullet_system.hpp"
#include "../ecs/ecs.hpp"
#include "../components/components.hpp"
#include "../utils/helpers.hpp"
#include <raylib.h>
#include <cstdio>

void BulletSystem::update(ECS& ecs, int screenWidth, int screenHeight) {
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    const auto& transforms = ecs.getTransforms();
    const auto& renderables = ecs.getRenderables();
    auto& healths = ecs.getHealths();
    
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if ((entities[i].tags & EntityTag::BULLET) != EntityTag::BULLET) continue;
        if (!transforms.isActive(i) || !renderables.isActive(i)) continue;
        
        const auto& bulletTransform = transforms.get(i);
        const auto& bulletRenderable = renderables.get(i);
        
        Vector2 pos = bulletTransform.position;
        
        if (pos.x < 0 || pos.x > screenWidth || 
            pos.y < 0 || pos.y > screenHeight) {
        }
        
        for (int j = 0; j < entityCount; j++) {
            if (i == j) continue;
            if (!entities[j].active) continue;
            if ((entities[j].tags & EntityTag::ENEMY) != EntityTag::ENEMY) continue;
            if (!transforms.isActive(j) || !renderables.isActive(j) || !healths.isActive(j)) continue;
            
            const auto& enemyTransform = transforms.get(j);
            const auto& enemyRenderable = renderables.get(j);
            auto& enemyHealth = healths.get(j);
            
            if (Helpers::checkCircleCollision(pos, bulletRenderable.radius, 
                                              enemyTransform.position, enemyRenderable.radius)) {
                float currentTime = GetTime();
                
                enemyHealth.healthPoints -= BULLET_DAMAGE;
                enemyHealth.lastDamageTime = currentTime;
                ecs.getEntities()[i].active = false;
                printf("Enemy id: %d Hit. Current health: %d \n", j, enemyHealth.healthPoints);
            }
        }
    }
}

