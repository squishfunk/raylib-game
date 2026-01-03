#ifndef ECS_H
#define ECS_H

#include <raylib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 650 

#define MAX_ENTITIES 1024 /*  TODO */
#define MOVEMENT_SPEED 5
#define SHOOT_COOLDOWN 0.25f
#define BULLET_SPEED 10
#define BULLET_RADIUS 5

#define SPAWN_COOLDOWN 2
#define DAMAGE_COOLDOWN 1

#define ENEMY_DAMAGE 10
#define BULLET_DAMAGE 10

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
    int healthPoints;
    int maxHealthPoints;
    float lastDamageTime;
} HealthComponent;

typedef struct {
    float lastDamageTime;
} DamageCooldownComponent;

typedef enum {
    TAG_NONE = 0,
    TAG_BULLET = 1 << 0,    // 1
    TAG_PLAYER = 1 << 1,    // 2
    TAG_ENEMY = 1 << 2,     // 4
    // TAG_PICKUP = 1 << 3,    // 8
    // TAG_WALL = 1 << 4,      // 16
} EntityTag;

typedef struct {
    bool active;
    TransformComponent transform;
    VelocityComponent velocity;
    RenderableComponent renderable;
    HealthComponent health;
    DamageCooldownComponent damageCooldown;

    bool hasTransform;
    bool hasVelocity;
    bool hasRenderable;
    bool hasHealth;
    bool hasDamageCooldown;

    EntityTag tags;
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
int ecs_add_health(ECS *ecs, int entityId, int initialHealthPoints, int maxHealthPoints);
int ecs_add_damage_cooldown(ECS *ecs, int entityId);

/* Systems */
void movement_system(ECS *ecs);
void render_system(ECS *ecs);
void player_input_system(ECS *ecs, int entityId);
void collision_system(ECS *ecs);
void health_system(ECS *ecs);

/*  Shooting */
void shooting_system(ECS *ecs, int playerId, float currentTime, float *lastShootTime);
void bullet_system(ECS *ecs);

/* Enemies */
void enemy_spawn_system(ECS *ecs);

#endif