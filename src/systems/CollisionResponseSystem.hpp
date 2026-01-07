#ifndef COLLISION_RESPONSE_SYSTEM_HPP
#define COLLISION_RESPONSE_SYSTEM_HPP

class ECS;

class CollisionResponseSystem {
public:
    static void handleCollision(ECS& ecs, int entityId1, int entityId2);
};

#endif

