#ifndef BEHAVIOUR_EFFECTS_HPP
#define BEHAVIOUR_EFFECTS_HPP

#include <memory>

enum class BehaviourEffectType {
    DOUBLE_SHOT,
    TRIPLE_SHOT,
    DRUNK_MOVEMENT,
    PIERCING_SHOT
};

class BehaviourEffectBase {
public:
    virtual ~BehaviourEffectBase() = default;
    virtual BehaviourEffectType getType() const = 0;
};

class DoubleShotEffect : public BehaviourEffectBase {
public:
    float spreadAngle;
    
    DoubleShotEffect(float angle) : spreadAngle(angle) {}
    
    BehaviourEffectType getType() const override {
        return BehaviourEffectType::DOUBLE_SHOT;
    }
};

class TripleShotEffect : public BehaviourEffectBase {
public:
    float spreadAngle;
    
    TripleShotEffect(float angle) : spreadAngle(angle) {}
    
    BehaviourEffectType getType() const override {
        return BehaviourEffectType::TRIPLE_SHOT;
    }
};

class DrunkMovementEffect : public BehaviourEffectBase {
public:
    float slowdownChance;
    float randomDeviation;
    
    DrunkMovementEffect(float chance, float deviation) 
        : slowdownChance(chance), randomDeviation(deviation) {}
    
    BehaviourEffectType getType() const override {
        return BehaviourEffectType::DRUNK_MOVEMENT;
    }
};

class PiercingShotEffect : public BehaviourEffectBase {
public:
    PiercingShotEffect() = default;
    
    BehaviourEffectType getType() const override {
        return BehaviourEffectType::PIERCING_SHOT;
    }
};

#endif

