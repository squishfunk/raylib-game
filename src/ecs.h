#ifndef ECS_H
#define ECS_H

#include <raylib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 650 

#define MAX_ENTITIES 10 /*  TODO */
#define MOVEMENT_SPEED 5
#define SHOOT_COOLDOWN 0.25f
#define BULLET_SPEED 10
#define BULLET_RADIUS 5

typedef struct {
    Vector2 position;
} TransformComponent;

typedef struct {
    Vector2 velocity;
} VelocityComponent;

typedef struct {
    float radius;
    Color color;
} RenderableComponent;

typedef struct {
    bool active;
    TransformComponent transform;
    VelocityComponent velocity;
    RenderableComponent renderable;

    bool hasTransform;
    bool hasVelocity;
    bool hasRenderable;
} Entity;

typedef struct {
    Entity entities[MAX_ENTITIES];
    int entityCount;
} ECS;

/* ECS */
int ecs_create_entity(ECS *ecs);
int ecs_add_tranform(ECS *ecs, int entityId, Vector2 position);
int ecs_add_velocity(ECS *ecs, int entityId, Vector2 velocity);
int ecs_add_renderable(ECS *ecs, int entityId, float radius, Color color);

/* Systems */
void movement_system(ECS *ecs);
void render_system(ECS *ecs);
void player_input_system(ECS *ecs, int entityId);

/*  Shooting */
void shooting_system(ECS *ecs, int playerId, float currentTime, float *lastShootTime);
void bullet_cleanup_system(ECS *ecs);

/* Enemies */
void enemy_spawn_system(ECS *ecs);

#endif