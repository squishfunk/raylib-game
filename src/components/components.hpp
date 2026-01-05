#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <raylib.h>
#include <cstdint>


constexpr int MAX_ENTITIES = 1024;
constexpr float MOVEMENT_SPEED = 350.0f;
constexpr float SHOOT_COOLDOWN = 0.25f;
constexpr float BULLET_SPEED = 200.0f;
constexpr float BULLET_RADIUS = 5.0f;
constexpr float DAMAGE_COOLDOWN = 1.0f;
constexpr int ENEMY_DAMAGE = 10;
constexpr int BULLET_DAMAGE = 40;

struct TransformComponent {
    Vector2 position;
};

struct VelocityComponent {
    Vector2 velocity;
};

struct RenderableComponent {
    float radius;
    Color color;
};

struct HealthComponent {
    int healthPoints;
    int maxHealthPoints;
    float lastDamageTime;
};

enum class EnemyType {
    NORMAL,
    FAST,
    TANK,
    BOSS
};

struct EnemyComponent {
    EnemyType type;
    float movementSpeed;
    int damage;
    float attackCooldown;
    float lastAttackTime;
};

enum class EntityTag : uint8_t {
    NONE = 0,
    BULLET = 1 << 0,
    PLAYER = 1 << 1,
    ENEMY = 1 << 2,
};

inline EntityTag operator|(EntityTag a, EntityTag b) {
    return static_cast<EntityTag>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline EntityTag operator&(EntityTag a, EntityTag b) {
    return static_cast<EntityTag>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline EntityTag& operator|=(EntityTag& a, EntityTag b) {
    a = a | b;
    return a;
}

struct DoorComponent {
    float width;
    float height;
};

struct ShootableComponent {
    float lastShootTime;
    float shootingRange;
    float shootingSpeed;
    float shootCooldown;
};

struct EnemyConfig {
    float radius;
    Color color;
    int health;
    int maxHealth;
    float movementSpeed;
    int damage;
    float attackCooldown;
};

#endif // COMPONENTS_HPP

