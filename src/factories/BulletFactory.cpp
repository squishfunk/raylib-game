#include "BulletFactory.hpp"

int BulletFactory::create(ECS& ecs, Vector2 position, Vector2 velocity, float speed){
    int bulletId = ecs.createEntity();
    if (bulletId >= 0) {
        ecs.addTransform(bulletId, position);
        ecs.addVelocity(bulletId, velocity, speed);
        ecs.addRenderable(bulletId, BULLET_RADIUS, ORANGE);
        ecs.addCircleCollider(bulletId, BULLET_RADIUS, true);
        ecs.getEntities()[bulletId].tags = EntityTag::BULLET;
    }
    return bulletId;
}