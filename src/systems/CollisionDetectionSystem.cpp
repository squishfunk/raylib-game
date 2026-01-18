#include "CollisionDetectionSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../events/EventBus.hpp"
#include "../events/Events.hpp"
#include "../utils/Helpers.hpp"
#include "CollisionResponseSystem.hpp"
#include <iostream>
#include <raylib.h>

static void checkCircleCollisions(ECS& ecs, EventBus& eventBus, int entityCount,
                                   const std::array<Entity, MAX_ENTITIES>& entities,
                                   const ComponentStorage<TransformComponent>& transforms,
                                   const ComponentStorage<CircleColliderComponent>& circleColliders,
                                   const ComponentStorage<BoxColliderComponent>& boxColliders) {
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if (!transforms.isActive(i)) continue;
        if (!circleColliders.isActive(i)) continue;
        
        const auto& transform1 = transforms.get(i);
        const auto& collider1 = circleColliders.get(i);
        EntityTag tag1 = entities[i].tags;
        
        for (int j = i + 1; j < entityCount; j++) {
            if (!entities[j].active) continue;
            if (!transforms.isActive(j)) continue;
            
            const auto& transform2 = transforms.get(j);
            EntityTag tag2 = entities[j].tags;
            
            // Circle vs Circle
            if (circleColliders.isActive(j)) {
                const auto& collider2 = circleColliders.get(j);
                
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
                        CollisionEvent event(i, j);
                        eventBus.emit(event);
                    }
                    
                    if (!collider1.isTrigger && !collider2.isTrigger) {
                        CollisionResponseSystem::handleCollision(ecs, i, j);
                    }
                }
            }
            // Circle vs Box
            else if (boxColliders.isActive(j)) {
                const auto& collider2 = boxColliders.get(j);
                

                if (Helpers::checkCircleRectCollision(
                    transform1.position, collider1.radius,
                    {transform2.position.x, transform2.position.y, collider2.width, collider2.height})) {
                    
                    // if trigger = emitEvent
                    if (collider1.isTrigger || collider2.isTrigger) {
                        CollisionEvent event(i, j);
                        eventBus.emit(event);
                    }
                    
                    if (!collider1.isTrigger && !collider2.isTrigger) {
                        CollisionResponseSystem::handleCollision(ecs, i, j);
                    }
                }
            }
        }
    }
}

static void checkBoxCollisions(ECS& ecs, EventBus& eventBus, int entityCount,
                               const std::array<Entity, MAX_ENTITIES>& entities,
                               const ComponentStorage<TransformComponent>& transforms,
                               const ComponentStorage<BoxColliderComponent>& boxColliders) {
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if (!transforms.isActive(i)) continue;
        if (!boxColliders.isActive(i)) continue;
        
        const auto& transform1 = transforms.get(i);
        const auto& collider1 = boxColliders.get(i);
        EntityTag tag1 = entities[i].tags;
        
        for (int j = i + 1; j < entityCount; j++) {
            if (!entities[j].active) continue;
            if (!transforms.isActive(j)) continue;
            if (!boxColliders.isActive(j)) continue; // Tylko Box vs Box
            
            const auto& transform2 = transforms.get(j);
            const auto& collider2 = boxColliders.get(j);
            EntityTag tag2 = entities[j].tags;
            
            // Box vs Box
            if (Helpers::checkRectRectCollision(
                transform1.position, collider1.width, collider1.height,
                transform2.position, collider2.width, collider2.height)) {
                
                bool isPlayerEnemyCollision = 
                    ((tag1 & EntityTag::PLAYER) == EntityTag::PLAYER && 
                     (tag2 & EntityTag::ENEMY) == EntityTag::ENEMY) ||
                    ((tag1 & EntityTag::ENEMY) == EntityTag::ENEMY && 
                     (tag2 & EntityTag::PLAYER) == EntityTag::PLAYER);
                
                // if trigger = emitEvent or enemy and player
                if (collider1.isTrigger || collider2.isTrigger || isPlayerEnemyCollision) {
                    CollisionEvent event(i, j);
                    eventBus.emit(event);
                }
                
                if (!collider1.isTrigger && !collider2.isTrigger) {
                    CollisionResponseSystem::handleCollision(ecs, i, j);
                }
            }
        }
    }
}

void CollisionDetectionSystem::update(ECS& ecs, EventBus& eventBus) {
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    const auto& transforms = ecs.getTransforms();
    const auto& circleColliders = ecs.getCircleColliders();
    const auto& boxColliders = ecs.getBoxColliders();
    
    checkCircleCollisions(ecs, eventBus, entityCount, entities, transforms, circleColliders, boxColliders);
    
    checkBoxCollisions(ecs, eventBus, entityCount, entities, transforms, boxColliders);
}