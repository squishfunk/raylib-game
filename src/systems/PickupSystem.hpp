#ifndef PICKUP_SYSTEM_HPP
#define PICKUP_SYSTEM_HPP

class ECS;
class EventBus;

class PickupSystem {
public:
    static void update(ECS& ecs, EventBus& eventBus);
};

#endif

