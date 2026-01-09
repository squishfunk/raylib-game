#pragma once

#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../components/BehaviourEffects.hpp"
#include <memory>

class ItemFactory {
public:
    static int createRandomItem(ECS& ecs, Vector2 position);
    
    static int createSpeedBoostItem(ECS& ecs, Vector2 position, float speedBonus);
    static int createHealthBoostItem(ECS& ecs, Vector2 position, int healthBonus);
    static int createDamageBoostItem(ECS& ecs, Vector2 position, float damageMultiplier);
    static int createFireRateBoostItem(ECS& ecs, Vector2 position, float fireRateMultiplier);
    static int createDoubleShotItem(ECS& ecs, Vector2 position, float spreadAngle = 0.3f);
    static int createTripleShotItem(ECS& ecs, Vector2 position, float spreadAngle = 0.4f);
    static int createDrunkMovementItem(ECS& ecs, Vector2 position, float slowdownChance = 0.1f, float randomDeviation = 0.2f);
    static int createPiercingShotItem(ECS& ecs, Vector2 position);
    
private:
    static int createItemBase(ECS& ecs, Vector2 position, const ItemComponent& itemComponent);
};

