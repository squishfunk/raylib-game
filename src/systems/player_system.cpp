#include "player_system.hpp"
#include "../ecs/ecs.hpp"
#include "../components/components.hpp"
#include "../utils/helpers.hpp"
#include "bullet_system.hpp"
#include <raylib.h>
#include <raymath.h>

int PlayerSystem::createPlayer(ECS& ecs, int initX, int initY) {
    int playerId = ecs.createEntity();
    ecs.addTransform(playerId, Vector2{static_cast<float>(initX), static_cast<float>(initY)});
    ecs.addVelocity(playerId, Vector2{0, 0});
    ecs.addRenderable(playerId, 20.0f, GREEN);
    ecs.addHealth(playerId, 100, 100);
    ecs.addShootable(playerId, 1000.0f, BULLET_SPEED, SHOOT_COOLDOWN);
    ecs.getEntities()[playerId].tags = EntityTag::PLAYER;
    return playerId;
}

void PlayerSystem::update(ECS &ecs){
    int playerId = Helpers::getPlayerId(ecs);

    PlayerSystem::handleInput(ecs, playerId);
    PlayerSystem::handleShooting(ecs, playerId);
}

void PlayerSystem::handleInput(ECS& ecs, int playerId) {
    
    if (!ecs.getVelocities().isActive(playerId)) return;

    float deltaTime = GetFrameTime();
    float acceleration = 15.0f;
    float friction = 8.0f;
    
    auto& velocity = ecs.getVelocities().get(playerId);
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

void PlayerSystem::handleShooting(ECS& ecs, int playerId) {
    if (!(IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || 
          IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))) return;

    if (!ecs.getTransforms().isActive(playerId) || !ecs.getShootables().isActive(playerId)) return;

    float currentTime = GetTime();
    
    auto& shootable = ecs.getShootables().get(playerId);
    
    if (currentTime - shootable.lastShootTime < shootable.shootCooldown) return;
    
    const auto& playerTransform = ecs.getTransforms().get(playerId);
    Vector2 position = playerTransform.position;
    Vector2 velocity = {0, 0};
    
    if (IsKeyDown(KEY_UP)) velocity.y = -shootable.shootingSpeed;
    if (IsKeyDown(KEY_DOWN)) velocity.y = shootable.shootingSpeed;
    if (IsKeyDown(KEY_LEFT)) velocity.x = -shootable.shootingSpeed;
    if (IsKeyDown(KEY_RIGHT)) velocity.x = shootable.shootingSpeed;
    
    int bulletId = ecs.createEntity();
    if (bulletId >= 0) {
        ecs.addTransform(bulletId, position);
        ecs.addVelocity(bulletId, velocity);
        ecs.addRenderable(bulletId, BULLET_RADIUS, ORANGE);
        ecs.getEntities()[bulletId].tags = EntityTag::BULLET;
    }
    
    shootable.lastShootTime = currentTime;
}

