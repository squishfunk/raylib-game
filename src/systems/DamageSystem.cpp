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
    
    float currentTime = GetTime();
    
    if (((tag1 & EntityTag::PLAYER) == EntityTag::PLAYER && 
         (tag2 & EntityTag::ENEMY) == EntityTag::ENEMY) ||
        ((tag1 & EntityTag::ENEMY) == EntityTag::ENEMY && 
         (tag2 & EntityTag::PLAYER) == EntityTag::PLAYER)) {
        
        int playerId = (tag1 & EntityTag::PLAYER) == EntityTag::PLAYER ? id1 : id2;
        
        DamageSystem::handleEnemyHitPlayer(ecs, playerId);
    }
    

    if (((tag1 & EntityTag::BULLET) == EntityTag::BULLET && 
         (tag2 & EntityTag::ENEMY) == EntityTag::ENEMY) ||
        ((tag1 & EntityTag::ENEMY) == EntityTag::ENEMY && 
         (tag2 & EntityTag::BULLET) == EntityTag::BULLET)) 
    {
        int bulletId = (tag1 & EntityTag::BULLET) == EntityTag::BULLET ? id1 : id2;
        int enemyId = (tag1 & EntityTag::ENEMY) == EntityTag::ENEMY ? id1 : id2;
        DamageSystem::handlePlayerBulletHitEnemy(ecs, bulletId, enemyId);
    }

    if (((tag1 & EntityTag::ENEMY_BULLET) == EntityTag::ENEMY_BULLET && 
        (tag2 & EntityTag::PLAYER) == EntityTag::PLAYER) ||
        ((tag1 & EntityTag::PLAYER) == EntityTag::PLAYER && 
        (tag2 & EntityTag::ENEMY_BULLET) == EntityTag::ENEMY_BULLET)) 
    {
        int bulletId = (tag1 & EntityTag::ENEMY_BULLET) == EntityTag::ENEMY_BULLET ? id1 : id2;
        int playerId = (tag1 & EntityTag::PLAYER) == EntityTag::PLAYER ? id1 : id2;
        DamageSystem::handleEnemyBulletHitPlayer(ecs, bulletId, playerId);
    }
}


void DamageSystem::handleEnemyHitPlayer(ECS &ecs, int playerId){
    float currentTime = GetTime();

    if (ecs.getHealths().isActive(playerId)) {
        auto& playerHealth = ecs.getHealths().get(playerId);
        
        bool canDamage = true;
        if (currentTime - playerHealth.lastDamageTime < DAMAGE_COOLDOWN) {
            canDamage = false;
        }
        
        if (canDamage) {
            playerHealth.healthPoints -= ENEMY_DAMAGE;
            playerHealth.lastDamageTime = currentTime;
        }
    }
}

void DamageSystem::handlePlayerBulletHitEnemy(ECS &ecs, int bulletId, int enemyId){
    float currentTime = GetTime();
    
    if (ecs.getHealths().isActive(enemyId)) {
        auto& enemyHealth = ecs.getHealths().get(enemyId);
        
        enemyHealth.healthPoints -= BULLET_DAMAGE;
        enemyHealth.lastDamageTime = currentTime;
        
        const auto& entities = ecs.getEntities();
        if ((entities[bulletId].tags & EntityTag::PIERCING_BULLET) != EntityTag::PIERCING_BULLET) {
            ecs.getEntities()[bulletId].active = false;
        }
        
        if (ecs.getAudios().isActive(enemyId)) {
            ecs.getAudio(enemyId)->play("HIT_SOUND");
        }
    }
}

void DamageSystem::handleEnemyBulletHitPlayer(ECS &ecs, int bulletId, int playerId){
    float currentTime = GetTime();
    
    if (ecs.getHealths().isActive(playerId)) {
        auto& enemyHealth = ecs.getHealths().get(playerId);
        
        enemyHealth.healthPoints -= BULLET_DAMAGE;
        enemyHealth.lastDamageTime = currentTime;
        
        ecs.getEntities()[bulletId].active = false;
        
        if (ecs.getAudios().isActive(playerId)) {
            ecs.getAudio(playerId)->play("HIT_SOUND");
        }
    }
}