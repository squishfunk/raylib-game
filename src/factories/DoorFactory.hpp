#pragma once

#include "../ecs/Ecs.hpp"
#include "../map/Map.hpp"

class DoorFactory {
public:
    static int create(ECS& ecs, Vector2 position, const DoorFlags doorFlag, bool opened);
};