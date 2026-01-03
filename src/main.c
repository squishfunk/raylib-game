#include <raylib.h>
#include "ecs.h"

#define GAME_NAME "dupa"

#define MAX_BULLETS 100


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);

    SetTargetFPS(60);

    ECS ecs = {0};

    int playerId = ecs_create_entity(&ecs);
    ecs_add_tranform(&ecs, playerId, (Vector2){0,0});
    ecs_add_velocity(&ecs, playerId, (Vector2){0,0});
    ecs_add_renderable(&ecs, playerId, 20.0f, GREEN);

    float lastShootTime = 0.0f;

    while (!WindowShouldClose())
    {
        float currentTime = GetTime();

        player_input_system(&ecs, playerId);
        shooting_system(&ecs, playerId, currentTime, &lastShootTime);
        enemy_spawn_system(&ecs);
        movement_system(&ecs);
        
        BeginDrawing();

            ClearBackground(RAYWHITE);
            render_system(&ecs);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
