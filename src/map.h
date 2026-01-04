#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include <stdbool.h>
#include "ecs.h"

#define MAP_WIDTH 5
#define MAP_HEIGHT 5

typedef struct Game Game;

typedef enum {
    ROOM_TYPE_EMPTY,
    ROOM_TYPE_START,
    ROOM_TYPE_NORMAL,
    ROOM_TYPE_BOSS,
    ROOM_TYPE_TREASURE,
    ROOM_TYPE_SECRET 
} RoomType;

typedef enum {
    DOOR_NONE = 0,
    DOOR_UP = 1 << 0,
    DOOR_DOWN = 1 << 1,
    DOOR_LEFT = 1 << 2,
    DOOR_RIGHT = 1 << 3
} DoorFlags;

typedef struct {
    RoomType type;
    bool visited;
    bool cleared;
    DoorFlags doors;
    
    int gridX, gridY;
} Room;

typedef struct {
    Room rooms[MAP_HEIGHT][MAP_WIDTH];
    int startX, startY;
    int currentX, currentY;
    bool generated;
} Map;

void map_init(Map *map);
void map_generate(Map *map);
void map_render_minimap(Map *map, int screenX, int screenY);
void map_render_current_room(Map *map);
bool map_can_move_to(Map *map, int newX, int newY);
void map_move_to_room(Game *game, int newX, int newY);

#endif