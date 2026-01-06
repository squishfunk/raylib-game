#include "Ecs.hpp"

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

void ECS::addVelocity(int entityId, Vector2 velocity) {
    velocities.get(entityId).velocity = velocity;
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

void ECS::addDoor(int entityId, float width, float height, DoorFlags direction) {
    doors.get(entityId).width = width;
    doors.get(entityId).height = height;
    doors.get(entityId).direction = direction;
    doors.setActive(entityId, true);
}

DoorComponent* ECS::getDoor(int entityId) {
    return doors.getPtr(entityId);
}

const DoorComponent* ECS::getDoor(int entityId) const {
    return doors.getPtr(entityId);
}

void ECS::addShootable(int entityId, float shootingRange, float shootingSpeed, float shootCooldown) {
    shootables.get(entityId).lastShootTime = 0.0f;
    shootables.get(entityId).shootingRange = shootingRange;
    shootables.get(entityId).shootingSpeed = shootingSpeed;
    shootables.get(entityId).shootCooldown = shootCooldown;
    shootables.setActive(entityId, true);
}

ShootableComponent* ECS::getShootable(int entityId) {
    return shootables.getPtr(entityId);
}

const ShootableComponent* ECS::getShootable(int entityId) const {
    return shootables.getPtr(entityId);
}

