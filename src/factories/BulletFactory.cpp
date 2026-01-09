#include "BulletFactory.hpp"
#include <cstdio>
#include <raylib.h>

int BulletFactory::create(ECS& ecs, Vector2 position, Vector2 velocity, float speed, bool isPlayerShooting){
    int bulletId = ecs.createEntity();
    if (bulletId >= 0) {
        EntityTag bulletTag;
        Color color;

        if(isPlayerShooting){
            bulletTag = EntityTag::BULLET;
            color = ORANGE;
        }else{
            bulletTag = EntityTag::ENEMY_BULLET;
            color = RED;
        }

        ecs.addTransform(bulletId, position);
        ecs.addVelocity(bulletId, velocity, speed);
        ecs.addRenderable(bulletId, BULLET_RADIUS, color);
        ecs.addCircleCollider(bulletId, BULLET_RADIUS, true);

        ecs.getEntities()[bulletId].tags = bulletTag;
    }
    return bulletId;
}