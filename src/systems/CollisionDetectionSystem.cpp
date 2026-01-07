#include "CollisionDetectionSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../events/EventBus.hpp"
#include "../events/Events.hpp"
#include "../utils/Helpers.hpp"
#include "CollisionResponseSystem.hpp"
#include <raylib.h>

void CollisionDetectionSystem::update(ECS& ecs, EventBus& eventBus) {
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    const auto& transforms = ecs.getTransforms();
    const auto& colliders = ecs.getCircleColliders();
    
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if (!transforms.isActive(i) || !colliders.isActive(i)) continue;
        
        const auto& transform1 = transforms.get(i);
        const auto& collider1 = colliders.get(i);
        EntityTag tag1 = entities[i].tags;
        
        for (int j = i + 1; j < entityCount; j++) {
            if (!entities[j].active) continue;
            if (!transforms.isActive(j) || !colliders.isActive(j)) continue;
            
            const auto& transform2 = transforms.get(j);
            const auto& collider2 = colliders.get(j);
            EntityTag tag2 = entities[j].tags;
            
            if (Helpers::checkCircleCollision(
                transform1.position, collider1.radius,
                transform2.position, collider2.radius)) {
                
                bool isPlayerEnemyCollision = 
                    ((tag1 & EntityTag::PLAYER) == EntityTag::PLAYER && 
                     (tag2 & EntityTag::ENEMY) == EntityTag::ENEMY) ||
                    ((tag1 & EntityTag::ENEMY) == EntityTag::ENEMY && 
                     (tag2 & EntityTag::PLAYER) == EntityTag::PLAYER);
                
                // if trigger = emitEvent or enemy and player
                if (collider1.isTrigger || collider2.isTrigger || isPlayerEnemyCollision) {
                    CircleCollisionEvent event(i, j);
                    eventBus.emit(event);
                }
                
                if (!collider1.isTrigger && !collider2.isTrigger) {
                    CollisionResponseSystem::handleCollision(ecs, i, j);
                }
            }
        }
    }
}

