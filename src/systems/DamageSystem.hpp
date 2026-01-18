#ifndef DAMAGE_SYSTEM_HPP
#define DAMAGE_SYSTEM_HPP

class ECS;
class EventBus;
struct CollisionEvent;

class DamageSystem {
public:
    static void init(ECS& ecs, EventBus& eventBus);
    static void handleDamage(ECS& ecs, const CollisionEvent& event);
    static void handleEnemyHitPlayer(ECS &ecs, int playerId, int enemyId);
    static void handleBulletHitEntity(ECS &ecs, int bulletId, int victimId);
};

#endif

