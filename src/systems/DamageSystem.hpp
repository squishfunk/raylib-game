#ifndef DAMAGE_SYSTEM_HPP
#define DAMAGE_SYSTEM_HPP

class ECS;
class EventBus;
struct CircleCollisionEvent;

class DamageSystem {
public:
    static void init(ECS& ecs, EventBus& eventBus);
    static void handleDamage(ECS& ecs, const CircleCollisionEvent& event);
    static void handleEnemyHitPlayer(ECS &ecs, int playerId, int enemyId);
    static void handlePlayerBulletHitEnemy(ECS &ecs, int bulletId, int enemyId);
    static void handleEnemyBulletHitPlayer(ECS &ecs, int bulletId, int playerId);
};

#endif

