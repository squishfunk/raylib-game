#ifndef ROOM_SYSTEM_HPP
#define ROOM_SYSTEM_HPP

class ECS;
class Map;

class RoomSystem {
private:
    static void updateEntitiesPositionsToRoomBounds(ECS &ecs);
    static void updateClearedFlag(ECS &ecs, Map &map);
public:
    static void update(ECS& ecs, Map& map);
};

#endif // ROOM_SYSTEM_HPP