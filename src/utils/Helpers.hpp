#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <raylib.h>
#include <cmath>

class ECS;

namespace Helpers {
    bool checkCircleCollision(Vector2 pos1, float radius1, Vector2 pos2, float radius2);
    bool checkCircleRectCollision(Vector2 circlePos, float circleRadius, Vector2 rectPos, float rectWidth, float rectHeight);
    int getPlayerId(const ECS& ecs);
}

#endif // HELPERS_HPP

