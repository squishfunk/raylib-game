#include "Ecs.hpp"
#include "components/Components.hpp"
#include <raylib.h>

int ECS::createEntity() {
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) {
            entities[i] = Entity();
            entities[i].active = true;
            
            transforms.setActive(i, false);
            velocities.setActive(i, false);
            renderables.setActive(i, false);
            healths.setActive(i, false);
            enemies.setActive(i, false);
            doors.setActive(i, false);
            shootables.setActive(i, false);
            audios.setActive(i, false);
            circleColliders.setActive(i, false);
            statsManagers.setActive(i, false);
            behaviourModifiers.setActive(i, false);
            items.setActive(i, false);
            bullets.setActive(i, false);
            return i;
        }
    }
    
    if (entityCount >= MAX_ENTITIES) {
        return -1;
    }
    
    int entityId = entityCount++;
    entities[entityId] = Entity();
    entities[entityId].active = true;
    
    transforms.setActive(entityId, false);
    velocities.setActive(entityId, false);
    renderables.setActive(entityId, false);
    healths.setActive(entityId, false);
    enemies.setActive(entityId, false);
    doors.setActive(entityId, false);
    shootables.setActive(entityId, false);
    audios.setActive(entityId, false);
    circleColliders.setActive(entityId, false);
    statsManagers.setActive(entityId, false);
    behaviourModifiers.setActive(entityId, false);
    items.setActive(entityId, false);
    items.setActive(entityId, false);
    
    return entityId;
}

void ECS::destroyEntity(int entityId) {
    if (entityId < 0 || entityId >= entityCount) return;
    
    entities[entityId].active = false;
    
    transforms.setActive(entityId, false);
    velocities.setActive(entityId, false);
    renderables.setActive(entityId, false);
    healths.setActive(entityId, false);
    enemies.setActive(entityId, false);
    doors.setActive(entityId, false);
    shootables.setActive(entityId, false);
    audios.setActive(entityId, false);
    circleColliders.setActive(entityId, false);
    statsManagers.setActive(entityId, false);
    behaviourModifiers.setActive(entityId, false);
    items.setActive(entityId, false);
    bullets.setActive(entityId, false);
}

void ECS::addTransform(int entityId, Vector2 position) {
    transforms.get(entityId).position = position;
    transforms.setActive(entityId, true);
}

TransformComponent* ECS::getTransform(int entityId) {
    return transforms.getPtr(entityId);
}

const TransformComponent* ECS::getTransform(int entityId) const {
    return transforms.getPtr(entityId);
}

void ECS::addVelocity(int entityId, Vector2 velocity, float speed) {
    velocities.get(entityId).velocity = velocity;
    velocities.get(entityId).speed = speed;
    velocities.setActive(entityId, true);
}

VelocityComponent* ECS::getVelocity(int entityId) {
    return velocities.getPtr(entityId);
}

const VelocityComponent* ECS::getVelocity(int entityId) const {
    return velocities.getPtr(entityId);
}

void ECS::addRenderable(int entityId, float radius, Color color) {
    renderables.get(entityId).radius = radius;
    renderables.get(entityId).color = color;
    renderables.setActive(entityId, true);
}

RenderableComponent* ECS::getRenderable(int entityId) {
    return renderables.getPtr(entityId);
}

const RenderableComponent* ECS::getRenderable(int entityId) const {
    return renderables.getPtr(entityId);
}

void ECS::addHealth(int entityId, int initialHealthPoints, int maxHealthPoints) {
    healths.get(entityId).healthPoints = initialHealthPoints;
    healths.get(entityId).maxHealthPoints = maxHealthPoints;
    healths.get(entityId).lastDamageTime = 0.0f;
    healths.setActive(entityId, true);
}

HealthComponent* ECS::getHealth(int entityId) {
    return healths.getPtr(entityId);
}

const HealthComponent* ECS::getHealth(int entityId) const {
    return healths.getPtr(entityId);
}

void ECS::addEnemy(int entityId, const EnemyComponent& enemyComponent) {
    enemies.get(entityId) = enemyComponent;
    enemies.setActive(entityId, true);
}

EnemyComponent* ECS::getEnemy(int entityId) {
    return enemies.getPtr(entityId);
}

const EnemyComponent* ECS::getEnemy(int entityId) const {
    return enemies.getPtr(entityId);
}

void ECS::addDoor(int entityId, float width, float height, DoorFlags direction, bool opened) {
    doors.get(entityId).width = width;
    doors.get(entityId).height = height;
    doors.get(entityId).direction = direction;
    doors.get(entityId).opened = opened;
    doors.setActive(entityId, true);
}

DoorComponent* ECS::getDoor(int entityId) {
    return doors.getPtr(entityId);
}

const DoorComponent* ECS::getDoor(int entityId) const {
    return doors.getPtr(entityId);
}

void ECS::addShootable(int entityId, float shootingRange, float shootingSpeed, float shootCooldown) {
    shootables.get(entityId).lastShootTime = (float) GetTime();
    shootables.get(entityId).shootingRange = shootingRange;
    shootables.get(entityId).shootingSpeed = shootingSpeed;
    shootables.get(entityId).shootCooldown = shootCooldown;
    shootables.get(entityId).damage = 1;
    shootables.get(entityId).shoot = false;
    shootables.setActive(entityId, true);
}

ShootableComponent* ECS::getShootable(int entityId) {
    return shootables.getPtr(entityId);
}

const ShootableComponent* ECS::getShootable(int entityId) const {
    return shootables.getPtr(entityId);
}

void ECS::addAudio(int entityId, const std::string& soundKey, Sound sound) {
    audios.get(entityId).addSound(soundKey, sound);
    audios.setActive(entityId, true);
}

AudioComponent* ECS::getAudio(int entityId) {
    return audios.getPtr(entityId);
}

const AudioComponent* ECS::getAudio(int entityId) const {
    return audios.getPtr(entityId);
}

void ECS::addCircleCollider(int entityId, float radius, bool isTrigger) {
    if (entityId >= 0 && entityId < MAX_ENTITIES) {
        circleColliders.get(entityId).radius = radius;
        circleColliders.get(entityId).isTrigger = isTrigger;
        circleColliders.setActive(entityId, true);
    }
}

CircleColliderComponent* ECS::getCircleCollider(int entityId) {
    return circleColliders.getPtr(entityId);
}

const CircleColliderComponent* ECS::getCircleCollider(int entityId) const {
    return circleColliders.getPtr(entityId);
}

void ECS::addStatsManager(int entityId) {
    statsManagers.setActive(entityId, true);
}

StatsManagerComponent* ECS::getStatsManager(int entityId) {
    return statsManagers.getPtr(entityId);
}

const StatsManagerComponent* ECS::getStatsManager(int entityId) const {
    return statsManagers.getPtr(entityId);
}

void ECS::addBehaviourModifier(int entityId) {
    behaviourModifiers.setActive(entityId, true);
}

BehaviourModifierComponent* ECS::getBehaviourModifier(int entityId) {
    return behaviourModifiers.getPtr(entityId);
}

const BehaviourModifierComponent* ECS::getBehaviourModifier(int entityId) const {
    return behaviourModifiers.getPtr(entityId);
}

void ECS::addItem(int entityId, const ItemComponent& itemComponent) {
    items.get(entityId) = itemComponent;
    items.setActive(entityId, true);
}

ItemComponent* ECS::getItem(int entityId) {
    return items.getPtr(entityId);
}

const ItemComponent* ECS::getItem(int entityId) const {
    return items.getPtr(entityId);
}

void ECS::addBullet(int entityId, const BulletComponent& enemyComponent) {
    bullets.get(entityId) = enemyComponent;
    bullets.setActive(entityId, true);
}

BulletComponent* ECS::getBullet(int entityId) {
    return bullets.getPtr(entityId);
}

const BulletComponent* ECS::getBullet(int entityId) const {
    return bullets.getPtr(entityId);
}