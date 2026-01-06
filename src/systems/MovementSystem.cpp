#include "MovementSystem.hpp"
#include "../ecs/Ecs.hpp"
#include <raylib.h>

void MovementSystem::update(ECS& ecs) {
    float deltaTime = GetFrameTime();
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    
    auto& transforms = ecs.getTransforms();
    auto& velocities = ecs.getVelocities();
    
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if (!transforms.isActive(i) || !velocities.isActive(i)) continue;
        
        auto& transform = transforms.get(i);
        auto& velocity = velocities.get(i);
        
        transform.position.x += velocity.velocity.x * deltaTime;
        transform.position.y += velocity.velocity.y * deltaTime;
    }
}

