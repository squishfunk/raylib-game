#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "../map/Map.hpp"

struct DoorCollisionEvent {
    int entityId;
    DoorFlags doorDirection;
    
    DoorCollisionEvent(int id, DoorFlags direction) 
        : entityId(id), doorDirection(direction) {}
};

struct CircleCollisionEvent {
    int entityId1;
    int entityId2;
    
    CircleCollisionEvent(int id1, int id2) 
        : entityId1(id1), entityId2(id2) {}
};

struct ItemPickupEvent {
    int playerEntityId;
    int itemEntityId;
    
    ItemPickupEvent(int playerId, int itemId, const std::string& name = "") 
        : playerEntityId(playerId), itemEntityId(itemId) {}
};

#endif 

