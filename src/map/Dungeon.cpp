#include "Dungeon.hpp"
#include "Map.hpp"
#include "../factories/DoorFactory.hpp"
#include "../factories/EnemyFactory.hpp"
#include "../factories/ItemFactory.hpp"
#include "events/Events.hpp"
#include "map/Room.hpp"
#include "textures/TextureManager.hpp"
#include <cassert>
#include <cstdio>
#include <raylib.h>


Dungeon::Dungeon(ECS& ecs, Map &map, int playerId, EventBus& eventBus): 
    playerId(playerId), ecs(ecs), map(map), eventBus(eventBus) {
    
    eventBus.subscribe<DoorCollisionEvent>([this](const DoorCollisionEvent& event) {
        onDoorCollision(event);
    });

    eventBus.subscribe<ClearedRoomEvent>([this](const ClearedRoomEvent& event) {
        onRoomClearEvent(event);
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

    float margin = 200.0f;

    switch(entryDoor){
        case DoorFlags::UP: 
            
            newPosition.x = room.structure.gridWidth * 100 / 2.0f;
            newPosition.y = margin;
            break;
        case DoorFlags::DOWN: 
            newPosition.x = room.structure.gridWidth * 100  / 2.0f;
            newPosition.y = room.structure.gridHeight * 100 - margin;
            break;
        case DoorFlags::LEFT: 
            newPosition.x = margin;
            newPosition.y = room.structure.gridHeight * 100  / 2.0f;
            break;
        case DoorFlags::RIGHT: 
            newPosition.x = room.structure.gridWidth * 100 - margin;
            newPosition.y = room.structure.gridHeight * 100 / 2.0f;
            break;
        default:
            newPosition.x = room.structure.gridWidth * 100 / 2.0f;
            newPosition.y = room.structure.gridHeight * 100 / 2.0f;
            break;
    }
    playerTransform->position = newPosition;

    if(!room.cleared){
        spawnEnemies(room);
    }

    spawnWalls(room);
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

void Dungeon::spawnWalls(const Room& room) {

    // const DoorFlags allDoors[] = {
    //     DoorFlags::UP,
    //     DoorFlags::DOWN,
    //     DoorFlags::LEFT,
    //     DoorFlags::RIGHT
    // };
    
    // for (DoorFlags doorFlag : allDoors) {
    //     if ((room.doors & doorFlag) != DoorFlags::NONE) {
    //         int door = DoorFactory::create(ecs, doorFlag, room.cleared);
    //         if (door != -1) {
    //             currentRoomEntities.push_back(door);
    //         }
    //     }
    // }
    
    /* TOOD move to factory */
    for(int gridY = 0; gridY < (int)room.structure.grid.size(); gridY++) {
        for(int gridX = 0; gridX < (int)room.structure.grid[gridY].size(); gridX++) {

            if(room.structure.grid[gridY][gridX] == TileType::WALL) {
                float worldX = gridX * TILE_SIZE;
                float worldY = gridY * TILE_SIZE;
                
                int wallId = ecs.createEntity();
                ecs.addTransform(wallId, {worldX, worldY});
                ecs.addSpriteRenderer(wallId, {TextureManager::Get("Environment/wall.png"), {0,0,100,100}});
                ecs.addBoxCollider(wallId, 100, 100);
            }

            /* DOOR */
            if(room.structure.grid[gridY][gridX] >= TileType::DOOR_UP && room.structure.grid[gridY][gridX] <= TileType::DOOR_LEFT) {



                Vector2 position = {gridX * TILE_SIZE, gridY * TILE_SIZE};

                DoorFlags doorFlag; 

                switch(room.structure.grid[gridY][gridX]){
                    case TileType::DOOR_UP:
                        doorFlag = DoorFlags::UP;
                        break;
                    case TileType::DOOR_DOWN:
                        doorFlag = DoorFlags::DOWN;
                        break;
                    case TileType::DOOR_LEFT:
                        doorFlag = DoorFlags::LEFT;
                        break;
                    case TileType::DOOR_RIGHT:
                        doorFlag = DoorFlags::RIGHT;
                        break;
                    default:
                        assert(false);
                        break;
                }

                
                DoorFactory::create(ecs, position, doorFlag, room.cleared);
            }
        }
    }
}

void Dungeon::spawnDoors(const Room& room) {
    // const DoorFlags allDoors[] = {
    //     DoorFlags::UP,
    //     DoorFlags::DOWN,
    //     DoorFlags::LEFT,
    //     DoorFlags::RIGHT
    // };
    
    // for (DoorFlags doorFlag : allDoors) {
    //     if ((room.doors & doorFlag) != DoorFlags::NONE) {
    //         int door = DoorFactory::create(ecs, doorFlag, room.cleared);
    //         if (door != -1) {
    //             currentRoomEntities.push_back(door);
    //         }
    //     }
    // }
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

void Dungeon::onRoomClearEvent(const ClearedRoomEvent& event){
    if(map.getCurrentRoom().type == RoomType::BOSS){
        NextLevelEvent e;
        eventBus.emit(e);
    }
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