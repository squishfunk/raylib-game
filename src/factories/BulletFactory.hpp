#pragma once

#include "../ecs/Ecs.hpp"

class BulletFactory {
public:
    static int create(ECS& ecs, Vector2 position, Vector2 velocity, float speed);
};