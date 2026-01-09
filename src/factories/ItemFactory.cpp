#include "ItemFactory.hpp"
#include <raylib.h>
#include <cstdlib>

int ItemFactory::createItemBase(ECS& ecs, Vector2 position, const ItemComponent& itemComponent) {
    int itemId = ecs.createEntity();
    if (itemId >= 0) {
        ecs.addTransform(itemId, position);
        ecs.addItem(itemId, itemComponent);
        ecs.addRenderable(itemId, 15.0f, PURPLE);
        ecs.addCircleCollider(itemId, 15.0f, true);
        ecs.getEntities()[itemId].tags = EntityTag::ITEM;
    }
    return itemId;
}

int ItemFactory::createRandomItem(ECS& ecs, Vector2 position) {
    int itemType = GetRandomValue(0, 7);
    
    switch (itemType) {
        case 0:
            return createSpeedBoostItem(ecs, position, GetRandomValue(50, 150));
        case 1:
            return createHealthBoostItem(ecs, position, GetRandomValue(20, 50));
        case 2:
            return createDamageBoostItem(ecs, position, 1.2f + (GetRandomValue(0, 30) / 100.0f));
        case 3:
            return createFireRateBoostItem(ecs, position, 1.2f + (GetRandomValue(0, 30) / 100.0f));
        case 4:
            return createDoubleShotItem(ecs, position, 0.2f + (GetRandomValue(0, 20) / 100.0f));
        case 5:
            return createTripleShotItem(ecs, position, 0.3f + (GetRandomValue(0, 20) / 100.0f));
        case 6:
            return createDrunkMovementItem(ecs, position, 0.05f + (GetRandomValue(0, 10) / 100.0f), 0.1f + (GetRandomValue(0, 20) / 100.0f));
        case 7:
            return createPiercingShotItem(ecs, position);
        default:
            return createSpeedBoostItem(ecs, position, 100.0f);
    }
}

int ItemFactory::createSpeedBoostItem(ECS& ecs, Vector2 position, float speedBonus) {
    ItemComponent item;
    item.name = "Speed Boost";
    item.statEffects.push_back({StatType::SPEED, EffectType::ADDITIVE, speedBonus});
    return createItemBase(ecs, position, item);
}

int ItemFactory::createHealthBoostItem(ECS& ecs, Vector2 position, int healthBonus) {
    ItemComponent item;
    item.name = "Health Boost";
    item.statEffects.push_back({StatType::HEALTH, EffectType::ADDITIVE, static_cast<float>(healthBonus)});
    return createItemBase(ecs, position, item);
}

int ItemFactory::createDamageBoostItem(ECS& ecs, Vector2 position, float damageMultiplier) {
    ItemComponent item;
    item.name = "Damage Boost";
    item.statEffects.push_back({StatType::DAMAGE, EffectType::MULTIPLICATIVE, damageMultiplier});
    return createItemBase(ecs, position, item);
}

int ItemFactory::createFireRateBoostItem(ECS& ecs, Vector2 position, float fireRateMultiplier) {
    ItemComponent item;
    item.name = "Fire Rate Boost";
    item.statEffects.push_back({StatType::FIRE_RATE, EffectType::MULTIPLICATIVE, fireRateMultiplier});
    return createItemBase(ecs, position, item);
}

int ItemFactory::createDoubleShotItem(ECS& ecs, Vector2 position, float spreadAngle) {
    ItemComponent item;
    item.name = "Double Shot";
    item.behaviourEffects.push_back(std::make_shared<DoubleShotEffect>(spreadAngle));
    return createItemBase(ecs, position, item);
}

int ItemFactory::createTripleShotItem(ECS& ecs, Vector2 position, float spreadAngle) {
    ItemComponent item;
    item.name = "Triple Shot";
    item.behaviourEffects.push_back(std::make_shared<TripleShotEffect>(spreadAngle));
    return createItemBase(ecs, position, item);
}

int ItemFactory::createDrunkMovementItem(ECS& ecs, Vector2 position, float slowdownChance, float randomDeviation) {
    ItemComponent item;
    item.name = "Drunk Movement";
    item.behaviourEffects.push_back(std::make_shared<DrunkMovementEffect>(slowdownChance, randomDeviation));
    return createItemBase(ecs, position, item);
}

int ItemFactory::createPiercingShotItem(ECS& ecs, Vector2 position) {
    ItemComponent item;
    item.name = "Piercing Shot";
    item.behaviourEffects.push_back(std::make_shared<PiercingShotEffect>());
    return createItemBase(ecs, position, item);
}

