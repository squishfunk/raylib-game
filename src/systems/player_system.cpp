#include "player_system.hpp"
#include "../ecs/ecs.hpp"
#include "../components/components.hpp"
#include "bullet_system.hpp"
#include <raymath.h>

int PlayerSystem::createPlayer(ECS& ecs, int initX, int initY) {
    int playerId = ecs.createEntity();
    ecs.addTransform(playerId, Vector2{static_cast<float>(initX), static_cast<float>(initY)});
    ecs.addVelocity(playerId, Vector2{0, 0});
    ecs.addRenderable(playerId, 20.0f, GREEN);
    ecs.addHealth(playerId, 100, 100);
    ecs.getEntities()[playerId].tags = EntityTag::PLAYER;
    return playerId;
}

void PlayerSystem::handleInput(ECS& ecs, int entityId) {
    if (!ecs.getVelocities().isActive(entityId)) return;
    
    float deltaTime = GetFrameTime();
    float acceleration = 15.0f;
    float friction = 8.0f;
    
    auto& velocity = ecs.getVelocities().get(entityId);
    Vector2 targetVelocity = {0, 0};
    
    if (IsKeyDown(KEY_W)) targetVelocity.y = -MOVEMENT_SPEED;
    if (IsKeyDown(KEY_S)) targetVelocity.y = MOVEMENT_SPEED;
    if (IsKeyDown(KEY_A)) targetVelocity.x = -MOVEMENT_SPEED;
    if (IsKeyDown(KEY_D)) targetVelocity.x = MOVEMENT_SPEED;
    
    Vector2 currentVel = velocity.velocity;
    
    if (targetVelocity.x != 0 || targetVelocity.y != 0) {
        velocity.velocity.x = Lerp(currentVel.x, targetVelocity.x, acceleration * deltaTime);
        velocity.velocity.y = Lerp(currentVel.y, targetVelocity.y, acceleration * deltaTime);
    } else {
        velocity.velocity.x = Lerp(currentVel.x, 0.0f, friction * deltaTime);
        velocity.velocity.y = Lerp(currentVel.y, 0.0f, friction * deltaTime);
    }
}

void PlayerSystem::handleShooting(ECS& ecs, int playerId, float currentTime, float& lastShootTime) {
    BulletSystem::update(ecs);
    
    if (!(IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || 
          IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))) return;
    
    if (currentTime - lastShootTime < SHOOT_COOLDOWN) return;
    if (!ecs.getTransforms().isActive(playerId)) return;
    
    const auto& playerTransform = ecs.getTransforms().get(playerId);
    Vector2 position = playerTransform.position;
    Vector2 velocity = {0, 0};
    
    if (IsKeyDown(KEY_UP)) velocity.y = -BULLET_SPEED;
    if (IsKeyDown(KEY_DOWN)) velocity.y = BULLET_SPEED;
    if (IsKeyDown(KEY_LEFT)) velocity.x = -BULLET_SPEED;
    if (IsKeyDown(KEY_RIGHT)) velocity.x = BULLET_SPEED;
    
    int bulletId = ecs.createEntity();
    if (bulletId >= 0) {
        ecs.addTransform(bulletId, position);
        ecs.addVelocity(bulletId, velocity);
        ecs.addRenderable(bulletId, BULLET_RADIUS, ORANGE);
        ecs.getEntities()[bulletId].tags = EntityTag::BULLET;
    }
    
    lastShootTime = currentTime;
}

