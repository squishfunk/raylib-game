#include "RoomSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../map/Map.hpp"
#include "RoomSystem.hpp"
#include <raylib.h>

constexpr int SCREEN_HEIGHT = 650;
constexpr int SCREEN_WIDTH = 1000;

void RoomSystem::updateClearedFlag(ECS& ecs, Map& map) {
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
        /* TODO Emit event */
        currentRoom.cleared = true;
    }

    
    auto& doors = ecs.getDoors();
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if (!doors.isActive(i)) continue;
        
        auto& doorComponent = doors.get(i);
        doorComponent.opened = !hasActiveEnemies;
    }
}

void RoomSystem::updateEntitiesPositionsToRoomBounds(ECS &ecs){
    int entityCount = ecs.getEntityCount();

    const auto& entities = ecs.getEntities();

    for (int i = 0; i < entityCount; i++){
        if(!entities[i].active) continue;
        if(!((entities[i].tags & EntityTag::PLAYER) == EntityTag::PLAYER)) continue;
        auto& playerTransform = ecs.getTransforms().get(i);
        if(playerTransform.position.x >= SCREEN_WIDTH){
            playerTransform.position.x = SCREEN_WIDTH;
        }
        if(playerTransform.position.x <= 0){
            playerTransform.position.x = 0;
        }
        if(playerTransform.position.y >= SCREEN_HEIGHT){
            playerTransform.position.y = SCREEN_HEIGHT;
        }
        if(playerTransform.position.y <= 0){
            playerTransform.position.y = 0;
        }
    }
}

void RoomSystem::update(ECS& ecs, Map& map) {
    updateClearedFlag(ecs, map);
    updateEntitiesPositionsToRoomBounds(ecs);
}

