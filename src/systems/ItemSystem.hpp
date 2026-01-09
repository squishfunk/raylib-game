#ifndef ITEM_SYSTEM_HPP
#define ITEM_SYSTEM_HPP

class ECS;
class EventBus;

class ItemSystem {
public:
    static void init(ECS& ecs, EventBus& eventBus);
};

#endif

