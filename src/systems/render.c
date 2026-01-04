#include "../ecs.h"
#include <raymath.h>

void render_healthbar(ECS *ecs, int entityId){
    TransformComponent *transform = &ecs->transforms.data[entityId];
    RenderableComponent *renderable = &ecs->renderables.data[entityId];
    HealthComponent *health = &ecs->healths.data[entityId];
            
    int width = (int)(renderable->radius * 2.5);
    int height = SCREEN_WIDTH / 100;

    int x = (int)(transform->position.x - width / 2.0f);
    int y = (int)(transform->position.y + renderable->radius * 1.5);

    DrawRectangle(x, y, width, height, DARKGRAY);
    DrawRectangle(x, y, (int)(width * health->healthPoints / health->maxHealthPoints), height, RED);
    DrawRectangleLines(x, y, width, height, BLACK);
}

void render_system(ECS *ecs){
    for(int i = 0; i < ecs->entityCount; i++){
        if (!ecs->entities[i].active) continue;
        if (!ecs->transforms.active[i] || !ecs->renderables.active[i]) continue;

        TransformComponent *transform = &ecs->transforms.data[i];
        RenderableComponent *renderable = &ecs->renderables.data[i];

        DrawCircleV(transform->position, renderable->radius, renderable->color);

        if ((ecs->entities[i].tags & TAG_ENEMY || ecs->entities[i].tags & TAG_PLAYER) && ecs->healths.active[i]){
            render_healthbar(ecs, i);
        }
    }
}