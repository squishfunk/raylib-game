#pragma once

#include "../ecs/ecs.hpp"
#include "../map/map.hpp"

class DoorFactory {
public:
    static int create(ECS& ecs, const DoorFlags doorFlag);
};