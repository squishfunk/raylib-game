#include "CollisionDetectionSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../events/EventBus.hpp"
#include "../events/Events.hpp"
#include "../utils/Helpers.hpp"
#include "CollisionResponseSystem.hpp"
#include <iostream>
#include <raylib.h>

static void checkAllCollisions(ECS& ecs, EventBus& eventBus) {
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    const auto& transforms = ecs.getTransforms();
    const auto& circleColliders = ecs.getCircleColliders();
    const auto& boxColliders = ecs.getBoxColliders();
    
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if (!transforms.isActive(i)) continue;
        
        const auto& transform1 = transforms.get(i);
        EntityTag tag1 = entities[i].tags;
        
        bool hasCircle1 = circleColliders.isActive(i);
        bool hasBox1 = boxColliders.isActive(i);
        
        if (!hasCircle1 && !hasBox1) continue;
        
        for (int j = i + 1; j < entityCount; j++) {
            if (!entities[j].active) continue;
            if (!transforms.isActive(j)) continue;
            
            const auto& transform2 = transforms.get(j);
            EntityTag tag2 = entities[j].tags;
            
            bool hasCircle2 = circleColliders.isActive(j);
            bool hasBox2 = boxColliders.isActive(j);
            
            if (!hasCircle2 && !hasBox2) continue;
            
            // Circle vs Circle
            if (hasCircle1 && hasCircle2) {
                const auto& collider1 = circleColliders.get(i);
                const auto& collider2 = circleColliders.get(j);
                
                if (Helpers::checkCircleCollision(
                    transform1.position, collider1.radius,
                    transform2.position, collider2.radius)) {
                    
                    bool isPlayerEnemyCollision = 
                        ((tag1 & EntityTag::PLAYER) == EntityTag::PLAYER && 
                         (tag2 & EntityTag::ENEMY) == EntityTag::ENEMY) ||
                        ((tag1 & EntityTag::ENEMY) == EntityTag::ENEMY && 
                         (tag2 & EntityTag::PLAYER) == EntityTag::PLAYER);
                    
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
            else if (hasCircle1 && hasBox2) {
                const auto& circleCollider = circleColliders.get(i);
                const auto& boxCollider = boxColliders.get(j);
                
                Rectangle rect = {transform2.position.x, transform2.position.y, 
                                 boxCollider.width, boxCollider.height};
                
                if (Helpers::checkCircleRectCollision(
                    transform1.position, circleCollider.radius,
                    rect, 0.0f)) {
                    
                    if (circleCollider.isTrigger || boxCollider.isTrigger) {
                        CollisionEvent event(i, j);
                        eventBus.emit(event);
                    }
                    
                    if (!circleCollider.isTrigger && !boxCollider.isTrigger) {
                        CollisionResponseSystem::handleCollision(ecs, i, j);
                    }
                }
            }
            // Box vs Circle 
            else if (hasBox1 && hasCircle2) {
                const auto& boxCollider = boxColliders.get(i);
                const auto& circleCollider = circleColliders.get(j);
                
                Rectangle rect = {transform1.position.x, transform1.position.y, 
                                 boxCollider.width, boxCollider.height};
                
                if (Helpers::checkCircleRectCollision(
                    transform2.position, circleCollider.radius,
                    rect, 0.0f)) {
                    
                    if (boxCollider.isTrigger || circleCollider.isTrigger) {
                        CollisionEvent event(i, j);
                        eventBus.emit(event);
                    }
                    
                    if (!boxCollider.isTrigger && !circleCollider.isTrigger) {
                        CollisionResponseSystem::handleCollision(ecs, i, j);
                    }
                }
            }
            // Box vs Box
            else if (hasBox1 && hasBox2) {
                const auto& collider1 = boxColliders.get(i);
                const auto& collider2 = boxColliders.get(j);
                
                if (Helpers::checkRectRectCollision(
                    transform1.position, collider1.width, collider1.height,
                    transform2.position, collider2.width, collider2.height)) {
                    
                    bool isPlayerEnemyCollision = 
                        ((tag1 & EntityTag::PLAYER) == EntityTag::PLAYER && 
                         (tag2 & EntityTag::ENEMY) == EntityTag::ENEMY) ||
                        ((tag1 & EntityTag::ENEMY) == EntityTag::ENEMY && 
                         (tag2 & EntityTag::PLAYER) == EntityTag::PLAYER);
                    
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
}

void CollisionDetectionSystem::update(ECS& ecs, EventBus& eventBus) {
    checkAllCollisions(ecs, eventBus);
}