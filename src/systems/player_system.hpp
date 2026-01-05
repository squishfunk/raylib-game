#ifndef PLAYER_SYSTEM_HPP
#define PLAYER_SYSTEM_HPP

#include <raylib.h>

class ECS;

class PlayerSystem {
public:
    static int createPlayer(ECS& ecs, int initX, int initY);
    static void handleInput(ECS& ecs, int entityId);
    static void handleShooting(ECS& ecs, int playerId, float currentTime, float& lastShootTime);
};

#endif // PLAYER_SYSTEM_HPP

