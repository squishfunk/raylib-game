#ifndef DEBUG_SYSTEM_HPP
#define DEBUG_SYSTEM_HPP

class ECS;
class Map;
class Dungeon;

class DebugSystem {
public:
    static void update(ECS& ecs, Map& map, Dungeon* dungeon, int playerId);
    static void render(const ECS& ecs, const Map& map, int playerId, int screenWidth, int screenHeight);
    
private:
    static void handleDebugInput(ECS& ecs, Map& map, Dungeon* dungeon, int playerId);
    static void killAllEnemies(ECS& ecs);
    static void healPlayer(ECS& ecs, int playerId);
    static void clearCurrentRoom(Map& map);
    static void teleportToBoss(Map& map, Dungeon* dungeon);
};

#endif 