#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <raylib.h>
#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include "BehaviourEffects.hpp"

enum class DoorFlags : uint8_t;


constexpr int MAX_ENTITIES = 1024;
constexpr float MOVEMENT_SPEED = 400.0f;
constexpr float SHOOT_COOLDOWN = 0.25f;
constexpr float BULLET_SPEED = 200.0f;
constexpr float BULLET_RADIUS = 5.0f;
constexpr float DAMAGE_COOLDOWN = 1.0f;
constexpr int ENEMY_DAMAGE = 10;
constexpr int BULLET_DAMAGE = 40;

enum class StatType {
    HEALTH,
    SPEED,
    DAMAGE,
    FIRE_RATE
};

enum class EffectType {
    ADDITIVE,
    MULTIPLICATIVE
};

struct TransformComponent {
    Vector2 position;
};

struct VelocityComponent {
    Vector2 velocity;
    float speed;
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
    BOSS,
    RANGED_NORMAL,
    RANGED_TANK,
};

struct EnemyComponent {
    EnemyType type;
    int damage;
    float attackCooldown;
    float lastAttackTime;
    float lastSoundTime;
    float nextSoundTime;
    bool ranged;
};

enum class EntityTag : uint8_t {
    NONE = 0,
    PLAYER = 1 << 0,
    ENEMY = 1 << 1,
    BULLET = 1 << 2,
    ENEMY_BULLET = 1 << 3,
    ITEM = 1 << 4,
    PIERCING_BULLET = 1 << 5,
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
    DoorFlags direction;
    bool opened;
};

struct ShootableComponent {
    float lastShootTime;
    float shootingRange;
    float shootingSpeed;
    float shootCooldown;
    Vector2 direction;
    bool shoot;
};

struct AudioComponent {
    std::unordered_map<std::string, Sound> sounds;
    
    void addSound(const std::string& key, Sound sound) {
        sounds[key] = sound;
    }
    
    Sound getSound(const std::string& key) const {
        auto it = sounds.find(key);
        if (it != sounds.end() && it->second.frameCount > 0) {
            return it->second;
        }
        Sound emptySound = {};
        return emptySound; 
    }
    
    bool hasSound(const std::string& key) const {
        auto it = sounds.find(key);
        return it != sounds.end() && it->second.frameCount > 0;
    }
    
    void play(const std::string& key) const {
        auto it = sounds.find(key);
        if (it != sounds.end() && it->second.frameCount > 0) {
            PlaySound(it->second);
        }
    }
};

struct CircleColliderComponent {
    float radius;
    bool isTrigger = false;
};

struct StatEffect {
    StatType type;
    EffectType effectType;
    float value;
};

struct StatsManagerComponent {
    std::vector<StatEffect> effects;
    
    void addEffect(const StatEffect& effect) {
        effects.push_back(effect);
    }
    
    float getFinalValue(StatType statType, float baseValue) const {
        float additiveSum = 0.0f;
        float multiplicativeProduct = 1.0f;
        
        for (const auto& effect : effects) {
            if (effect.type == statType) {
                if (effect.effectType == EffectType::ADDITIVE) {
                    additiveSum += effect.value;
                } else if (effect.effectType == EffectType::MULTIPLICATIVE) {
                    multiplicativeProduct *= effect.value;
                }
            }
        }
        
        return (baseValue + additiveSum) * multiplicativeProduct;
    }
};

struct BehaviourModifierComponent {
    std::vector<std::shared_ptr<BehaviourEffectBase>> effects;
    
    void addEffect(std::shared_ptr<BehaviourEffectBase> effect) {
        effects.push_back(effect);
    }
    
    bool hasEffect(BehaviourEffectType type) const {
        for (const auto& effect : effects) {
            if (effect && effect->getType() == type) {
                return true;
            }
        }
        return false;
    }
};

struct ItemComponent {
    std::string name;
    std::vector<StatEffect> statEffects;
    std::vector<std::shared_ptr<BehaviourEffectBase>> behaviourEffects;
    bool isPickedUp = false;
};

struct EnemyConfig {
    float radius;
    Color color;
    int health;
    int maxHealth;
    float movementSpeed;
    int damage;
    float attackCooldown;
    bool ranged;
};

#endif

