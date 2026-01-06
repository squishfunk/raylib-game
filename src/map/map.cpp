#include "map.hpp"
#include "../game/game.hpp"
#include "../ecs/ecs.hpp"
#include "../components/components.hpp"
#include "../systems/enemy_system.hpp"
#include "../utils/helpers.hpp"
#include <cstring>
#include <cstdlib>
#include <raylib.h>

Map::Map() : startX(0), startY(0), currentX(0), currentY(0), generated(false) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            rooms[y][x] = Room();
        }
    }
}

void Map::init() {
    generated = false;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            rooms[y][x] = Room();
        }
    }
}

void Map::connectRooms(int x1, int y1, int x2, int y2) {
    Room& room1 = rooms[y1][x1];
    Room& room2 = rooms[y2][x2];
    
    if (x2 > x1) {
        room1.doors = room1.doors | DoorFlags::RIGHT;
        room2.doors = room2.doors | DoorFlags::LEFT;
    } else if (x2 < x1) {
        room1.doors = room1.doors | DoorFlags::LEFT;
        room2.doors = room2.doors | DoorFlags::RIGHT;
    }
    
    if (y2 > y1) {
        room1.doors = room1.doors | DoorFlags::DOWN;
        room2.doors = room2.doors | DoorFlags::UP;
    } else if (y2 < y1) {
        room1.doors = room1.doors | DoorFlags::UP;
        room2.doors = room2.doors | DoorFlags::DOWN;
    }
}

void Map::generate() {
    init();
    
    int pathLength = 8 + GetRandomValue(0, 5);
    int currentX = MAP_WIDTH / 2;
    int currentY = MAP_HEIGHT / 2;
    
    startX = currentX;
    startY = currentY;
    this->currentX = currentX;
    this->currentY = currentY;

    Rectangle roomBounds = Rectangle{0, 0, 1000.0f, 650.0f};
    
    rooms[currentY][currentX].type = RoomType::START;
    rooms[currentY][currentX].gridX = currentX;
    rooms[currentY][currentX].gridY = currentY;
    rooms[currentY][currentX].visited = true;
    rooms[currentY][currentX].cleared = false;
    rooms[currentY][currentX].bounds = roomBounds; 
    rooms[currentY][currentX].enemySpawns = generateEnemySpawns(RoomType::START, roomBounds);

    
    for (int i = 1; i < pathLength; i++) {
        int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
        int attempts = 0;
        int newX, newY;
        
        do {
            int dir = GetRandomValue(0, 3);
            newX = currentX + directions[dir][0];
            newY = currentY + directions[dir][1];
            attempts++;
        } while ((newX < 0 || newX >= MAP_WIDTH || 
                 newY < 0 || newY >= MAP_HEIGHT ||
                 rooms[newY][newX].type != RoomType::EMPTY) && attempts < 20);
        
        if (attempts < 20) {
            connectRooms(currentX, currentY, newX, newY);
            
            currentX = newX;
            currentY = newY;

            RoomType roomType;
            
            if (i == pathLength - 1) {
                roomType = RoomType::BOSS;
            } else {
                roomType = RoomType::NORMAL;
            }
            rooms[currentY][currentX].gridX = currentX;
            rooms[currentY][currentX].gridY = currentY;
            rooms[currentY][currentX].bounds = roomBounds; 
            rooms[currentY][currentX].type = roomType;
            rooms[currentY][currentX].enemySpawns = generateEnemySpawns(rooms[currentY][currentX].type, roomBounds);
        }
    }
    
    int extraRooms = GetRandomValue(2, 5);
    for (int e = 0; e < extraRooms; e++) {
        int baseX, baseY;
        int attempts = 0;
        do {
            baseX = GetRandomValue(0, MAP_WIDTH - 1);
            baseY = GetRandomValue(0, MAP_HEIGHT - 1);
            attempts++;
        } while (rooms[baseY][baseX].type == RoomType::EMPTY && attempts < 50);
        
        if (attempts < 50) {
            int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
            int dir = GetRandomValue(0, 3);
            int newX = baseX + directions[dir][0];
            int newY = baseY + directions[dir][1];
            
            if (newX >= 0 && newX < MAP_WIDTH && 
                newY >= 0 && newY < MAP_HEIGHT &&
                rooms[newY][newX].type == RoomType::EMPTY) {
                
                connectRooms(baseX, baseY, newX, newY);
                rooms[newY][newX].type = RoomType::NORMAL;
                rooms[newY][newX].gridX = newX;
                rooms[newY][newX].gridY = newY;
                rooms[newY][newX].bounds = roomBounds;
                rooms[currentY][currentX].enemySpawns = generateEnemySpawns(RoomType::START, roomBounds);
            }
        }
    }
    
    generated = true;
}

void Map::renderMinimap(int screenX, int screenY) const {
    int cellSize = 15;
    int offsetX = screenX;
    int offsetY = screenY;
    
    int minimapWidth = MAP_WIDTH * cellSize + 4;
    int minimapHeight = MAP_HEIGHT * cellSize + 4;
    
    DrawRectangle(offsetX - 2, offsetY - 2, minimapWidth, minimapHeight, WHITE);
    DrawRectangleLines(offsetX - 2, offsetY - 2, minimapWidth, minimapHeight, BLACK);
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            const Room& room = rooms[y][x];
            
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

bool Map::canMoveTo(int newX, int newY) const {
    if (newX < 0 || newX >= MAP_WIDTH || newY < 0 || newY >= MAP_HEIGHT) return false;
    
    const Room& currentRoom = rooms[currentY][currentX];
    const Room& targetRoom = rooms[newY][newX];
    
    if (targetRoom.type == RoomType::EMPTY) return false;
    
    if (newX > currentX && (currentRoom.doors & DoorFlags::RIGHT) != DoorFlags::RIGHT) return false;
    if (newX < currentX && (currentRoom.doors & DoorFlags::LEFT) != DoorFlags::LEFT) return false;
    if (newY > currentY && (currentRoom.doors & DoorFlags::DOWN) != DoorFlags::DOWN) return false;
    if (newY < currentY && (currentRoom.doors & DoorFlags::UP) != DoorFlags::UP) return false;
    
    if (!currentRoom.cleared) return false;
    
    return true;
}

void Map::moveToRoom(Game& game, int newX, int newY) {
    if (!canMoveTo(newX, newY)) return;
    
    ECS& ecs = game.getECS();
    int playerId = game.getPlayerId();
    
    auto* playerTransform = ecs.getTransform(playerId);
    if (!playerTransform) return;
    
    const auto& entities = ecs.getEntities();
    int entityCount = ecs.getEntityCount();
    
    for (int i = 0; i < entityCount; i++) {
        if (entities[i].active) {
            EntityTag tags = entities[i].tags;
            if ((tags & EntityTag::BULLET) == EntityTag::BULLET || 
                (tags & EntityTag::ENEMY) == EntityTag::ENEMY) {
                ecs.getEntities()[i].active = false;
            }
        }
    }
    
    currentX = newX;
    currentY = newY;
    rooms[newY][newX].visited = true;
}

std::vector<EnemySpawn> Map::generateEnemySpawns(RoomType roomType, const Rectangle& bounds) {
    std::vector<EnemySpawn> spawns;
    
    switch (roomType) {
        case RoomType::START:
            if (GetRandomValue(0, 100) < 30) {
                int count = GetRandomValue(1, 2);
                for (int i = 0; i < count; i++) {
                    float x = static_cast<float>(GetRandomValue(100, static_cast<int>(bounds.width) - 100));
                    float y = static_cast<float>(GetRandomValue(100, static_cast<int>(bounds.height) - 100));
                    spawns.push_back(EnemySpawn{Vector2{x, y}, EnemyType::NORMAL});
                }
            }
            break;
            
        case RoomType::NORMAL:
            {
                int count = GetRandomValue(2, 5);
                for (int i = 0; i < count; i++) {
                    float x = static_cast<float>(GetRandomValue(50, static_cast<int>(bounds.width) - 50));
                    float y = static_cast<float>(GetRandomValue(50, static_cast<int>(bounds.height) - 50));
                    
                    int typeRoll = GetRandomValue(0, 100);
                    EnemyType type;
                    if (typeRoll < 50) {
                        type = EnemyType::NORMAL;
                    } else if (typeRoll < 80) {
                        type = EnemyType::FAST;
                    } else {
                        type = EnemyType::TANK;
                    }
                    
                    spawns.push_back(EnemySpawn{Vector2{x, y}, type});
                }
            }
            break;
            
        case RoomType::BOSS:
            {
                spawns.push_back(EnemySpawn{
                    Vector2{bounds.width / 2.0f, bounds.height / 2.0f}, 
                    EnemyType::BOSS
                });
                
                int minionCount = GetRandomValue(2, 4);
                for (int i = 0; i < minionCount; i++) {
                    float angle = (i * 360.0f / minionCount) * DEG2RAD;
                    float radius = 200.0f;
                    float x = bounds.width / 2.0f + cosf(angle) * radius;
                    float y = bounds.height / 2.0f + sinf(angle) * radius;
                    
                    x = (x < 50) ? 50 : (x > bounds.width - 50) ? bounds.width - 50 : x;
                    y = (y < 50) ? 50 : (y > bounds.height - 50) ? bounds.height - 50 : y;
                    
                    spawns.push_back(EnemySpawn{Vector2{x, y}, EnemyType::NORMAL});
                }
            }
            break;
            
        case RoomType::TREASURE:
            if (GetRandomValue(0, 100) < 60) {
                int count = GetRandomValue(1, 3);
                for (int i = 0; i < count; i++) {
                    float x = static_cast<float>(GetRandomValue(100, static_cast<int>(bounds.width) - 100));
                    float y = static_cast<float>(GetRandomValue(100, static_cast<int>(bounds.height) - 100));
                    spawns.push_back(EnemySpawn{Vector2{x, y}, EnemyType::TANK});
                }
            }
            break;
            
        case RoomType::SECRET:
            if (GetRandomValue(0, 100) < 40) {
                int count = GetRandomValue(1, 2);
                for (int i = 0; i < count; i++) {
                    float x = static_cast<float>(GetRandomValue(50, static_cast<int>(bounds.width) - 50));
                    float y = static_cast<float>(GetRandomValue(50, static_cast<int>(bounds.height) - 50));
                    spawns.push_back(EnemySpawn{Vector2{x, y}, EnemyType::FAST});
                }
            }
            break;
            
        case RoomType::EMPTY:
        default:
            break;
    }
    
    return spawns;
}
