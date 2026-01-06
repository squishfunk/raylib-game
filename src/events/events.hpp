#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "../map/map.hpp"

struct DoorCollisionEvent {
    int entityId;
    DoorFlags doorDirection;
    
    DoorCollisionEvent(int id, DoorFlags direction) 
        : entityId(id), doorDirection(direction) {}
};

#endif // EVENTS_HPP

