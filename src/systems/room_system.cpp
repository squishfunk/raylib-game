#include "room_system.hpp"
#include "../ecs/ecs.hpp"
#include "../components/components.hpp"
#include "../map/map.hpp"

void RoomSystem::update(ECS& ecs, Map& map) {
    Room& currentRoom = map.getRoom(map.getCurrentX(), map.getCurrentY());
    
    if (currentRoom.cleared) return;
    
    bool hasActiveEnemies = false;
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    
    for (int i = 0; i < entityCount; i++) {
        if (entities[i].active && (entities[i].tags & EntityTag::ENEMY) == EntityTag::ENEMY) {
            hasActiveEnemies = true;
            break;
        }
    }
    
    if (!hasActiveEnemies) {
        currentRoom.cleared = true;
    }
}

