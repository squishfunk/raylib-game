#pragma once

#include "../ecs/ecs.hpp"
#include "map.hpp"
#include <vector>

class Dungeon {
public:
    Dungeon(ECS& ecs, Map &map, int player);

    void loadRoom(const Room& room, DoorFlags entryDoor);

private:
    int playerId;
    ECS& ecs;
    Map& map;

    Vector2 currentRoom;
    std::vector<int> currentRoomEntities; 

    void spawnRoom(const Room& room, DoorFlags entryDoor);
    void despawnCurrentRoom();
    void spawnEnemies(const Room& room);
    void spawnDoors(const Room& room);
};