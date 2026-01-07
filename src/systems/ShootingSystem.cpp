#include "ShootingSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../factories/BulletFactory.hpp"
#include <cstdio>
#include <raylib.h>

void ShootingSystem::shoot(ECS& ecs){
    ComponentStorage<ShootableComponent> shootables = ecs.getShootables();

    float time = GetTime();

    int i = 0;
    for(bool activeFlag : shootables.getActiveFlags()){
        if (activeFlag){
            ShootableComponent *shootableComponent = ecs.getShootable(i);
            if(shootableComponent->shoot && GetTime() - shootableComponent->lastShootTime > shootableComponent->shootCooldown){
                TransformComponent *transform = ecs.getTransform(i);

                BulletFactory::create(ecs, transform->position, shootableComponent->direction, shootableComponent->shootingSpeed);
                
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