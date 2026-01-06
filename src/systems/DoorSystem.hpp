#ifndef DOOR_SYSTEM_HPP
#define DOOR_SYSTEM_HPP

class ECS;
class EventBus;

class DoorSystem {
public:
    static void update(ECS& ecs, EventBus& eventBus);
};

#endif // DOOR_SYSTEM_HPP

