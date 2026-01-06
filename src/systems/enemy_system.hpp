#ifndef ENEMY_SYSTEM_HPP
#define ENEMY_SYSTEM_HPP

#include <raylib.h>

class ECS;

class EnemySystem {
public:
    static void update(ECS& ecs);
};

#endif // ENEMY_SYSTEM_HPP

