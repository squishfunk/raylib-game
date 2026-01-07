#include "BulletSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include <raylib.h>

void BulletSystem::update(ECS& ecs, int screenWidth, int screenHeight) {
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    const auto& transforms = ecs.getTransforms();
    
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if ((entities[i].tags & EntityTag::BULLET) != EntityTag::BULLET) continue;
        if (!transforms.isActive(i)) continue;
        
        const auto& bulletTransform = transforms.get(i);
        Vector2 pos = bulletTransform.position;
        
        if (pos.x < 0 || pos.x > screenWidth || 
            pos.y < 0 || pos.y > screenHeight) {
            ecs.getEntities()[i].active = false;
        }
    }
}

