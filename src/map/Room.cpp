#include "Room.hpp"
#include <cmath>

void Room::GenerateRoomShape(){
    structure.gridWidth = ROOM_GRID_WIDTH;
    structure.gridHeight = ROOM_GRID_HEIGHT;

    std::vector<std::vector<TileType>> grid(ROOM_GRID_HEIGHT, std::vector<TileType>(ROOM_GRID_WIDTH, TileType::EMPTY));

    if(structure.structureType == RoomStructureType::BOX){
        for(int y = 0; y < ROOM_GRID_HEIGHT; y++) {
            for(int x = 0; x < ROOM_GRID_WIDTH; x++) {
                bool isEdge = (x == 0 || x == ROOM_GRID_WIDTH-1 || y == 0 || y == ROOM_GRID_HEIGHT-1);
                
                if(isEdge) {
                    // bool isDoorPosition = false;
                    
                    // if(y == 0 && x == ROOM_GRID_WIDTH / 2) {
                    //     isDoorPosition = true;
                    // }
                    // else if(y == ROOM_GRID_HEIGHT-1 && x == ROOM_GRID_WIDTH / 2) {
                    //     isDoorPosition = true;
                    // }
                    // else if(x == 0 && y == ROOM_GRID_HEIGHT / 2) {
                    //     isDoorPosition = true;
                    // }
                    // else if(x == ROOM_GRID_WIDTH-1 && y == ROOM_GRID_HEIGHT / 2) {
                    //     isDoorPosition = true;
                    // }
                    
                    // if(isDoorPosition) {
                    //     grid[y][x] = TileType::DOOR;
                    // } else {
                    // }
                    grid[y][x] = TileType::WALL;
                }
            }
        }
    }

    structure.grid = grid;
}

void Room::GenerateEnemySpawns() {
    
    switch (type) {
        case RoomType::START:
            if (GetRandomValue(0, 100) < 30) {
                int count = GetRandomValue(1, 2);
                for (int i = 0; i < count; i++) {
                    float x = static_cast<float>(GetRandomValue(100, static_cast<int>(bounds.width) - 100));
                    float y = static_cast<float>(GetRandomValue(100, static_cast<int>(bounds.height) - 100));
                    enemySpawns.push_back(EnemySpawn{Vector2{x, y}, EnemyType::NORMAL});
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
                    
                    enemySpawns.push_back(EnemySpawn{Vector2{x, y}, type});
                }
            }
            break;
            
        case RoomType::BOSS:
            {
                enemySpawns.push_back(EnemySpawn{
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
                    
                    enemySpawns.push_back(EnemySpawn{Vector2{x, y}, EnemyType::NORMAL});
                }
            }
            break;
            
        case RoomType::TREASURE:
            if (GetRandomValue(0, 100) < 60) {
                int count = GetRandomValue(1, 3);
                for (int i = 0; i < count; i++) {
                    float x = static_cast<float>(GetRandomValue(100, static_cast<int>(bounds.width) - 100));
                    float y = static_cast<float>(GetRandomValue(100, static_cast<int>(bounds.height) - 100));
                    enemySpawns.push_back(EnemySpawn{Vector2{x, y}, EnemyType::TANK});
                }
            }
            break;
            
        case RoomType::SECRET:
            if (GetRandomValue(0, 100) < 40) {
                int count = GetRandomValue(1, 2);
                for (int i = 0; i < count; i++) {
                    float x = static_cast<float>(GetRandomValue(50, static_cast<int>(bounds.width) - 50));
                    float y = static_cast<float>(GetRandomValue(50, static_cast<int>(bounds.height) - 50));
                    enemySpawns.push_back(EnemySpawn{Vector2{x, y}, EnemyType::FAST});
                }
            }
            break;
            
        case RoomType::EMPTY:
        default:
            break;
    }
    
}
