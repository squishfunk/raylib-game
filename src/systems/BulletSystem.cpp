#include "BulletSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "events/Events.hpp"
#include "events/EventBus.hpp"
#include <raylib.h>

void BulletSystem::handleDestroyBullet(ECS& ecs, CollisionEvent event) {
    const auto& entities = ecs.getEntities();
    int id1 = event.entityId1;
    int id2 = event.entityId2;
    
    if (id1 < 0 || id2 < 0 || id1 >= MAX_ENTITIES || id2 >= MAX_ENTITIES) return;
    if (!entities[id1].active || !entities[id2].active) return;
    
    bool isBullet1 = ecs.getBullets().isActive(id1);
    bool isBullet2 = ecs.getBullets().isActive(id2);
    
    if (!isBullet1 && !isBullet2) return;
    
    int bulletId = isBullet1 ? id1 : id2;
    int otherId = isBullet1 ? id2 : id1;
    
    const auto& bullet = ecs.getBullets().get(bulletId);
    EntityTag bulletTarget = bullet.targetTag;
    
    bool isTarget = (entities[otherId].tags & bulletTarget) == bulletTarget;
    
    if (ecs.getBoxColliders().isActive(otherId) && !isTarget) {
        ecs.getEntities()[bulletId].active = false;
    }
}



void BulletSystem::init(ECS& ecs, EventBus& eventBus) {
    /*  TODO in future change Event type to BulletWallCollisionEvent */
    eventBus.subscribe<CollisionEvent>([&ecs](const CollisionEvent& event) {
        handleDestroyBullet(ecs, event);
    });
}