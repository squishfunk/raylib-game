#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include <raylib.h>

class ECS;
class DrunkMovementEffect;

class MovementSystem {
public:
    static void update(ECS& ecs);
    static void applyDrunkEffect(const DrunkMovementEffect* drunkEffect, Vector2* movementDirection, float* finalSpeed);
};

#endif // MOVEMENT_SYSTEM_HPP

