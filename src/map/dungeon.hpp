#pragma once

#include "../ecs/ecs.hpp"
#include "map.hpp"
#include "../events/eventbus.hpp"
#include "../events/events.hpp"
#include <vector>

class Dungeon {
public:
    Dungeon(ECS& ecs, Map &map, int player, EventBus& eventBus);

    void loadRoom(const Room& room, DoorFlags entryDoor);

private:
    int playerId;
    ECS& ecs;
    Map& map;
    EventBus& eventBus;

    Vector2 currentRoom;
    std::vector<int> currentRoomEntities; 

    void spawnRoom(const Room& room, DoorFlags entryDoor);
    void despawnCurrentRoom();
    void spawnEnemies(const Room& room);
    void spawnDoors(const Room& room);
    void onDoorCollision(const DoorCollisionEvent& event);
    
    DoorFlags getOppositeDoor(DoorFlags door) const;
};