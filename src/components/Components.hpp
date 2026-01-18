#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <raylib.h>
#include <cstdint>
#include <set>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <raylib.h>
#include "BehaviourEffects.hpp"

enum class DoorFlags : uint8_t;


constexpr int MAX_ENTITIES = 1024;
constexpr float MOVEMENT_SPEED = 400.0f;
constexpr float SHOOT_COOLDOWN = 0.25f;
constexpr float BULLET_SPEED = 500.0f;
constexpr float BULLET_RADIUS = 5.0f;
constexpr float DAMAGE_COOLDOWN = 1.0f;
constexpr int ENEMY_DAMAGE = 1;
constexpr int BULLET_DAMAGE = 1;

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
    float lastSoundTime;
    float nextSoundTime;
    float spawnTime;
    float spawnIdleDuration;
    bool ranged;
};

enum class EntityTag : uint8_t {
    NONE = 0,
    PLAYER = 1 << 0,
    ENEMY = 1 << 1,
    BULLET = 1 << 2,
    ITEM = 1 << 4,
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
    float damage;
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

    template<typename T>
    const T* get() const {
        for (const auto& effect : effects) {
            if (auto* casted = dynamic_cast<const T*>(effect.get())) {
                return casted;
            }
        }
        return nullptr;
    }
    
    bool hasEffect(BehaviourEffectType type) const {
        for (const auto& effect : effects) {
            if (effect && effect->getType() == type) {
                return true;
            }
        }
        return false;
    }
    
    std::set<BehaviourEffectType> getAllEffectTypes() const {
        std::set<BehaviourEffectType> types;
        for (const auto& effect : effects) {
            if (effect) {
                types.insert(effect->getType());
            }
        }
        return types;
    }
};

struct ItemComponent {
    std::string name;
    std::vector<StatEffect> statEffects;
    std::vector<std::shared_ptr<BehaviourEffectBase>> behaviourEffects;
    bool isPickedUp = false;
};

struct SpriteRendererComponent {
    Texture2D texture;
    Rectangle source;
};

struct BoxColliderComponent {
    float width;
    float height;
    bool isTrigger;
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

struct BulletComponent {
    int damage;
    bool isPiercing;
    std::set<int> hitEntities;
    EntityTag targetTag;
};

#endif

