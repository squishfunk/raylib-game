#pragma once

#include "components/Components.hpp"
#include <cstdint>
#include <raylib.h>

constexpr int ROOM_GRID_WIDTH = 11;
constexpr int ROOM_GRID_HEIGHT = 11;
constexpr float TILE_SIZE = 100.0f;

enum class DoorFlags : uint8_t {
    NONE = 0,
    UP = 1 << 0,
    DOWN = 1 << 1,
    LEFT = 1 << 2,
    RIGHT = 1 << 3
};

inline DoorFlags operator|(DoorFlags a, DoorFlags b) {
    return static_cast<DoorFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline DoorFlags operator&(DoorFlags a, DoorFlags b) {
    return static_cast<DoorFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline DoorFlags& operator|=(DoorFlags& a, DoorFlags b) {
    a = a | b;
    return a;
}


enum class RoomType {
    EMPTY,
    START,
    NORMAL,
    BOSS,
    TREASURE,
    SECRET
};

enum class RoomStructureType {
    BOX,
    L_SHAPE
};

enum class TileType {
    EMPTY,
    WALL,
    DOOR_UP,
    DOOR_RIGHT,
    DOOR_DOWN,
    DOOR_LEFT,
};

struct RoomStructure {
    int gridWidth;
    int gridHeight;
    RoomStructureType structureType;
    std::vector<std::vector<TileType>> grid;
};

struct RoomCord {
    int x;
    int y;

    bool operator==(const RoomCord& other) const {
        return x == other.x && y == other.y;
    }
};

struct EnemySpawn {
    Vector2 position;
    EnemyType type;
    
    EnemySpawn(Vector2 pos, EnemyType t) : position(pos), type(t) {}
};

struct Room {
    RoomType type;
    RoomStructure structure;
    bool visited;
    bool cleared;
    DoorFlags doors;
    int gridX, gridY;
    Rectangle bounds; /* TODO to delete */
    std::vector<EnemySpawn> enemySpawns;
    
    Room() : 
    type(RoomType::EMPTY), 
    structure{},
    visited(false), cleared(false), 
    doors(DoorFlags::NONE), 
    gridX(0), 
    gridY(0), 
    bounds({0, 0, 0, 0}),
    enemySpawns{}
    {

    }

    void GenerateEnemySpawns();
    void GenerateRoomShape();
};