#ifndef ENEMY_SYSTEM_HPP
#define ENEMY_SYSTEM_HPP

#include <raylib.h>
#include "../components/components.hpp"

class ECS;

class EnemySystem {
private:
    static const EnemyConfig ENEMY_CONFIGS[];
    
public:
    static void generateRoomEnemies(ECS& ecs, int screenWidth, int screenHeight);
    static int createEnemy(ECS& ecs, Vector2 position, EnemyType type, int screenWidth, int screenHeight);
    static void updateMovement(ECS& ecs);
};

#endif // ENEMY_SYSTEM_HPP

