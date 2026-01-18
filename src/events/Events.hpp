#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "../map/Map.hpp"
#include "map/Room.hpp"

struct DoorCollisionEvent {
    int entityId;
    DoorFlags doorDirection;
    
    DoorCollisionEvent(int id, DoorFlags direction) 
        : entityId(id), doorDirection(direction) {}
};

struct CollisionEvent {
    int entityId1;
    int entityId2;
    
    CollisionEvent(int id1, int id2) 
        : entityId1(id1), entityId2(id2) {}
};

struct ItemPickupEvent {
    int playerEntityId;
    int itemEntityId;
    
    ItemPickupEvent(int playerId, int itemId, const std::string& name = "") 
        : playerEntityId(playerId), itemEntityId(itemId) {}
};

struct ClearedRoomEvent {
    RoomCord roomCord;

    ClearedRoomEvent(RoomCord roomCord): roomCord(roomCord)
    {}
};

struct NextLevelEvent{
    NextLevelEvent() = default;
};

#endif 

