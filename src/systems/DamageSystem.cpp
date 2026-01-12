#include "DamageSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../events/EventBus.hpp"
#include "../events/Events.hpp"
#include "DebugSystem.hpp"
#include <raylib.h>
#include <cstdio>

void DamageSystem::init(ECS& ecs, EventBus& eventBus) {
    eventBus.subscribe<CircleCollisionEvent>([&ecs](const CircleCollisionEvent& event) {
        handleDamage(ecs, event);
    });
}

void DamageSystem::handleDamage(ECS& ecs, const CircleCollisionEvent& event) {
    const auto& entities = ecs.getEntities();
    int id1 = event.entityId1;
    int id2 = event.entityId2;
    
    if (id1 < 0 || id2 < 0 || id1 >= MAX_ENTITIES || id2 >= MAX_ENTITIES) return;
    if (!entities[id1].active || !entities[id2].active) return;
    
    EntityTag tag1 = entities[id1].tags;
    EntityTag tag2 = entities[id2].tags;

    bool isBulletCollision = false;
    if(ecs.getBullets().isActive(id1) || ecs.getBullets().isActive(id2)){
        isBulletCollision = true;
    }
    
    if(isBulletCollision){
        int bulletId = ecs.getBullets().isActive(id1) ? id1 : id2;
        EntityTag bulletTarget = ecs.getBullets().get(bulletId).targetTag;
        int victimId = bulletId == id1 ? id2 : id1;

        if ((entities[victimId].tags & bulletTarget) == bulletTarget){
            DamageSystem::handleBulletHitEntity(ecs, bulletId, victimId);
        }
    }else{
        if (((tag1 & EntityTag::PLAYER) == EntityTag::PLAYER && 
            (tag2 & EntityTag::ENEMY) == EntityTag::ENEMY) ||
            ((tag1 & EntityTag::ENEMY) == EntityTag::ENEMY && 
            (tag2 & EntityTag::PLAYER) == EntityTag::PLAYER)) {
        
        int playerId = (tag1 & EntityTag::PLAYER) == EntityTag::PLAYER ? id1 : id2;
        int enemyId = (tag1 & EntityTag::ENEMY) == EntityTag::ENEMY ? id1 : id2;
        
        DamageSystem::handleEnemyHitPlayer(ecs, playerId, enemyId);
   }
    }
}


void DamageSystem::handleEnemyHitPlayer(ECS &ecs, int playerId, int enemyId){
    float currentTime = GetTime();

    if (ecs.getHealths().isActive(playerId)) {
        auto& playerHealth = ecs.getHealths().get(playerId);
        auto& enemyComponent = ecs.getEnemies().get(enemyId);
        
        bool canDamage = true;
        if (currentTime - playerHealth.lastDamageTime < DAMAGE_COOLDOWN) {
            canDamage = false;
        }
        
        if (canDamage) {
            playerHealth.healthPoints -= enemyComponent.damage;
            playerHealth.lastDamageTime = currentTime;
        }
    }
}

void DamageSystem::handleBulletHitEntity(ECS &ecs, int bulletId, int victimId){
    float currentTime = GetTime();
    auto* bullet = ecs.getBullet(bulletId);
    auto* victimHealth = ecs.getHealth(victimId);
    if(!bullet) return;
    if(!victimHealth) return;
    if(bullet->hitEntities.count(victimId)) return;
    
    victimHealth->healthPoints -= bullet->damage;
    victimHealth->lastDamageTime = currentTime;

    bullet->hitEntities.insert(victimId);
    
    const auto& bullets = ecs.getBullets();
    if (!bullets.get(bulletId).isPiercing) {
        ecs.getEntities()[bulletId].active = false;
    }
    
    if (ecs.getAudios().isActive(victimId) && ecs.getAudio(victimId)->hasSound("HIT_SOUND")) {
        ecs.getAudio(victimId)->play("HIT_SOUND");
    }
}