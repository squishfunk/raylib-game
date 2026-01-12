#include "EnemySystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../utils/Helpers.hpp"
#include <raymath.h>
#include <raylib.h>
#include <cstdlib>
#include <ctime>

void EnemySystem::update(ECS& ecs) {
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    auto& transforms = ecs.getTransforms();
    auto& velocities = ecs.getVelocities();
    auto& enemies = ecs.getEnemies();
    auto& healths = ecs.getHealths();
    auto& audios = ecs.getAudios();
    
    int playerId = Helpers::getPlayerId(ecs);
    if (playerId < 0) return;
    
    const auto& playerTransform = transforms.get(playerId);
    float currentTime = GetTime();
    
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;
        if ((entities[i].tags & EntityTag::ENEMY) != EntityTag::ENEMY) continue;
        if (!enemies.isActive(i)) continue;

        auto& enemy = enemies.get(i);

        EnemySystem::handleShooting(ecs, i, playerId);

        bool isAlive = false;
        if (healths.isActive(i)) {
            const auto& health = healths.get(i);
            isAlive = health.healthPoints > 0;
        }
        
        if (!isAlive) continue;
        
        
        if(currentTime > enemy.spawnTime + enemy.spawnIdleDuration){
            EnemySystem::HandleMove(ecs, i, playerTransform);
        }
        
        if (audios.isActive(i)) {
            // Initialize next sound time if not set
            if (enemy.nextSoundTime == 0.0f) {
                float randomDelay = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 4.0f)); // 1-5 seconds
                enemy.nextSoundTime = currentTime + randomDelay;
            }
            
            if (currentTime >= enemy.nextSoundTime) {
                audios.get(i).play("IDLE_SOUND");
                enemy.lastSoundTime = currentTime;
                float randomDelay = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 4.0f));
                enemy.nextSoundTime = currentTime + randomDelay;
            }
        }
    }
}

void EnemySystem::handleShooting(ECS& ecs, int enemyId, int playerId) {
    float currentTime = GetTime();
    
    auto& shootable = ecs.getShootables().get(enemyId);
    auto& transform = ecs.getTransforms().get(enemyId);
    
    if (currentTime - shootable.lastShootTime < shootable.shootCooldown) return;
    
    const auto& playerTransform = ecs.getTransforms().get(playerId);

    Vector2 direction = Vector2Subtract(playerTransform.position, transform.position);
    direction = Vector2Normalize(direction);
    
    if (ecs.getAudios().isActive(enemyId)) {
        ecs.getAudio(enemyId)->play("SHOOT_SOUND");
    }
    
    shootable.direction = direction;
    shootable.shoot = true;
}


void EnemySystem::HandleMove(ECS& ecs, int enemyId, const TransformComponent& playerTransform) {
    auto& enemyVelocity = ecs.getVelocities().get(enemyId);
    Vector2 enemyPos = ecs.getTransforms().get(enemyId).position;
    Vector2 playerPos = playerTransform.position;
    
    Vector2 direction = Vector2Subtract(playerPos, enemyPos);
    
    float length = Vector2Length(direction);
    if (length < 0.001f) {
        enemyVelocity.velocity = Vector2{0, 0};
    } else {
        Vector2 velocity = Vector2Normalize(direction);
        enemyVelocity.velocity = velocity;
    }
}