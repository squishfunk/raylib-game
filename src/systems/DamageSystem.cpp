#include "DamageSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../events/EventBus.hpp"
#include "../events/Events.hpp"
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
        
        if (ecs.getHealths().isActive(playerId)) {
            auto& playerHealth = ecs.getHealths().get(playerId);
            
            bool canDamage = true;
            if (currentTime - playerHealth.lastDamageTime < DAMAGE_COOLDOWN) {
                canDamage = false;
            }
            
            if (canDamage) {
                playerHealth.healthPoints -= ENEMY_DAMAGE;
                playerHealth.lastDamageTime = currentTime;
                printf("Player Health: %d \n", playerHealth.healthPoints);
            }
        }
    }
    
    if (((tag1 & EntityTag::BULLET) == EntityTag::BULLET && 
         (tag2 & EntityTag::ENEMY) == EntityTag::ENEMY) ||
        ((tag1 & EntityTag::ENEMY) == EntityTag::ENEMY && 
         (tag2 & EntityTag::BULLET) == EntityTag::BULLET)) {
        
        int bulletId = (tag1 & EntityTag::BULLET) == EntityTag::BULLET ? id1 : id2;
        int enemyId = (tag1 & EntityTag::ENEMY) == EntityTag::ENEMY ? id1 : id2;
        
        if (ecs.getHealths().isActive(enemyId)) {
            auto& enemyHealth = ecs.getHealths().get(enemyId);
            
            enemyHealth.healthPoints -= BULLET_DAMAGE;
            enemyHealth.lastDamageTime = currentTime;
            
            ecs.getEntities()[bulletId].active = false;
            
            if (ecs.getAudios().isActive(enemyId)) {
                ecs.getAudio(enemyId)->play("HIT_SOUND");
            }
            
            printf("Enemy id: %d Hit. Current health: %d \n", enemyId, enemyHealth.healthPoints);
        }
    }
}

