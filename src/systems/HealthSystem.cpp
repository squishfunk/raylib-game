#include "HealthSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include <cstdio>
#include <raylib.h>

void HealthSystem::update(ECS& ecs) {
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    auto& healths = ecs.getHealths();
    auto& audios = ecs.getAudios();
    
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if (!healths.isActive(i)) continue;
        
        if (healths.get(i).healthPoints <= 0) {
            // Play death sound for enemies
            if ((entities[i].tags & EntityTag::ENEMY) == EntityTag::ENEMY && audios.isActive(i)) {
                audios.get(i).play("DIE_SOUND");
            }
            
            ecs.getEntities()[i].active = false;
            printf("Entity %d died!\n", i);
        }
    }
}

