#include "DoorSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../utils/Helpers.hpp"
#include "../events/EventBus.hpp"
#include "../events/Events.hpp"
#include <cstdio>

void DoorSystem::update(ECS& ecs, EventBus& eventBus) {
    int playerId = Helpers::getPlayerId(ecs);
    
    if (playerId < 0 || !ecs.getHealths().isActive(playerId)) return;
    if (!ecs.getTransforms().isActive(playerId) || !ecs.getRenderables().isActive(playerId)) return;
    
    const auto& playerTransform = ecs.getTransforms().get(playerId);
    const auto& playerRenderable = ecs.getRenderables().get(playerId);
    
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    const auto& transforms = ecs.getTransforms();
    const auto& doors = ecs.getDoors();
    
    for (int i = 0; i < entityCount; i++) {
        if (i == playerId) continue;
        if (!entities[i].active) continue;
        if (!transforms.isActive(i) || !doors.isActive(i)) continue;
        
        const auto& doorTransform = transforms.get(i);
        const auto& doorComponent = doors.get(i);
        
        if (Helpers::checkCircleRectCollision(
            playerTransform.position, playerRenderable.radius,
            doorTransform.position, doorComponent.width, doorComponent.height)) {
            
            DoorCollisionEvent event(i, doorComponent.direction);
            eventBus.emit(event);
        }
    }
}

