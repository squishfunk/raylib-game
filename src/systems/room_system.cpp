#include "room_system.hpp"
#include "../ecs/ecs.hpp"
#include "enemy_system.hpp"
#include <raylib.h>

void RoomSystem::createRoom(ECS& ecs, int screenWidth, int screenHeight) {
    EnemySystem::generateRoomEnemies(ecs, screenWidth, screenHeight);
    
    const float doorWidth = 80.0f;
    const float doorHeight = 20.0f;

    int topDoorId = ecs.createEntity();
    if (topDoorId >= 0) {
        ecs.addTransform(topDoorId, Vector2{static_cast<float>(screenWidth) / 2.0f - doorWidth / 2.0f, 0});
        ecs.addDoor(topDoorId, doorWidth, doorHeight);
    }
    
    int bottomDoorId = ecs.createEntity();
    if (bottomDoorId >= 0) {
        ecs.addTransform(bottomDoorId, Vector2{static_cast<float>(screenWidth) / 2.0f - doorWidth / 2.0f, static_cast<float>(screenHeight) - doorHeight});
        ecs.addDoor(bottomDoorId, doorWidth, doorHeight);
    }
    
    int leftDoorId = ecs.createEntity();
    if (leftDoorId >= 0) {
        ecs.addTransform(leftDoorId, Vector2{0, static_cast<float>(screenHeight) / 2.0f - doorWidth / 2.0f});
        ecs.addDoor(leftDoorId, doorHeight, doorWidth);
    }
    
    int rightDoorId = ecs.createEntity();
    if (rightDoorId >= 0) {
        ecs.addTransform(rightDoorId, Vector2{static_cast<float>(screenWidth) - doorHeight, static_cast<float>(screenHeight) / 2.0f - doorWidth / 2.0f});
        ecs.addDoor(rightDoorId, doorHeight, doorWidth);
    }
}

void RoomSystem::initRoom(ECS& ecs, int screenWidth, int screenHeight) {
    createRoom(ecs, screenWidth, screenHeight);
}

