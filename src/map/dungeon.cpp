#include "dungeon.hpp"
#include "map.hpp"
#include "../factories/door_factory.hpp"
#include "../factories/enemy_factory.hpp"
#include <cassert>
#include <cstdio>


Dungeon::Dungeon(ECS& ecs, Map &map, int playerId): playerId(playerId), ecs(ecs), map(map){
}

void Dungeon::loadRoom(const Room& room, DoorFlags entryDoor){
    despawnCurrentRoom();
    spawnRoom(room, entryDoor);
}


void Dungeon::spawnRoom(const Room& room, DoorFlags entryDoor) {
    auto* playerTransform = ecs.getTransform(playerId);
    assert(playerTransform != nullptr && "Player transform not found!");

    assert(room.bounds.width > 0 && room.bounds.height > 0 && "Room bounds are invalid!");

    Vector2 newPosition {};

    switch(entryDoor){
        case DoorFlags::UP: 
            newPosition.x = room.bounds.width / 2.0f;
            newPosition.y = room.bounds.height - 50.0f;
            break;
        case DoorFlags::DOWN: 
            newPosition.x = room.bounds.width / 2.0f;
            newPosition.y = 50.0f;
            break;
        case DoorFlags::LEFT: 
            newPosition.x = room.bounds.width - 50.0f;
            newPosition.y = room.bounds.height / 2.0f;
            break;
        case DoorFlags::RIGHT: 
            newPosition.x = 50.0f;
            newPosition.y = room.bounds.height / 2.0f;
            break;
        default:
            newPosition.x = room.bounds.width / 2.0f;
            newPosition.y = room.bounds.height / 2.0f;
            break;
    }
    playerTransform->position = newPosition;

    spawnDoors(room);
    spawnEnemies(room);
}

void Dungeon::despawnCurrentRoom() {
    for (int e : currentRoomEntities) {
        ecs.destroyEntity(e);
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
            int door = DoorFactory::create(ecs, doorFlag);
            if (door != -1) {
                currentRoomEntities.push_back(door);
            }
        }
    }
}