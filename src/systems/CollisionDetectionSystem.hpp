#ifndef COLLISION_DETECTION_SYSTEM_HPP
#define COLLISION_DETECTION_SYSTEM_HPP

class ECS;
class EventBus;

class CollisionDetectionSystem {
public:
    static void update(ECS& ecs, EventBus& eventBus);
};

#endif

