#ifndef ECS_HPP
#define ECS_HPP

#include "../components/components.hpp"
#include <array>

class Game;

struct Entity {
    bool active;
    EntityTag tags;
    
    Entity() : active(false), tags(EntityTag::NONE) {}
};

template<typename T>
class ComponentStorage {
private:
    std::array<T, MAX_ENTITIES> data;
    std::array<bool, MAX_ENTITIES> active_flags;
    
public:
    ComponentStorage() {
        active_flags.fill(false);
    }
    
    void setActive(int entityId, bool value) {
        if (entityId >= 0 && entityId < MAX_ENTITIES) {
            active_flags[entityId] = value;
        }
    }
    
    bool isActive(int entityId) const {
        if (entityId >= 0 && entityId < MAX_ENTITIES) {
            return active_flags[entityId];
        }
        return false;
    }
    
    T& get(int entityId) {
        return data[entityId];
    }
    
    const T& get(int entityId) const {
        return data[entityId];
    }
    
    T* getPtr(int entityId) {
        if (entityId >= 0 && entityId < MAX_ENTITIES && active_flags[entityId]) {
            return &data[entityId];
        }
        return nullptr;
    }
    
    const T* getPtr(int entityId) const {
        if (entityId >= 0 && entityId < MAX_ENTITIES && active_flags[entityId]) {
            return &data[entityId];
        }
        return nullptr;
    }
};

class ECS {
private:
    std::array<Entity, MAX_ENTITIES> entities;
    int entityCount;
    
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<VelocityComponent> velocities;
    ComponentStorage<RenderableComponent> renderables;
    ComponentStorage<HealthComponent> healths;
    ComponentStorage<EnemyComponent> enemies;
    
public:
    ECS() : entityCount(0) {
        entities.fill(Entity());
    }
    
    int createEntity();
    void destroyEntity(int entityId);
    
    void addTransform(int entityId, Vector2 position);
    TransformComponent* getTransform(int entityId);
    const TransformComponent* getTransform(int entityId) const;
    
    void addVelocity(int entityId, Vector2 velocity);
    VelocityComponent* getVelocity(int entityId);
    const VelocityComponent* getVelocity(int entityId) const;
    
    void addRenderable(int entityId, float radius, Color color);
    RenderableComponent* getRenderable(int entityId);
    const RenderableComponent* getRenderable(int entityId) const;
    
    void addHealth(int entityId, int initialHealthPoints, int maxHealthPoints);
    HealthComponent* getHealth(int entityId);
    const HealthComponent* getHealth(int entityId) const;
    
    void addEnemy(int entityId, const EnemyComponent& enemyComponent);
    EnemyComponent* getEnemy(int entityId);
    const EnemyComponent* getEnemy(int entityId) const;
    
    const std::array<Entity, MAX_ENTITIES>& getEntities() const { return entities; }
    std::array<Entity, MAX_ENTITIES>& getEntities() { return entities; }
    int getEntityCount() const { return entityCount; }
    
    ComponentStorage<TransformComponent>& getTransforms() { return transforms; }
    const ComponentStorage<TransformComponent>& getTransforms() const { return transforms; }
    
    ComponentStorage<VelocityComponent>& getVelocities() { return velocities; }
    const ComponentStorage<VelocityComponent>& getVelocities() const { return velocities; }
    
    ComponentStorage<RenderableComponent>& getRenderables() { return renderables; }
    const ComponentStorage<RenderableComponent>& getRenderables() const { return renderables; }
    
    ComponentStorage<HealthComponent>& getHealths() { return healths; }
    const ComponentStorage<HealthComponent>& getHealths() const { return healths; }
    
    ComponentStorage<EnemyComponent>& getEnemies() { return enemies; }
    const ComponentStorage<EnemyComponent>& getEnemies() const { return enemies; }
};

#endif // ECS_HPP

