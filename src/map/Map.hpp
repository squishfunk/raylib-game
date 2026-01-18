#ifndef MAP_HPP
#define MAP_HPP

#include <raylib.h>
#include <vector>
#include "../components/Components.hpp"
#include "Room.hpp"


class Game;

constexpr int MAP_WIDTH = 5;
constexpr int MAP_HEIGHT = 5;
constexpr int MIN_MAP_PATH_LENGTH = 8;

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
    // void renderCurrentRoom(int screenWidth, int screenHeight) const;
    bool canMoveTo(int newX, int newY) const;
    void moveToRoom(Game& game, int newX, int newY);
    
    int getStartX() const { return startX; }
    int getStartY() const { return startY; }
    int getCurrentX() const { return currentX; }
    int getCurrentY() const { return currentY; }
    Room& getCurrentRoom() { return rooms[currentY][currentX]; }
    void setCurrentRoom(int x, int y) { currentX = x; currentY = y; }
    const Room& getRoom(int x, int y) const { return rooms[y][x]; }
    Room& getRoom(int x, int y) { return rooms[y][x]; }
    bool isGenerated() const { return generated; }
    

    static std::vector<RoomCord> GeneratePath(RoomCord startRoom);
};

#endif // MAP_HPP

