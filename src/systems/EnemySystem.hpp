#ifndef ENEMY_SYSTEM_HPP
#define ENEMY_SYSTEM_HPP

#include <raylib.h>
#include "components/Components.hpp"

class ECS;

class EnemySystem {
    static void HandleMove(ECS& ecs, int enemyId, const TransformComponent& playerTransform);
public:
    static void update(ECS& ecs);
    static void handleShooting(ECS& ecs, int enemyId, int playerId);
};

#endif

