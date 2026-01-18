#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <raylib.h>
#include <cmath>

class ECS;

namespace Helpers {
    bool checkCircleCollision(Vector2 pos1, float radius1, Vector2 pos2, float radius2);
    bool checkCircleRectCollision(Vector2 circlePos, float circleRadius, Rectangle rect, float tolerance = 0);
    bool checkRectRectCollision(Vector2 rect1Pos, float rect1Width, float rect1Height, Vector2 rect2Pos, float rect2Width, float rect2Height);
    int getPlayerId(const ECS& ecs);
}

#endif // HELPERS_HPP

