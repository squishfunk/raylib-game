#ifndef BULLET_SYSTEM_HPP
#define BULLET_SYSTEM_HPP

class ECS;
class Map;
class EventBus;
class CollisionEvent;

class BulletSystem {
    static void handleDestroyBullet(ECS& ecs, CollisionEvent event);
public:
    static void init(ECS& ecs, EventBus& eventBus);
};

#endif // BULLET_SYSTEM_HPP

