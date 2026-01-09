#include "ShootingSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../factories/BulletFactory.hpp"
#include <cstdio>
#include <raylib.h>

void ShootingSystem::shoot(ECS& ecs){
    ComponentStorage<ShootableComponent> &shootables = ecs.getShootables();
    
    const auto& entities = ecs.getEntities();

    float time = GetTime();

    auto &activeFlags = shootables.getActiveFlags();

    int i = 0;
    for(bool activeFlag : activeFlags){
        if (activeFlag && entities[i].active){

            ShootableComponent *shootableComponent = ecs.getShootable(i);
            if(shootableComponent->shoot && GetTime() - shootableComponent->lastShootTime > shootableComponent->shootCooldown){
                TransformComponent *transform = ecs.getTransform(i);

                bool isPlayerShooting = true;
                if((entities[i].tags & EntityTag::ENEMY) == EntityTag::ENEMY){
                    isPlayerShooting = false;
                }

                BulletFactory::create(ecs, transform->position, shootableComponent->direction, shootableComponent->shootingSpeed, isPlayerShooting);
                
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