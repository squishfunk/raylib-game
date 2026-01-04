#ifndef HELPERS_H
#define HELPERS_H

#include <raylib.h>
#include "ecs.h"

bool check_circle_collision(Vector2 pos1, float radius1, Vector2 pos2, float radius2);

int get_player_id(ECS *ecs);

#endif