#include "ShootingSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../factories/BulletFactory.hpp"
#include "../components/Components.hpp"
#include "../components/BehaviourEffects.hpp"
#include <cassert>
#include <cstdio>
#include <raylib.h>
#include <cmath>

void ShootingSystem::shoot(ECS& ecs){
    ComponentStorage<ShootableComponent> &shootables = ecs.getShootables();
    
    const auto& entities = ecs.getEntities();
    const auto& statsManagers = ecs.getStatsManagers();
    const auto& behaviourModifiers = ecs.getBehaviourModifiers();

    float time = GetTime();

    auto &activeFlags = shootables.getActiveFlags();

    int i = 0;
    for(bool activeFlag : activeFlags){
        if (activeFlag && entities[i].active){

            ShootableComponent *shootableComponent = ecs.getShootable(i);
            
            float finalCooldown = shootableComponent->shootCooldown;
            if (statsManagers.isActive(i)) {
                const auto* statsManager = ecs.getStatsManager(i);
                if (statsManager) {
                    float baseFireRate = 1.0f / shootableComponent->shootCooldown;
                    float finalFireRate = statsManager->getFinalValue(StatType::FIRE_RATE, baseFireRate);
                    if (finalFireRate > 0.0f) {
                        finalCooldown = 1.0f / finalFireRate;
                    }
                }
            }
            
            if(shootableComponent->shoot && GetTime() - shootableComponent->lastShootTime > finalCooldown){
                TransformComponent *transform = ecs.getTransform(i);

                bool isPlayerShooting = true;
                if((entities[i].tags & EntityTag::ENEMY) == EntityTag::ENEMY){
                    isPlayerShooting = false;
                }

                Vector2 baseDirection = shootableComponent->direction;
                float baseSpeed = shootableComponent->shootingSpeed;
                
                bool hasPiercing = false;
                std::vector<Vector2> shotDirections;
                
                if (behaviourModifiers.isActive(i) && isPlayerShooting) {
                    const auto* behaviourModifier = ecs.getBehaviourModifier(i);
                    assert(behaviourModifier && "The flag of BehaviourModifier is active and pointer is null");
                    
                    for (const auto& effect : behaviourModifier->effects) {
                        if (!effect) continue;
                        
                        BehaviourEffectType effectType = effect->getType();
                        
                        if (effectType == BehaviourEffectType::DOUBLE_SHOT) {
                            auto* doubleShot = dynamic_cast<DoubleShotEffect*>(effect.get());
                            if (doubleShot) {
                                float angle = doubleShot->spreadAngle;
                                float length = sqrtf(baseDirection.x * baseDirection.x + baseDirection.y * baseDirection.y);
                                if (length > 0.0f) {
                                    Vector2 normalized = {baseDirection.x / length, baseDirection.y / length};
                                    float baseAngle = atan2f(normalized.y, normalized.x);
                                    shotDirections.push_back({cosf(baseAngle - angle/2.0f), sinf(baseAngle - angle/2.0f)});
                                    shotDirections.push_back({cosf(baseAngle + angle/2.0f), sinf(baseAngle + angle/2.0f)});
                                }
                            }
                        } else if (effectType == BehaviourEffectType::TRIPLE_SHOT) {
                            auto* tripleShot = dynamic_cast<TripleShotEffect*>(effect.get());
                            if (tripleShot) {
                                float angle = tripleShot->spreadAngle;
                                float length = sqrtf(baseDirection.x * baseDirection.x + baseDirection.y * baseDirection.y);
                                if (length > 0.0f) {
                                    Vector2 normalized = {baseDirection.x / length, baseDirection.y / length};
                                    float baseAngle = atan2f(normalized.y, normalized.x);
                                    shotDirections.push_back({cosf(baseAngle - angle), sinf(baseAngle - angle)});
                                    shotDirections.push_back(normalized);
                                    shotDirections.push_back({cosf(baseAngle + angle), sinf(baseAngle + angle)});
                                }
                            }
                        } else if (effectType == BehaviourEffectType::PIERCING_SHOT) {
                            hasPiercing = true;
                        }
                    }
                }
                
                if (shotDirections.empty()) {
                    shotDirections.push_back(baseDirection);
                }
                
                for (const auto& direction : shotDirections) {
                    BulletFactory::create(ecs, transform->position, direction, baseSpeed, isPlayerShooting, hasPiercing);
                }
                
                shootableComponent->shoot = false;
                shootableComponent->direction = {0,0};
                shootableComponent->lastShootTime = time;
            }
        }
        i++;
    }
}

void ShootingSystem::update(ECS& ecs){
    shoot(ecs);
}