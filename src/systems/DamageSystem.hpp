#ifndef DAMAGE_SYSTEM_HPP
#define DAMAGE_SYSTEM_HPP

class ECS;
class EventBus;
struct CircleCollisionEvent;

class DamageSystem {
public:
    static void init(ECS& ecs, EventBus& eventBus);
    static void handleDamage(ECS& ecs, const CircleCollisionEvent& event);
};

#endif

