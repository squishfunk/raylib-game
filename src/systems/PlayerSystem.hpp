#ifndef PLAYER_SYSTEM_HPP
#define PLAYER_SYSTEM_HPP

#include <raylib.h>

class ECS;

class PlayerSystem {
    static void handleInput(ECS& ecs, int playerId);
    static void handleShooting(ECS& ecs, int playerId);
    static void handleCamera(ECS& ecs, int playerId, Camera2D& camera);
public:
    static void update(ECS &ecs, Camera2D& camera);
};

#endif // PLAYER_SYSTEM_HPP

