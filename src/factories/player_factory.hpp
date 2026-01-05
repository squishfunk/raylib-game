#pragma once

#include "../ecs/ecs.hpp"

class PlayerFactory {
public:
    static int create(ECS& ecs, const Vector2 desc);
};