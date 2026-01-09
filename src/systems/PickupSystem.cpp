#include "PickupSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../events/EventBus.hpp"
#include "../events/Events.hpp"
#include "../utils/Helpers.hpp"

void PickupSystem::update(ECS& ecs, EventBus& eventBus) {
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    const auto& transforms = ecs.getTransforms();
    const auto& colliders = ecs.getCircleColliders();
    const auto& items = ecs.getItems();
    
    int playerId = Helpers::getPlayerId(ecs);
    
    if (playerId < 0) return;
    
    if (!transforms.isActive(playerId) || !colliders.isActive(playerId)) return;
    
    const auto& playerTransform = transforms.get(playerId);
    const auto& playerCollider = colliders.get(playerId);
    
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if ((entities[i].tags & EntityTag::ITEM) != EntityTag::ITEM) continue;
        if (!items.isActive(i)) continue;
        
        const auto& item = items.get(i);
        if (item.isPickedUp) continue;
        
        if (!transforms.isActive(i) || !colliders.isActive(i)) continue;
        
        const auto& itemTransform = transforms.get(i);
        const auto& itemCollider = colliders.get(i);
        
        // Sprawdź kolizję
        if (Helpers::checkCircleCollision(
            playerTransform.position, playerCollider.radius,
            itemTransform.position, itemCollider.radius)) {
            
            // Emituj event z nazwą itemu
            std::string itemName = item.name.empty() ? "Item" : item.name;
            ItemPickupEvent event(playerId, i, itemName);
            eventBus.emit(event);
        }
    }
}

