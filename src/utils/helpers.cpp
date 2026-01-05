#include "helpers.hpp"
#include "../ecs/ecs.hpp"

namespace Helpers {
    bool checkCircleCollision(Vector2 pos1, float radius1, Vector2 pos2, float radius2) {
        float dx = pos1.x - pos2.x;
        float dy = pos1.y - pos2.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        float minDistance = radius1 + radius2;
        return distance < minDistance;
    }
    
    bool checkCircleRectCollision(Vector2 circlePos, float circleRadius, Vector2 rectPos, float rectWidth, float rectHeight) {
        float rectLeft = rectPos.x - rectWidth / 2.0f;
        float rectRight = rectPos.x + rectWidth / 2.0f;
        float rectTop = rectPos.y - rectHeight / 2.0f;
        float rectBottom = rectPos.y + rectHeight / 2.0f;
        
        float closestX = std::max(rectLeft, std::min(circlePos.x, rectRight));
        float closestY = std::max(rectTop, std::min(circlePos.y, rectBottom));
        
        float dx = circlePos.x - closestX;
        float dy = circlePos.y - closestY;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        return distance < circleRadius;
    }
    
    int getPlayerId(const ECS& ecs) {
        const auto& entities = ecs.getEntities();
        int entityCount = ecs.getEntityCount();
        
        for (int i = 0; i < entityCount; i++) {
            if (entities[i].active && (entities[i].tags & EntityTag::PLAYER) == EntityTag::PLAYER) {
                return i;
            }
        }
        return -1;
    }
}

