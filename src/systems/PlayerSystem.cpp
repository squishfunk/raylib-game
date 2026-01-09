#include "PlayerSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../utils/Helpers.hpp"
#include "BulletSystem.hpp"
#include <raylib.h>
#include <raymath.h>

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
    
    if (IsKeyDown(KEY_W)) targetVelocity.y = -1.0;
    if (IsKeyDown(KEY_S)) targetVelocity.y = 1.0;
    if (IsKeyDown(KEY_A)) targetVelocity.x = -1.0;
    if (IsKeyDown(KEY_D)) targetVelocity.x = 1.0;

    targetVelocity = Vector2Normalize(targetVelocity);
    
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
    
    Vector2 direction = {0, 0};
    
    if (IsKeyDown(KEY_UP)) direction.y = -1.0f;
    if (IsKeyDown(KEY_DOWN)) direction.y = 1.0f;
    if (IsKeyDown(KEY_LEFT)) direction.x = -1.0f;
    if (IsKeyDown(KEY_RIGHT)) direction.x = 1.0f;

    direction = Vector2Normalize(direction);
    
    if (ecs.getAudios().isActive(playerId)) {
        ecs.getAudio(playerId)->play("SHOOT_SOUND");
    }
    
    shootable.direction = direction;
    shootable.shoot = true;
}

