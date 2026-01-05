#pragma once

#include "../ecs/ecs.hpp"
#include "../components/components.hpp"
#include <raylib.h>

struct EnemySpawnData {
    Vector2 position;
    EnemyType type;
    Rectangle bounds;
};

class EnemyFactory {
public:
    static int create(ECS& ecs, const EnemySpawnData& data);
};