#include "DebugSystem.hpp"
#include "../ecs/Ecs.hpp"
#include "../map/Map.hpp"
#include "../map/Dungeon.hpp"
#include "../components/Components.hpp"
#include "../utils/Helpers.hpp"
#include <raylib.h>
#include <cstdio>

void DebugSystem::update(ECS& ecs, Map& map, Dungeon* dungeon, int playerId) {
    handleDebugInput(ecs, map, dungeon, playerId);
}

void DebugSystem::handleDebugInput(ECS& ecs, Map& map, Dungeon* dungeon, int playerId) {
    if (IsKeyPressed(KEY_X)) {
        killAllEnemies(ecs);
        printf("[DEBUG] Killed all enemies\n");
    }
    
    if (IsKeyPressed(KEY_H)) {
        healPlayer(ecs, playerId);
        printf("[DEBUG] Player healed to full HP\n");
    }
    
    if (IsKeyPressed(KEY_C)) {
        clearCurrentRoom(map);
        printf("[DEBUG] Current room cleared\n");
    }
    
    if (IsKeyPressed(KEY_T)) {
        teleportToBoss(map, dungeon);
        printf("[DEBUG] Teleported to BOSS room\n");
    }
    
    if (IsKeyPressed(KEY_K)) {
        if (ecs.getHealths().isActive(playerId)) {
            ecs.getHealths().get(playerId).healthPoints = 0;
            printf("[DEBUG] Player killed\n");
        }
    }
}

void DebugSystem::killAllEnemies(ECS& ecs) {
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    
    for (int i = 0; i < entityCount; i++) {
        if (entities[i].active && 
            (entities[i].tags & EntityTag::ENEMY) == EntityTag::ENEMY) {
            ecs.destroyEntity(i);
        }
    }
}

void DebugSystem::healPlayer(ECS& ecs, int playerId) {
    if (ecs.getHealths().isActive(playerId)) {
        auto& health = ecs.getHealths().get(playerId);
        health.healthPoints = health.maxHealthPoints;
    }
}

void DebugSystem::clearCurrentRoom(Map& map) {
    int currentX = map.getCurrentX();
    int currentY = map.getCurrentY();
    Room& room = map.getRoom(currentX, currentY);
    room.cleared = true;
}

void DebugSystem::teleportToBoss(Map& map, Dungeon* dungeon) {
    int bossX = -1, bossY = -1;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map.getRoom(x, y).type == RoomType::BOSS) {
                bossX = x;
                bossY = y;
                break;
            }
        }
        if (bossX != -1) break;
    }
    
    if (bossX != -1 && bossY != -1) {
        dungeon->loadRoom(map.getRoom(bossX, bossY), DoorFlags::DOWN);
    }
}


void DebugSystem::render(const ECS& ecs, const Map& map, int playerId, int screenWidth, int screenHeight) {
    int padding = 10;
    int fontSize = 14;
    int lineHeight = 18;
    
    int infoStartY = screenHeight - 200;
    int infoWidth = 350;
    
    DrawRectangle(0, infoStartY, infoWidth, 190, (Color){0, 0, 0, 200});
    DrawRectangleLines(0, infoStartY, infoWidth, 190, DARKGRAY);
    
    DrawText("DEBUG INFO", padding, infoStartY + 5, 18, WHITE);
    int y = infoStartY + 25;
    
    if (ecs.getTransforms().isActive(playerId)) {
        const auto& transform = ecs.getTransforms().get(playerId);
        char posText[64];
        snprintf(posText, sizeof(posText), "Pos: (%.1f, %.1f)", 
                 transform.position.x, transform.position.y);
        DrawText(posText, padding, y, fontSize, GREEN);
        y += lineHeight;
    }
    
    int roomX = map.getCurrentX();
    int roomY = map.getCurrentY();
    char roomText[64];
    snprintf(roomText, sizeof(roomText), "Room: (%d, %d)", roomX, roomY);
    DrawText(roomText, padding, y, fontSize, YELLOW);
    y += lineHeight;
    const Room& currentRoom = map.getRoom(roomX, roomY);
    const char* roomTypeStr = "UNKNOWN";
    switch(currentRoom.type) {
        case RoomType::START: roomTypeStr = "START"; break;
        case RoomType::NORMAL: roomTypeStr = "NORMAL"; break;
        case RoomType::BOSS: roomTypeStr = "BOSS"; break;
        case RoomType::TREASURE: roomTypeStr = "TREASURE"; break;
        case RoomType::EMPTY: roomTypeStr = "EMPTY"; break;
    }
    char typeText[64];
    snprintf(typeText, sizeof(typeText), "Room Type: %s", roomTypeStr);
    DrawText(typeText, padding, y, fontSize, LIGHTGRAY);
    y += lineHeight;
    
    if (ecs.getHealths().isActive(playerId)) {
        const auto& health = ecs.getHealths().get(playerId);
        char hpText[64];
        snprintf(hpText, sizeof(hpText), "HP: %d/%d (%.0f%%)", 
                 health.healthPoints, health.maxHealthPoints,
                 (float)health.healthPoints / health.maxHealthPoints * 100.0f);
        DrawText(hpText, padding, y, fontSize, RED);
        y += lineHeight;
    }
    
    int enemyCount = 0;
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    for (int i = 0; i < entityCount; i++) {
        if (entities[i].active && 
            (entities[i].tags & EntityTag::ENEMY) == EntityTag::ENEMY) {
            enemyCount++;
        }
    }
    char enemyText[64];
    snprintf(enemyText, sizeof(enemyText), "Enemies: %d", enemyCount);
    DrawText(enemyText, padding, y, fontSize, ORANGE);
    y += lineHeight;
    
    const char* roomStatus = currentRoom.cleared ? "CLEARED" : "NOT CLEARED";
    Color statusColor = currentRoom.cleared ? GREEN : RED;
    DrawText(roomStatus, padding, y, fontSize, statusColor);
    y += lineHeight;
    
    char fpsText[32];
    snprintf(fpsText, sizeof(fpsText), "FPS: %d", GetFPS());
    DrawText(fpsText, padding, y, fontSize, BLUE);
    
    int cheatStartX = screenWidth - 280;
    int cheatStartY = screenHeight - 200;
    int cheatWidth = 270;
    
    DrawRectangle(cheatStartX, cheatStartY, cheatWidth, 190, (Color){0, 0, 0, 200});
    DrawRectangleLines(cheatStartX, cheatStartY, cheatWidth, 190, DARKGRAY);
    
    DrawText("DEBUG COMMANDS", cheatStartX + padding, cheatStartY + 5, 18, WHITE);
    int cheatY = cheatStartY + 25;
    
    struct CheatCommand {
        const char* key;
        const char* description;
        Color color;
    };
    
    CheatCommand commands[] = {
        {"X", "Kill all enemies", RED},
        {"H", "Heal player", GREEN},
        {"C", "Clear current room", YELLOW},
        {"T", "Teleport to BOSS", MAGENTA},
        {"K", "Kill player", DARKGREEN},
    };
    
    for (int i = 0; i < 5; i++) {
        char keyText[16];
        snprintf(keyText, sizeof(keyText), "[%s]", commands[i].key);
        DrawText(keyText, cheatStartX + padding, cheatY, fontSize + 2, commands[i].color);
        
        int keyWidth = MeasureText(keyText, fontSize + 2);
        DrawText(commands[i].description, 
                 cheatStartX + padding + keyWidth + 10, 
                 cheatY + 2, 
                 fontSize, 
                 LIGHTGRAY);
        cheatY += lineHeight + 2;
    }
}