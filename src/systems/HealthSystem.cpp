#include "HealthSystem.hpp"
#include "../ecs/Ecs.hpp"
#include <cstdio>

void HealthSystem::update(ECS& ecs) {
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    auto& healths = ecs.getHealths();
    
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if (!healths.isActive(i)) continue;
        
        if (healths.get(i).healthPoints <= 0) {
            ecs.getEntities()[i].active = false;
            printf("Entity %d died!\n", i);
        }
    }
}

