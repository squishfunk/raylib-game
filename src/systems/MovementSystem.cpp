#include "MovementSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../components/BehaviourEffects.hpp"
#include <raylib.h>
#include <cmath>
#include <cstdlib>

void MovementSystem::update(ECS& ecs) {
    float deltaTime = GetFrameTime();
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    
    auto& transforms = ecs.getTransforms();
    auto& velocities = ecs.getVelocities();
    const auto& statsManagers = ecs.getStatsManagers();
    const auto& behaviourModifiers = ecs.getBehaviourModifiers();
    
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if (!transforms.isActive(i) || !velocities.isActive(i)) continue;
        
        auto& transform = transforms.get(i);
        auto& velocity = velocities.get(i);
        
        float finalSpeed = velocity.speed;
        if (statsManagers.isActive(i)) {
            finalSpeed = ecs.getStatsManager(i)->getFinalValue(StatType::SPEED, velocity.speed);
        }
        
        Vector2 movementDirection = velocity.velocity;
        
        if (behaviourModifiers.isActive(i)) {
            const auto* drunkEffect = ecs.getBehaviourModifier(i)->get<DrunkMovementEffect>();

            if (drunkEffect) {
                applyDrunkEffect(drunkEffect, &movementDirection, &finalSpeed);
            }
        }
        
        transform.position.x += movementDirection.x * finalSpeed * deltaTime;
        transform.position.y += movementDirection.y * finalSpeed * deltaTime;
    }
}


void MovementSystem::applyDrunkEffect(const DrunkMovementEffect* drunkEffect, Vector2* movementDirection, float* finalSpeed){

    float length = sqrtf(movementDirection->x * movementDirection->x + movementDirection->y * movementDirection->y);
    if (length > 0.05f) {
        movementDirection->x /= length;
        movementDirection->y /= length;
    }
    
    float randomAngle = (GetRandomValue(0, 10000) / 10000.0f) * 2.0f * drunkEffect->randomDeviation - drunkEffect->randomDeviation;
    float cosAngle = cosf(randomAngle);
    float sinAngle = sinf(randomAngle);
    float newX = movementDirection->x * cosAngle - movementDirection->y * sinAngle;
    float newY = movementDirection->x * sinAngle + movementDirection->y * cosAngle;
    movementDirection->x = newX;
    movementDirection->y = newY;
    
    if (GetRandomValue(1, 100) <= drunkEffect->slowdownChance * 100) {
        *finalSpeed *= 0.5f;
    }
}
