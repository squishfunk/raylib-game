#include "RenderSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../components/Components.hpp"
#include "../map/Map.hpp"
#include "textures/TextureManager.hpp"
#include "utils/Helpers.hpp"
#include <cassert>
#include <cstdio>
#include <string>
#include <raylib.h>

namespace {
    std::string lastPickedUpItemName = "";
    float lastPickedUpItemTime = -1.0f;
}

void RenderSystem::renderMinimap(const Map& map, int screenX, int screenY) {
    int cellSize = 15;
    int offsetX = screenX;
    int offsetY = screenY;
    
    int minimapWidth = MAP_WIDTH * cellSize + 4;
    int minimapHeight = MAP_HEIGHT * cellSize + 4;
    
    DrawRectangle(offsetX - 2, offsetY - 2, minimapWidth, minimapHeight, BLACK);
    DrawRectangleLines(offsetX - 2, offsetY - 2, minimapWidth, minimapHeight, BLACK);
    
    int currentX = map.getCurrentX();
    int currentY = map.getCurrentY();
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            const Room& room = map.getRoom(x, y);
            
            if (room.type == RoomType::EMPTY) continue;
            
            int px = offsetX + x * cellSize;
            int py = offsetY + y * cellSize;
            
            Color color = GRAY;
            if (room.type == RoomType::START) color = GREEN;
            else if(room.cleared) color = BEIGE;
            else if (room.type == RoomType::BOSS) color = RED;
            else if (room.type == RoomType::TREASURE) color = GOLD;
            else if (room.visited) color = LIGHTGRAY;
            else color = DARKGRAY;
            
            if (x == currentX && y == currentY) {
                color = YELLOW;
            }
            
            DrawRectangle(px, py, cellSize - 2, cellSize - 2, color);
            
            if ((room.doors & DoorFlags::UP) == DoorFlags::UP) 
                DrawLine(px + cellSize/2, py, px + cellSize/2, py - 3, WHITE);
            if ((room.doors & DoorFlags::DOWN) == DoorFlags::DOWN) 
                DrawLine(px + cellSize/2, py + cellSize, px + cellSize/2, py + cellSize + 3, WHITE);
            if ((room.doors & DoorFlags::LEFT) == DoorFlags::LEFT) 
                DrawLine(px, py + cellSize/2, px - 3, py + cellSize/2, WHITE);
            if ((room.doors & DoorFlags::RIGHT) == DoorFlags::RIGHT) 
                DrawLine(px + cellSize, py + cellSize/2, px + cellSize + 3, py + cellSize/2, WHITE);
        }
    }
}


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

void RenderSystem::renderECS(const ECS& ecs){
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

                Color color = GRAY;
                if(!door.opened){
                    color = BLACK;
                }

                DrawRectangle(transform.position.x, transform.position.y, door.width, door.height, color);
            }else if (renderables.isActive(i)){
                const TransformComponent &transform = transforms.get(i);
                const auto& renderable = renderables.get(i);
                
                DrawCircleV(transform.position, renderable.radius, renderable.color);
                
                EntityTag tags = entities[i].tags;
                if ((tags & EntityTag::ENEMY) == EntityTag::ENEMY && 
                    healths.isActive(i)) {
                    renderHealthbar(ecs, i);
                }
            }
        }
    }
}

void RenderSystem::render(const ECS& ecs, const Map &map) {
    renderECS(ecs);
}

void RenderSystem::renderUI(const ECS& ecs, int screenWidth, int screenHeight, int currentLevel) {
    float currentTime = GetTime();
    if (!lastPickedUpItemName.empty() && lastPickedUpItemTime >= 0.0f && currentTime - lastPickedUpItemTime < 2.0f) {
        float elapsed = currentTime - lastPickedUpItemTime;
        float alpha = 1.0f - (elapsed / 2.0f); // Fade out
        
        const char* text = lastPickedUpItemName.c_str();
        int fontSize = 30;
        int textWidth = MeasureText(text, fontSize);
        int x = screenWidth / 2 - textWidth / 2;
        int y = 100;
        
        // background
        Color bgColor = {0, 0, 0, static_cast<unsigned char>(150 * alpha)};
        DrawRectangle(x - 10, y - 5, textWidth + 20, fontSize + 10, bgColor);
        
        Color textColor = {255, 255, 255, static_cast<unsigned char>(255 * alpha)};
        DrawText(text, x, y, fontSize, textColor);
    }

    RenderSystem::renderPlayerHearths(ecs);

    std::string levelText = "Level: " + std::to_string(currentLevel);
    int fontSize = 12;
    int padding = 10;
    DrawText(levelText.c_str(), 100, padding, fontSize, BLACK);
}

void RenderSystem::setPickedUpItemName(const std::string& name) {
    lastPickedUpItemName = name;
    lastPickedUpItemTime = GetTime();
}

void RenderSystem::renderPlayerHearths(const ECS& ecs){
    int playerId = Helpers::getPlayerId(ecs);

    const HealthComponent* health = ecs.getHealth(playerId);

    assert(health && "Component Health not found on player");
    
    int fullHearthCount = health->healthPoints / 2;
    int halfHearth = health->healthPoints % 2;
    int remainingEmptyHearths = (health->maxHealthPoints - health->healthPoints) / 2;

    Texture2D hearthFull = TextureManager::Get("UI/hearth_full.png");
    Texture2D hearthHalf = TextureManager::Get("UI/hearth_half.png");
    Texture2D hearthEmpty = TextureManager::Get("UI/hearth_empty.png");

    float xPosition = 800;
    float yPosition = 40;
    float offset = 32;
    for(int i = 0; i < fullHearthCount; i++){
        DrawTextureRec(hearthFull, {0,0, 32, 32}, {xPosition, yPosition}, WHITE);
        xPosition += offset;
    }
    for(int i = 0; i < halfHearth; i++){
        DrawTextureRec(hearthHalf, {0,0, 32, 32}, {xPosition, yPosition}, WHITE);
        xPosition += offset;
    }
    for(int i = 0; i < remainingEmptyHearths; i++){
        DrawTextureRec(hearthEmpty, {0,0, 32, 32}, {xPosition, yPosition}, WHITE);
        xPosition += offset;
    }
}
