#ifndef MAP_HPP
#define MAP_HPP

#include <raylib.h>
#include <cstdint>

class Game;

constexpr int MAP_WIDTH = 5;
constexpr int MAP_HEIGHT = 5;

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

struct Room {
    RoomType type;
    bool visited;
    bool cleared;
    DoorFlags doors;
    int gridX, gridY;
    
    Room() : type(RoomType::EMPTY), visited(false), cleared(false), 
             doors(DoorFlags::NONE), gridX(0), gridY(0) {}
};

class Map {
private:
    Room rooms[MAP_HEIGHT][MAP_WIDTH];
    int startX, startY;
    int currentX, currentY;
    bool generated;
    
    void connectRooms(int x1, int y1, int x2, int y2);
    
public:
    Map();
    
    void init();
    void generate();
    void renderMinimap(int screenX, int screenY) const;
    void renderCurrentRoom() const;
    bool canMoveTo(int newX, int newY) const;
    void moveToRoom(Game& game, int newX, int newY);
    
    int getCurrentX() const { return currentX; }
    int getCurrentY() const { return currentY; }
    const Room& getRoom(int x, int y) const { return rooms[y][x]; }
    Room& getRoom(int x, int y) { return rooms[y][x]; }
    bool isGenerated() const { return generated; }
    
    static void generateRoom(Game& game);
    static void checkRoomCleared(Game& game);
};

#endif // MAP_HPP

