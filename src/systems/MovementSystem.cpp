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
        
        // Oblicz finalną prędkość używając StatsManagerComponent
        float finalSpeed = velocity.speed;
        if (statsManagers.isActive(i)) {
            const auto* statsManager = ecs.getStatsManager(i);
            if (statsManager) {
                finalSpeed = statsManager->getFinalValue(StatType::SPEED, velocity.speed);
            }
        }
        
        // Aplikuj DrunkMovementEffect jeśli istnieje
        Vector2 movementDirection = velocity.velocity;
        bool hasDrunkMovement = false;
        float randomDeviation = 0.0f;
        float slowdownChance = 0.0f;
        
        if (behaviourModifiers.isActive(i)) {
            const auto* behaviourModifier = ecs.getBehaviourModifier(i);
            if (behaviourModifier) {
                for (const auto& effect : behaviourModifier->effects) {
                    if (effect && effect->getType() == BehaviourEffectType::DRUNK_MOVEMENT) {
                        hasDrunkMovement = true;
                        auto* drunkEffect = dynamic_cast<DrunkMovementEffect*>(effect.get());
                        if (drunkEffect) {
                            randomDeviation = drunkEffect->randomDeviation;
                            slowdownChance = drunkEffect->slowdownChance;
                        }
                        break;
                    }
                }
            }
        }
        
        // Aplikuj losowe odchylenia dla DrunkMovementEffect
        if (hasDrunkMovement && (movementDirection.x != 0.0f || movementDirection.y != 0.0f)) {
            // Normalizuj kierunek
            float length = sqrtf(movementDirection.x * movementDirection.x + movementDirection.y * movementDirection.y);
            if (length > 0.0f) {
                movementDirection.x /= length;
                movementDirection.y /= length;
            }
            
            // Dodaj losowe odchylenie
            float randomAngle = ((float)rand() / RAND_MAX) * 2.0f * randomDeviation - randomDeviation;
            float cosAngle = cosf(randomAngle);
            float sinAngle = sinf(randomAngle);
            float newX = movementDirection.x * cosAngle - movementDirection.y * sinAngle;
            float newY = movementDirection.x * sinAngle + movementDirection.y * cosAngle;
            movementDirection.x = newX;
            movementDirection.y = newY;
            
            // Sprawdź szansę spowolnienia
            if (((float)rand() / RAND_MAX) < slowdownChance) {
                finalSpeed *= 0.5f;  // Spowolnij o 50%
            }
        }
        
        transform.position.x += movementDirection.x * finalSpeed * deltaTime;
        transform.position.y += movementDirection.y * finalSpeed * deltaTime;
    }
}

