#ifndef ROOM_SYSTEM_HPP
#define ROOM_SYSTEM_HPP

class ECS;

class RoomSystem {
public:
    static void createRoom(ECS& ecs, int screenWidth, int screenHeight);
    static void initRoom(ECS& ecs, int screenWidth, int screenHeight);
};

#endif // ROOM_SYSTEM_HPP

