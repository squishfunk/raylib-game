#include "RenderSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include <cstdio>
#include <raylib.h>

void RenderSystem::renderHealthbar(const ECS& ecs, int entityId) {
    const auto& transforms = ecs.getTransforms();
    const auto& renderables = ecs.getRenderables();
    const auto& healths = ecs.getHealths();

    const auto& transform = transforms.get(entityId);
    const auto& renderable = renderables.get(entityId);
    const auto& health = healths.get(entityId);
    
    int width = static_cast<int>(renderable.radius * 2.5f);
    int height = 10;
    
    int x = static_cast<int>(transform.position.x - width / 2.0f);
    int y = static_cast<int>(transform.position.y + renderable.radius * 1.5f);
    
    DrawRectangle(x, y, width, height, DARKGRAY);
    DrawRectangle(x, y, static_cast<int>(width * health.healthPoints / health.maxHealthPoints), height, RED);
    DrawRectangleLines(x, y, width, height, BLACK);
}

void RenderSystem::render(const ECS& ecs) {
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    const auto& transforms = ecs.getTransforms();
    const auto& renderables = ecs.getRenderables();
    const auto& doors = ecs.getDoors();
    const auto& healths = ecs.getHealths();
    
    for (int i = 0; i < entityCount; i++) {
        if (!entities[i].active) continue;

        if (transforms.isActive(i)){
            /*  TODO renderables have to render doors and enemies */
            if(doors.isActive(i)){
                const TransformComponent &transform = transforms.get(i);
                const auto& door = doors.get(i);

                DrawRectangle(transform.position.x, transform.position.y, door.width, door.height, GRAY);
            }else if (renderables.isActive(i)){
                const TransformComponent &transform = transforms.get(i);
                const auto& renderable = renderables.get(i);
                
                DrawCircleV(transform.position, renderable.radius, renderable.color);
                
                EntityTag tags = entities[i].tags;
                if (((tags & EntityTag::ENEMY) == EntityTag::ENEMY || 
                     (tags & EntityTag::PLAYER) == EntityTag::PLAYER) && 
                    healths.isActive(i)) {
                    renderHealthbar(ecs, i);
                }
            }
        }
    }
}

