#ifndef ROOM_SYSTEM_HPP
#define ROOM_SYSTEM_HPP

#include "events/EventBus.hpp"
class ECS;
class Map;
class EventBus;

class RoomSystem {
private:
    static void updateEntitiesPositionsToRoomBounds(ECS &ecs);
    static void updateClearedFlag(ECS &ecs, Map &map, EventBus& eventBus);
public:
    static void update(ECS& ecs, Map& map, EventBus& eventBus);
};

#endif // ROOM_SYSTEM_HPP