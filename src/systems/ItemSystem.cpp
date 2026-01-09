#include "ItemSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../events/EventBus.hpp"
#include "../events/Events.hpp"
#include "../components/Components.hpp"
#include "RenderSystem.hpp"
#include <raylib.h>

void ItemSystem::init(ECS& ecs, EventBus& eventBus) {
    eventBus.subscribe<ItemPickupEvent>([&ecs](const ItemPickupEvent& event) {
        int playerId = event.playerEntityId;
        int itemId = event.itemEntityId;
        
        ItemComponent* item = ecs.getItem(itemId);
        if (!item || item->isPickedUp) {
            return;
        }
        
        StatsManagerComponent* statsManager = ecs.getStatsManager(playerId);
        BehaviourModifierComponent* behaviourModifier = ecs.getBehaviourModifier(playerId);
        
        if (!statsManager) {
            ecs.addStatsManager(playerId);
            statsManager = ecs.getStatsManager(playerId);
        }
        
        if (!behaviourModifier) {
            ecs.addBehaviourModifier(playerId);
            behaviourModifier = ecs.getBehaviourModifier(playerId);
        }
        
        if (!statsManager || !behaviourModifier) {
            return;
        }
        
        for (const auto& statEffect : item->statEffects) {
            statsManager->addEffect(statEffect);
        }
        
        for (const auto& behaviourEffect : item->behaviourEffects) {
            behaviourModifier->addEffect(behaviourEffect);
        }
        
        item->isPickedUp = true;
        
        RenderSystem::setPickedUpItemName(item->name);
        
        ecs.destroyEntity(itemId);
    });
}

