#include "Helpers.hpp"
#include "../ecs/Ecs.hpp"
#include <raylib.h>

namespace Helpers {
    bool checkCircleCollision(Vector2 pos1, float radius1, Vector2 pos2, float radius2) {
        float dx = pos1.x - pos2.x;
        float dy = pos1.y - pos2.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        float minDistance = radius1 + radius2;
        return distance < minDistance;
    }
    
    bool checkCircleRectCollision(Vector2 circlePos, float circleRadius, Rectangle rect, float tolerance) {
        float rectLeft = rect.x;
        float rectRight = rect.x + rect.width;
        float rectTop = rect.y;
        float rectBottom = rect.y + rect.height;
        
        float closestX = std::max(rectLeft, std::min(circlePos.x, rectRight));
        float closestY = std::max(rectTop, std::min(circlePos.y, rectBottom));
        
        float dx = circlePos.x - closestX;
        float dy = circlePos.y - closestY;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        return distance < (circleRadius + tolerance);
    }

    bool checkRectRectCollision(Vector2 rect1Pos, float rect1Width, float rect1Height, 
        Vector2 rect2Pos, float rect2Width, float rect2Height) {
        float rect1Left = rect1Pos.x;
        float rect1Right = rect1Pos.x + rect1Width;
        float rect1Top = rect1Pos.y;
        float rect1Bottom = rect1Pos.y + rect1Height;

        float rect2Left = rect2Pos.x;
        float rect2Right = rect2Pos.x + rect2Width;
        float rect2Top = rect2Pos.y;
        float rect2Bottom = rect2Pos.y + rect2Height;

        return !(rect1Right < rect2Left || rect1Left > rect2Right || 
        rect1Bottom < rect2Top || rect1Top > rect2Bottom);
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

