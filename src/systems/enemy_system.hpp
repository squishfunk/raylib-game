#ifndef ENEMY_SYSTEM_HPP
#define ENEMY_SYSTEM_HPP

#include <raylib.h>
#include "../components/components.hpp"

class ECS;

class EnemySystem {
private:
    static const EnemyConfig ENEMY_CONFIGS[];
    
public:
    static int createEnemy(ECS& ecs, Vector2 position, EnemyType type);
    static void updateMovement(ECS& ecs);
};

#endif // ENEMY_SYSTEM_HPP

