#ifndef ECS_H
#define ECS_H

#include <raylib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 650 

#define MAX_ENTITIES 1024 /*  TODO */
#define MOVEMENT_SPEED 350
#define SHOOT_COOLDOWN 0.25f
#define BULLET_SPEED 200
#define BULLET_RADIUS 5

#define SPAWN_COOLDOWN 2
#define DAMAGE_COOLDOWN 1

#define ENEMY_DAMAGE 10
#define BULLET_DAMAGE 40

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

typedef enum {
    ENEMY_TYPE_NORMAL,
    ENEMY_TYPE_FAST,
    ENEMY_TYPE_TANK,
    ENEMY_TYPE_BOSS
} EnemyType;

typedef struct {
    EnemyType type;
    float movementSpeed;
    int damage;
    float attackCooldown;
    float lastAttackTime;
} EnemyComponent;

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
    EntityTag tags;
} Entity;

typedef struct {
    TransformComponent data[MAX_ENTITIES];
    bool active[MAX_ENTITIES];
} TransformStorage;

typedef struct {
    VelocityComponent data[MAX_ENTITIES];
    bool active[MAX_ENTITIES];
} VelocityStorage;

typedef struct {
    RenderableComponent data[MAX_ENTITIES];
    bool active[MAX_ENTITIES];
} RenderableStorage;

typedef struct {
    HealthComponent data[MAX_ENTITIES];
    bool active[MAX_ENTITIES];
} HealthStorage;

typedef struct {
    EnemyComponent data[MAX_ENTITIES];
    bool active[MAX_ENTITIES];
} EnemyStorage;

typedef struct {
    Entity entities[MAX_ENTITIES];
    int entityCount;
    
    TransformStorage transforms;
    VelocityStorage velocities;
    RenderableStorage renderables;
    HealthStorage healths;
    EnemyStorage enemies;
} ECS;

typedef struct {
    float radius;
    Color color;
    int health;
    int maxHealth;
    float movementSpeed;
    int damage;
    float attackCooldown;
} EnemyConfig;


/* ECS */
int ecs_create_entity(ECS *ecs);
void ecs_destroy_entity(ECS *ecs, int entityId);

/* Components - add */
void ecs_add_tranform(ECS *ecs, int entityId, Vector2 position);
void ecs_add_velocity(ECS *ecs, int entityId, Vector2 velocity);
void ecs_add_renderable(ECS *ecs, int entityId, float radius, Color color);
void ecs_add_health(ECS *ecs, int entityId, int initialHealthPoints, int maxHealthPoints);
void ecs_add_enemy(ECS *ecs, int entityId, EnemyComponent *enemyComponent);

/* Components -  */
// void ecs_remove_transform(ECS *ecs, int entityId);
// void ecs_remove_velocity(ECS *ecs, int entityId);
// void ecs_remove_renderable(ECS *ecs, int entityId);
// void ecs_remove_health(ECS *ecs, int entityId);

/* Components - get */
TransformComponent* ecs_get_transform(ECS *ecs, int entityId);
VelocityComponent* ecs_get_velocity(ECS *ecs, int entityId);
RenderableComponent* ecs_get_renderable(ECS *ecs, int entityId);
HealthComponent* ecs_get_health(ECS *ecs, int entityId);

/* Systems */
void movement_system(ECS *ecs);
void render_system(ECS *ecs);
void collision_system(ECS *ecs);
void health_system(ECS *ecs);

/*  Shooting */
int player_create(ECS *ecs, int initX, int initY);
void player_input_system(ECS *ecs, int entityId);
void player_shooting_system(ECS *ecs, int playerId, float currentTime, float *lastShootTime);

void bullet_system(ECS *ecs);

/* Enemies */
void enemy_movement_system(ECS *ecs);
int enemy_create(ECS *ecs, Vector2 position, EnemyType type);


#endif