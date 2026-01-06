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
        
        // Check if enemy is alive
        bool isAlive = false;
        if (healths.isActive(i)) {
            const auto& health = healths.get(i);
            isAlive = health.healthPoints > 0;
        }
        
        if (!isAlive) continue;
        
        Vector2 enemyPos = transforms.get(i).position;
        Vector2 playerPos = playerTransform.position;
        
        float speed = 200.0f;
        
        Vector2 direction = Vector2Subtract(playerPos, enemyPos);
        
        float length = Vector2Length(direction);
        if (length < 0.001f) {
            velocities.get(i).velocity = Vector2{0, 0};
        } else {
            Vector2 velocity = Vector2Scale(Vector2Normalize(direction), speed);
            velocities.get(i).velocity = velocity;
        }
        
        // Handle random sounds
        if (enemies.isActive(i) && audios.isActive(i)) {
            auto& enemy = enemies.get(i);
            
            // Initialize next sound time if not set
            if (enemy.nextSoundTime == 0.0f) {
                float randomDelay = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 4.0f)); // 1-5 seconds
                enemy.nextSoundTime = currentTime + randomDelay;
            }
            
            // Play sound if time has come
            if (currentTime >= enemy.nextSoundTime) {
                PlaySound(audios.get(i).idle);
                enemy.lastSoundTime = currentTime;
                // Set next random sound time (1-5 seconds)
                float randomDelay = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 4.0f));
                enemy.nextSoundTime = currentTime + randomDelay;
            }
        }
    }
}

