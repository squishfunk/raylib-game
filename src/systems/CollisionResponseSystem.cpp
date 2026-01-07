#include "CollisionResponseSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../utils/Helpers.hpp"
#include <raymath.h>
#include <raylib.h>
#include <cmath>

void CollisionResponseSystem::handleCollision(ECS& ecs, int entityId1, int entityId2) {
    if (!ecs.getTransforms().isActive(entityId1) || !ecs.getTransforms().isActive(entityId2)) {
        return;
    }
    
    if (!ecs.getCircleColliders().isActive(entityId1) || !ecs.getCircleColliders().isActive(entityId2)) {
        return;
    }
    
    auto& transform1 = ecs.getTransforms().get(entityId1);
    auto& transform2 = ecs.getTransforms().get(entityId2);
    const auto& collider1 = ecs.getCircleColliders().get(entityId1);
    const auto& collider2 = ecs.getCircleColliders().get(entityId2);
    
    // direction of collision
    Vector2 direction = Vector2Subtract(transform1.position, transform2.position);
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance < 0.001f) {
        // unsnap a entity
        direction = {1.0f, 0.0f};
        distance = 1.0f;
    }
    
    // normalise the vector
    direction.x /= distance;
    direction.y /= distance;
    
    float offset = 5.0f;
    float minDistance = collider1.radius + collider2.radius - offset;
    float penetration = minDistance - distance;
    
    if (penetration > 0.0f) {
        float moveAmount = penetration * 0.5f;
        
        transform1.position.x += direction.x * moveAmount;
        transform1.position.y += direction.y * moveAmount;
        
        transform2.position.x -= direction.x * moveAmount;
        transform2.position.y -= direction.y * moveAmount;
    }
}

