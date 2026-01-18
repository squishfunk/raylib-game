#include "CollisionResponseSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../utils/Helpers.hpp"
#include <raymath.h>
#include <raylib.h>
#include <cmath>
#include <algorithm>

void CollisionResponseSystem::handleCollision(ECS& ecs, int entityId1, int entityId2) {
    if (!ecs.getTransforms().isActive(entityId1) || !ecs.getTransforms().isActive(entityId2)) {
        return;
    }
    
    auto& transform1 = ecs.getTransforms().get(entityId1);
    auto& transform2 = ecs.getTransforms().get(entityId2);
    
    bool hasCircle1 = ecs.getCircleColliders().isActive(entityId1);
    bool hasBox1 = ecs.getBoxColliders().isActive(entityId1);
    bool hasCircle2 = ecs.getCircleColliders().isActive(entityId2);
    bool hasBox2 = ecs.getBoxColliders().isActive(entityId2);
    
    // Circle vs Circle
    if (hasCircle1 && hasCircle2) {
        const auto& collider1 = ecs.getCircleColliders().get(entityId1);
        const auto& collider2 = ecs.getCircleColliders().get(entityId2);
        
        Vector2 direction = Vector2Subtract(transform1.position, transform2.position);
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (distance < 0.001f) {
            direction = {1.0f, 0.0f};
            distance = 1.0f;
        }
        
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
    // Circle vs Box
    else if ((hasCircle1 && hasBox2) || (hasBox1 && hasCircle2)) {
        int circleEntityId, boxEntityId;
        Vector2* circlePos;
        float circleRadius;
        Vector2 boxPos;
        float boxWidth, boxHeight;
        
        if (hasCircle1 && hasBox2) {
            circleEntityId = entityId1;
            boxEntityId = entityId2;
            circlePos = &transform1.position;
            circleRadius = ecs.getCircleColliders().get(entityId1).radius;
            boxPos = transform2.position;
            boxWidth = ecs.getBoxColliders().get(entityId2).width;
            boxHeight = ecs.getBoxColliders().get(entityId2).height;
        } else {
            circleEntityId = entityId2;
            boxEntityId = entityId1;
            circlePos = &transform2.position;
            circleRadius = ecs.getCircleColliders().get(entityId2).radius;
            boxPos = transform1.position;
            boxWidth = ecs.getBoxColliders().get(entityId1).width;
            boxHeight = ecs.getBoxColliders().get(entityId1).height;
        }
        
        float boxLeft = boxPos.x;
        float boxRight = boxPos.x + boxWidth;
        float boxTop = boxPos.y;
        float boxBottom = boxPos.y + boxHeight;
        
        float closestX = std::max(boxLeft, std::min(circlePos->x, boxRight));
        float closestY = std::max(boxTop, std::min(circlePos->y, boxBottom));
        
        Vector2 direction = {circlePos->x - closestX, circlePos->y - closestY};
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (distance < 0.001f) {
            float distToLeft = circlePos->x - boxLeft;
            float distToRight = boxRight - circlePos->x;
            float distToTop = circlePos->y - boxTop;
            float distToBottom = boxBottom - circlePos->y;
            
            float minDist = std::min({distToLeft, distToRight, distToTop, distToBottom});
            
            if (minDist == distToLeft) {
                direction = {-1.0f, 0.0f};
                distance = distToLeft;
            } else if (minDist == distToRight) {
                direction = {1.0f, 0.0f};
                distance = distToRight;
            } else if (minDist == distToTop) {
                direction = {0.0f, -1.0f};
                distance = distToTop;
            } else {
                direction = {0.0f, 1.0f};
                distance = distToBottom;
            }
        } else {
            direction.x /= distance;
            direction.y /= distance;
        }
        
        float penetration = circleRadius - distance;
        
        if (penetration > 0.0f) {
            circlePos->x += direction.x * penetration;
            circlePos->y += direction.y * penetration;
        }
    }
    // Box vs Box (optional)
    else if (hasBox1 && hasBox2) {
    }
}