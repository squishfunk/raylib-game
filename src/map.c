#include "map.h"
#include "ecs.h"
#include <string.h>

void map_init(Map *map){
    memset(map, 0, sizeof(Map));
    map->generated = false;
}


static void map_connect_rooms(Map *map, int x1, int y1, int x2, int y2){
    Room *room1 = &map->rooms[y1][x1];
    Room *room2 = &map->rooms[y2][x2];
    
    if(x2 > x1){
        room1->doors |= DOOR_RIGHT;
        room2->doors |= DOOR_LEFT;
    } else if(x2 < x1){
        room1->doors |= DOOR_LEFT;
        room2->doors |= DOOR_RIGHT;
    }
    
    if(y2 > y1){
        room1->doors |= DOOR_DOWN;
        room2->doors |= DOOR_UP;
    } else if(y2 < y1){
        room1->doors |= DOOR_UP;
        room2->doors |= DOOR_DOWN;
    }
}

void map_generate(Map *map){
    map_init(map);
    
    int pathLength = 8 + GetRandomValue(0, 5);
    int currentX = MAP_WIDTH / 2;
    int currentY = MAP_HEIGHT / 2;
    
    map->startX = currentX;
    map->startY = currentY;
    map->currentX = currentX;
    map->currentY = currentY;
    
    map->rooms[currentY][currentX].type = ROOM_TYPE_START;
    map->rooms[currentY][currentX].gridX = currentX;
    map->rooms[currentY][currentX].gridY = currentY;
    map->rooms[currentY][currentX].visited = true;
    
    int prevX = currentX, prevY = currentY;
    
    for(int i = 1; i < pathLength; i++){
        int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
        int attempts = 0;
        int newX, newY;
        
        do {
            int dir = GetRandomValue(0, 3);
            newX = currentX + directions[dir][0];
            newY = currentY + directions[dir][1];
            attempts++;
        } while((newX < 0 || newX >= MAP_WIDTH || 
                 newY < 0 || newY >= MAP_HEIGHT ||
                 map->rooms[newY][newX].type != ROOM_TYPE_EMPTY) && attempts < 20);
        
        if(attempts < 20){
            map_connect_rooms(map, currentX, currentY, newX, newY);
            
            prevX = currentX;
            prevY = currentY;
            currentX = newX;
            currentY = newY;
            
            if(i == pathLength - 1){
                map->rooms[currentY][currentX].type = ROOM_TYPE_BOSS;
            } else {
                map->rooms[currentY][currentX].type = ROOM_TYPE_NORMAL;
            }
            map->rooms[currentY][currentX].gridX = currentX;
            map->rooms[currentY][currentX].gridY = currentY;
        }
    }
    
    int extraRooms = GetRandomValue(2, 5);
    for(int e = 0; e < extraRooms; e++){
        int baseX, baseY;
        int attempts = 0;
        do {
            baseX = GetRandomValue(0, MAP_WIDTH - 1);
            baseY = GetRandomValue(0, MAP_HEIGHT - 1);
            attempts++;
        } while(map->rooms[baseY][baseX].type == ROOM_TYPE_EMPTY && attempts < 50);
        
        if(attempts < 50){
            int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
            int dir = GetRandomValue(0, 3);
            int newX = baseX + directions[dir][0];
            int newY = baseY + directions[dir][1];
            
            if(newX >= 0 && newX < MAP_WIDTH && 
               newY >= 0 && newY < MAP_HEIGHT &&
               map->rooms[newY][newX].type == ROOM_TYPE_EMPTY){
                
                map_connect_rooms(map, baseX, baseY, newX, newY);
                map->rooms[newY][newX].type = ROOM_TYPE_NORMAL;
                map->rooms[newY][newX].gridX = newX;
                map->rooms[newY][newX].gridY = newY;
            }
        }
    }
    
    map->generated = true;
}

void map_render_minimap(Map *map, int screenX, int screenY){
    int cellSize = 15;
    int offsetX = screenX;
    int offsetY = screenY;
    
    int minimapWidth = MAP_WIDTH * cellSize + 4;
    int minimapHeight = MAP_HEIGHT * cellSize + 4;
    
    DrawRectangle(offsetX - 2, offsetY - 2, minimapWidth, minimapHeight, WHITE);
    DrawRectangleLines(offsetX - 2, offsetY - 2, minimapWidth, minimapHeight, BLACK);
    
    for(int y = 0; y < MAP_HEIGHT; y++){
        for(int x = 0; x < MAP_WIDTH; x++){
            Room *room = &map->rooms[y][x];
            
            if(room->type == ROOM_TYPE_EMPTY) continue;
            
            int px = offsetX + x * cellSize;
            int py = offsetY + y * cellSize;
            
            Color color = GRAY;
            if(room->type == ROOM_TYPE_START) color = GREEN;
            else if(room->type == ROOM_TYPE_BOSS) color = RED;
            else if(room->type == ROOM_TYPE_TREASURE) color = GOLD;
            else if(room->visited) color = LIGHTGRAY;
            else color = DARKGRAY;
            
            if(x == map->currentX && y == map->currentY){
                color = YELLOW;
            }
            
            DrawRectangle(px, py, cellSize - 2, cellSize - 2, color);
            
            if(room->doors & DOOR_UP) DrawLine(px + cellSize/2, py, px + cellSize/2, py - 3, WHITE);
            if(room->doors & DOOR_DOWN) DrawLine(px + cellSize/2, py + cellSize, px + cellSize/2, py + cellSize + 3, WHITE);
            if(room->doors & DOOR_LEFT) DrawLine(px, py + cellSize/2, px - 3, py + cellSize/2, WHITE);
            if(room->doors & DOOR_RIGHT) DrawLine(px + cellSize, py + cellSize/2, px + cellSize + 3, py + cellSize/2, WHITE);
        }
    }
}

void map_render_current_room(Map *map){
    Room *room = &map->rooms[map->currentY][map->currentX];
    
    int wallThickness = 20;
    
    if(!(room->doors & DOOR_UP)){
        DrawRectangle(0, 0, SCREEN_WIDTH, wallThickness, DARKGRAY);
    } else {
        DrawRectangle(SCREEN_WIDTH/2 - 50, 0, 100, wallThickness, BROWN);
    }
    
    if(!(room->doors & DOOR_DOWN)){
        DrawRectangle(0, SCREEN_HEIGHT - wallThickness, SCREEN_WIDTH, wallThickness, DARKGRAY);
    } else {
        DrawRectangle(SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT - wallThickness, 100, wallThickness, BROWN);
    }
    
    if(!(room->doors & DOOR_LEFT)){
        DrawRectangle(0, 0, wallThickness, SCREEN_HEIGHT, DARKGRAY);
    } else {
        DrawRectangle(0, SCREEN_HEIGHT/2 - 50, wallThickness, 100, BROWN);
    }
    
    if(!(room->doors & DOOR_RIGHT)){
        DrawRectangle(SCREEN_WIDTH - wallThickness, 0, wallThickness, SCREEN_HEIGHT, DARKGRAY);
    } else {
        DrawRectangle(SCREEN_WIDTH - wallThickness, SCREEN_HEIGHT/2 - 50, wallThickness, 100, BROWN);
    }
}

bool map_can_move_to(Map *map, int newX, int newY){
    if(newX < 0 || newX >= MAP_WIDTH || newY < 0 || newY >= MAP_HEIGHT) return false;
    
    Room *currentRoom = &map->rooms[map->currentY][map->currentX];
    Room *targetRoom = &map->rooms[newY][newX];
    
    if(targetRoom->type == ROOM_TYPE_EMPTY) return false;
    
    if(newX > map->currentX && !(currentRoom->doors & DOOR_RIGHT)) return false;
    if(newX < map->currentX && !(currentRoom->doors & DOOR_LEFT)) return false;
    if(newY > map->currentY && !(currentRoom->doors & DOOR_DOWN)) return false;
    if(newY < map->currentY && !(currentRoom->doors & DOOR_UP)) return false;
    
    return true;
}

void map_move_to_room(Map *map, int newX, int newY){
    if(!map_can_move_to(map, newX, newY)) return;
    
    map->currentX = newX;
    map->currentY = newY;
    map->rooms[newY][newX].visited = true;
}

