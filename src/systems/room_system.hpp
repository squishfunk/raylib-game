#ifndef ROOM_SYSTEM_HPP
#define ROOM_SYSTEM_HPP

class ECS;
class Map;

class RoomSystem {
public:
    static void update(ECS& ecs, Map& map);
};

#endif // ROOM_SYSTEM_HPP

