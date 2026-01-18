#ifndef ECS_HPP
#define ECS_HPP

#include "../components/Components.hpp"
#include "../map/Map.hpp"
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

    std::array<T, MAX_ENTITIES>& getData(){
        return data;
    }
    
    const std::array<bool, MAX_ENTITIES>& getActiveFlags(){
        return active_flags;
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
    ComponentStorage<DoorComponent> doors;
    ComponentStorage<ShootableComponent> shootables;
    ComponentStorage<AudioComponent> audios;
    ComponentStorage<CircleColliderComponent> circleColliders;
    ComponentStorage<StatsManagerComponent> statsManagers;
    ComponentStorage<BehaviourModifierComponent> behaviourModifiers;
    ComponentStorage<ItemComponent> items;
    ComponentStorage<BulletComponent> bullets;
    ComponentStorage<SpriteRendererComponent> spriteRenderers;
    ComponentStorage<BoxColliderComponent> boxColliders;
    
public:
    ECS() : entityCount(0) {
        entities.fill(Entity());
    }
    
    int createEntity();
    void destroyEntity(int entityId);
    
    void addTransform(int entityId, Vector2 position);
    TransformComponent* getTransform(int entityId);
    const TransformComponent* getTransform(int entityId) const;
    
    void addVelocity(int entityId, Vector2 velocity, float speed = MOVEMENT_SPEED);
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
    
    void addDoor(int entityId, float width, float height, DoorFlags direction, bool opened);
    DoorComponent* getDoor(int entityId);
    const DoorComponent* getDoor(int entityId) const;
    
    void addShootable(int entityId, float shootingRange, float shootingSpeed, float shootCooldown);
    ShootableComponent* getShootable(int entityId);
    const ShootableComponent* getShootable(int entityId) const;
    
    void addAudio(int entityId, const std::string& soundKey, Sound sound);
    AudioComponent* getAudio(int entityId);
    const AudioComponent* getAudio(int entityId) const;
    
    void addCircleCollider(int entityId, float radius, bool isTrigger = false);
    CircleColliderComponent* getCircleCollider(int entityId);
    const CircleColliderComponent* getCircleCollider(int entityId) const;
    
    void addStatsManager(int entityId);
    StatsManagerComponent* getStatsManager(int entityId);
    const StatsManagerComponent* getStatsManager(int entityId) const;
    
    void addBehaviourModifier(int entityId);
    BehaviourModifierComponent* getBehaviourModifier(int entityId);
    const BehaviourModifierComponent* getBehaviourModifier(int entityId) const;
    
    void addItem(int entityId, const ItemComponent& itemComponent);
    ItemComponent* getItem(int entityId);
    const ItemComponent* getItem(int entityId) const;

    void addBullet(int entityId, const BulletComponent& bulletComponent);
    BulletComponent* getBullet(int entityId);
    const BulletComponent* getBullet(int entityId) const;

    void addSpriteRenderer(int entityId, const SpriteRendererComponent& spriteRendererComponent);
    SpriteRendererComponent* getSpriteRenderer(int entityId);
    const SpriteRendererComponent* getSpriteRenderer(int entityId) const;
    
    void addBoxCollider(int entityId, float width, float height, bool isTrigger = false);
    BoxColliderComponent* getBoxCollider(int entityId);
    const BoxColliderComponent* getBoxCollider(int entityId) const;
    
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
    
    ComponentStorage<DoorComponent>& getDoors() { return doors; }
    const ComponentStorage<DoorComponent>& getDoors() const { return doors; }
    
    ComponentStorage<ShootableComponent>& getShootables() { return shootables; }
    const ComponentStorage<ShootableComponent>& getShootables() const { return shootables; }
    
    ComponentStorage<AudioComponent>& getAudios() { return audios; }
    const ComponentStorage<AudioComponent>& getAudios() const { return audios; }
    
    ComponentStorage<CircleColliderComponent>& getCircleColliders() { return circleColliders; }
    const ComponentStorage<CircleColliderComponent>& getCircleColliders() const { return circleColliders; }
    
    ComponentStorage<StatsManagerComponent>& getStatsManagers() { return statsManagers; }
    const ComponentStorage<StatsManagerComponent>& getStatsManagers() const { return statsManagers; }
    
    ComponentStorage<BehaviourModifierComponent>& getBehaviourModifiers() { return behaviourModifiers; }
    const ComponentStorage<BehaviourModifierComponent>& getBehaviourModifiers() const { return behaviourModifiers; }
    
    ComponentStorage<ItemComponent>& getItems() { return items; }
    const ComponentStorage<ItemComponent>& getItems() const { return items; }

    ComponentStorage<BulletComponent>& getBullets() { return bullets; }
    const ComponentStorage<BulletComponent>& getBullets() const { return bullets; }

    ComponentStorage<SpriteRendererComponent>& getSpriteRenderers() { return spriteRenderers; }
    const ComponentStorage<SpriteRendererComponent>& getSpriteRenderers() const { return spriteRenderers; }
    
    ComponentStorage<BoxColliderComponent>& getBoxColliders() { return boxColliders; }
    const ComponentStorage<BoxColliderComponent>& getBoxColliders() const { return boxColliders; }
};

#endif // ECS_HPP

