#pragma once

#include "../ecs/Ecs.hpp"

class PlayerFactory {
public:
    static int create(ECS& ecs, const Vector2 desc);
};