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

