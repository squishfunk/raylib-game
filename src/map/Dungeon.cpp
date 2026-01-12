#include "Dungeon.hpp"
#include "Map.hpp"
#include "../factories/DoorFactory.hpp"
#include "../factories/EnemyFactory.hpp"
#include "../factories/ItemFactory.hpp"
#include <cassert>
#include <cstdio>
#include <raylib.h>


Dungeon::Dungeon(ECS& ecs, Map &map, int playerId, EventBus& eventBus): 
    playerId(playerId), ecs(ecs), map(map), eventBus(eventBus) {
    
    eventBus.subscribe<DoorCollisionEvent>([this](const DoorCollisionEvent& event) {
        onDoorCollision(event);
    });
}

void Dungeon::loadRoom(const Room& room, DoorFlags entryDoor){
    despawnCurrentRoom();
    spawnRoom(room, entryDoor);
}


void Dungeon::spawnRoom(const Room& room, DoorFlags entryDoor) {
    auto* playerTransform = ecs.getTransform(playerId);
    assert(playerTransform != nullptr && "Player transform not found!");

    assert(room.bounds.width > 0 && room.bounds.height > 0 && "Room bounds are invalid!");

    map.setCurrentRoom(room.gridX, room.gridY);

    Vector2 newPosition {};

    switch(entryDoor){
        case DoorFlags::UP: 
            newPosition.x = room.bounds.width / 2.0f;
            newPosition.y = 100.0f;
            break;
        case DoorFlags::DOWN: 
            newPosition.x = room.bounds.width / 2.0f;
            newPosition.y = room.bounds.height - 100.0f;
            break;
        case DoorFlags::LEFT: 
            newPosition.x = 100.0f;
            newPosition.y = room.bounds.height / 2.0f;
            break;
        case DoorFlags::RIGHT: 
            newPosition.x = room.bounds.width - 100.0f;
            newPosition.y = room.bounds.height / 2.0f;
            break;
        default:
            newPosition.x = room.bounds.width / 2.0f;
            newPosition.y = room.bounds.height / 2.0f;
            break;
    }
    playerTransform->position = newPosition;

    if(!room.cleared){
        spawnEnemies(room);
    }
    spawnDoors(room);
    
    if (room.type == RoomType::TREASURE && !room.cleared) {
        spawnItems(room);
    }
}

void Dungeon::despawnCurrentRoom() {
    for (int i = 0; i < ecs.getEntityCount(); i++) {
        if (i == playerId) continue;
        ecs.destroyEntity(i);
    }
    currentRoomEntities.clear();
}

void Dungeon::spawnEnemies(const Room& room) {
    for (const auto& spawn : room.enemySpawns) {
        EnemySpawnData enemySpawnData = {
            spawn.position, 
            spawn.type, 
            room.bounds
        };

        int enemyId = EnemyFactory::create(
            ecs, 
            enemySpawnData
        );

        if (enemyId >= 0) {
            currentRoomEntities.push_back(enemyId);
        }
    }
}

void Dungeon::spawnDoors(const Room& room) {
    const DoorFlags allDoors[] = {
        DoorFlags::UP,
        DoorFlags::DOWN,
        DoorFlags::LEFT,
        DoorFlags::RIGHT
    };
    
    for (DoorFlags doorFlag : allDoors) {
        if ((room.doors & doorFlag) != DoorFlags::NONE) {
            int door = DoorFactory::create(ecs, doorFlag, room.cleared);
            if (door != -1) {
                currentRoomEntities.push_back(door);
            }
        }
    }
}

void Dungeon::spawnItems(const Room& room) {
    int itemCount = 1;
    
    for (int i = 0; i < itemCount; i++) {
        float x = room.bounds.width / 2;
        float y = room.bounds.height / 2;
        
        int itemId = ItemFactory::createRandomItem(ecs, Vector2{x, y});
        if (itemId >= 0) {
            currentRoomEntities.push_back(itemId);
        }
    }
}

void Dungeon::onDoorCollision(const DoorCollisionEvent& event) {
    int currentX = map.getCurrentX();
    int currentY = map.getCurrentY();
    
    const Room& currentRoom = map.getRoom(currentX, currentY);

    if (!currentRoom.cleared) {
        return;
    }
    
    int newX = currentX;
    int newY = currentY;
    
    switch(event.doorDirection) {
        case DoorFlags::UP:
            newY = currentY - 1;
            break;
        case DoorFlags::DOWN:
            newY = currentY + 1;
            break;
        case DoorFlags::LEFT:
            newX = currentX - 1;
            break;
        case DoorFlags::RIGHT:
            newX = currentX + 1;
            break;
        default:
            return;
    }
    
    assert(!(newX < 0 || newX >= MAP_WIDTH || newY < 0 || newY >= MAP_HEIGHT) && "Trying to access room that doesn't exist"); 
    
    const Room& targetRoom = map.getRoom(newX, newY);
    
    assert(!((currentRoom.doors & event.doorDirection) == DoorFlags::NONE) && "Trying to move from room through non existing doors");
    
    if (ecs.getAudios().isActive(event.entityId)) {
        ecs.getAudio(event.entityId)->play("DOOR_SOUND");
    }
    
    DoorFlags entryDoor = getOppositeDoor(event.doorDirection);
    
    loadRoom(targetRoom, entryDoor);
}

DoorFlags Dungeon::getOppositeDoor(DoorFlags door) const {
    switch(door) {
        case DoorFlags::UP:
            return DoorFlags::DOWN;
        case DoorFlags::DOWN:
            return DoorFlags::UP;
        case DoorFlags::LEFT:
            return DoorFlags::RIGHT;
        case DoorFlags::RIGHT:
            return DoorFlags::LEFT;
        default:
            return DoorFlags::NONE;
    }
}