#ifndef MAP_HPP
#define MAP_HPP

#include <raylib.h>
#include <cstdint>
#include <set>
#include <vector>
#include "../components/Components.hpp"


class Game;

constexpr int MAP_WIDTH = 5;
constexpr int MAP_HEIGHT = 5;
constexpr int MIN_MAP_PATH_LENGTH = 8;

enum class RoomType {
    EMPTY,
    START,
    NORMAL,
    BOSS,
    TREASURE,
    SECRET
};

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

struct EnemySpawn {
    Vector2 position;
    EnemyType type;
    
    EnemySpawn(Vector2 pos, EnemyType t) : position(pos), type(t) {}
};

struct Room {
    RoomType type;
    bool visited;
    bool cleared;
    DoorFlags doors;
    int gridX, gridY;
    Rectangle bounds;
    std::vector<EnemySpawn> enemySpawns;
    
    Room() : 
    type(RoomType::EMPTY), 
    visited(false), cleared(false), 
    doors(DoorFlags::NONE), 
    gridX(0), 
    gridY(0), 
    bounds({0, 0, 0, 0}),
    enemySpawns{}
    {

    }
};

struct RoomCord {
    int x;
    int y;

    bool operator==(const RoomCord& other) const {
        return x == other.x && y == other.y;
    }
};

class Map {
private:
    Room rooms[MAP_HEIGHT][MAP_WIDTH];
    int startX, startY;
    int currentX, currentY;
    bool generated;
    
    void connectRooms(int x1, int y1, int x2, int y2);
    static std::vector<EnemySpawn> generateEnemySpawns(RoomType roomType, const Rectangle& bounds);
    
public:
    Map();
    
    void init();
    void generate();
    void renderMinimap(int screenX, int screenY) const;
    // void renderCurrentRoom(int screenWidth, int screenHeight) const;
    bool canMoveTo(int newX, int newY) const;
    void moveToRoom(Game& game, int newX, int newY);
    
    int getStartX() const { return startX; }
    int getStartY() const { return startY; }
    
    int getCurrentX() const { return currentX; }
    int getCurrentY() const { return currentY; }
    Room& getCurrentRoom() { return rooms[currentX][currentY]; }
    void setCurrentRoom(int x, int y) { currentX = x; currentY = y; }
    const Room& getRoom(int x, int y) const { return rooms[y][x]; }
    Room& getRoom(int x, int y) { return rooms[y][x]; }
    bool isGenerated() const { return generated; }
    

    static std::vector<RoomCord> GeneratePath(RoomCord startRoom);
    static void generateRoom(Game& game); /*  TODO DELETE */
    static void checkRoomCleared(Game& game); /*  TODO DELETE */
};

#endif // MAP_HPP

