#include "BulletFactory.hpp"
#include "components/Components.hpp"
#include <cstdio>
#include <raylib.h>

int BulletFactory::create(ECS& ecs, Vector2 position, Vector2 velocity, float speed, bool isPlayerShooting, bool isPiercing){
    int bulletId = ecs.createEntity();
    if (bulletId >= 0) {
        Color color;
        BulletComponent bullet = {1,isPiercing, {}, {}};
        
        if(isPlayerShooting){
            bullet.targetTag = EntityTag::ENEMY;
            color = ORANGE;
        }else{
            bullet.targetTag = EntityTag::PLAYER;
            color = RED;
        }

        ecs.addTransform(bulletId, position);
        ecs.addVelocity(bulletId, velocity, speed);
        ecs.addRenderable(bulletId, BULLET_RADIUS, color);
        ecs.addBullet(bulletId,  bullet);
        ecs.addCircleCollider(bulletId, BULLET_RADIUS, true);
    }
    return bulletId;
}