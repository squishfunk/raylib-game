#include "Map.hpp"
#include "../components/Components.hpp"
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <raylib.h>
#include <math.h>
#include <vector>

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

std::vector<RoomCord> Map::GeneratePath(RoomCord start){
    int pathLength = MIN_MAP_PATH_LENGTH + GetRandomValue(0, 5);
    int allPosibleDirections[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    
    std::vector<RoomCord> path;
    do{
        std::vector<RoomCord> visitedPath;
        path.clear();

        RoomCord current = start;
        path.push_back(start);
        visitedPath.push_back(start);

        for (int i = 0; i < pathLength; i++) {
            int attempts = 0;
            RoomCord newCord;
            
            do {
                int dir = GetRandomValue(0, 3);
                newCord = {current.x + allPosibleDirections[dir][0], current.y + allPosibleDirections[dir][1]};
                attempts++;
            } while ((newCord.x < 0 || newCord.x >= MAP_WIDTH || 
                newCord.y < 0 || newCord.y >= MAP_HEIGHT ||
                std::find(visitedPath.begin(), visitedPath.end(), newCord) != visitedPath.end()) && attempts < 20);
            
            if (attempts < 20) {
                visitedPath.push_back(newCord);
                path.push_back(newCord);
                current = newCord;
            } else {
                break;
            }
        }
    }
    while(path.size() < MIN_MAP_PATH_LENGTH);

    return path;
}
void Map::generate() {
    init();
    
    // 2
    startX = MAP_WIDTH / 2;
    // 2
    startY = MAP_HEIGHT / 2;

    currentX = startX;
    currentY = startY;
    
    Rectangle roomBounds = Rectangle{0, 0, 1000.0f, 650.0f};
    int allPosibleDirections[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    std::vector<RoomCord> path = Map::GeneratePath(RoomCord{startX, startY});

    for (size_t i = 0; i < path.size(); i++) {
        RoomCord currentCord = path[i];

        RoomType roomType;
        if (i == 0) {
            roomType = RoomType::START;
        }else{
            RoomCord prevCord = path[i-1];

            connectRooms(prevCord.x, prevCord.y, currentCord.x, currentCord.y);

            if (i == path.size() - 1) {
                roomType = RoomType::BOSS;
            } else {
                roomType = RoomType::NORMAL;
            }
        } 

        rooms[currentCord.y][currentCord.x].type = roomType;
        rooms[currentCord.y][currentCord.x].gridX = currentCord.x;
        rooms[currentCord.y][currentCord.x].gridY = currentCord.y;
        rooms[currentCord.y][currentCord.x].visited = false; /* TODO could be bug */
        rooms[currentCord.y][currentCord.x].cleared = false;
        rooms[currentCord.y][currentCord.x].bounds = roomBounds; 
        rooms[currentCord.y][currentCord.x].enemySpawns = generateEnemySpawns(roomType, roomBounds);
    }
    
    int extraRooms = GetRandomValue(2, 4);
    for (int e = 0; e < extraRooms; e++) {
        int attempts = 0;
        RoomCord baseRoom, newRoom;
        bool founded = false; 
        do {
            baseRoom.x = GetRandomValue(0, MAP_WIDTH - 1);
            baseRoom.y = GetRandomValue(0, MAP_HEIGHT - 1);
            RoomType baseRoomType = rooms[baseRoom.y][baseRoom.x].type;

            if(baseRoomType != RoomType::EMPTY && baseRoomType != RoomType::BOSS){

            
                int dir = GetRandomValue(0, 3);
                int i = 0;
                while (i < 4){
                    newRoom.x = baseRoom.x + allPosibleDirections[dir][0];
                    newRoom.y = baseRoom.y + allPosibleDirections[dir][1];
                    RoomType newRoomType = rooms[newRoom.y][newRoom.x].type;

                    if (newRoom.x >= 0 && newRoom.x < MAP_WIDTH && 
                        newRoom.y >= 0 && newRoom.y < MAP_HEIGHT &&
                        newRoomType == RoomType::EMPTY) {
                            founded = true;
                            break;
                    }

                    dir = dir + 1 > 3 ? 0 : dir+1;

                    i++;
                }
            }
            
            attempts++;
        } while (!founded && attempts < 50);
        
        if (founded) {
            connectRooms(baseRoom.x, baseRoom.y, newRoom.x, newRoom.y);
            rooms[newRoom.y][newRoom.x].type = RoomType::TREASURE;
            rooms[newRoom.y][newRoom.x].gridX = newRoom.x;
            rooms[newRoom.y][newRoom.x].gridY = newRoom.y;
            rooms[newRoom.y][newRoom.x].bounds = roomBounds;
        }
    }

    currentX = startX;
    currentY = startY;
    
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
                    if (typeRoll < 40) {
                        type = EnemyType::NORMAL;
                    }
                    else if (typeRoll < 60) {
                        type = EnemyType::RANGED_NORMAL;
                    }
                    else if (typeRoll < 80) {
                        type = EnemyType::RANGED_TANK;
                    }
                    else if (typeRoll < 95) {
                        type = EnemyType::FAST;
                    }
                    else {
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
