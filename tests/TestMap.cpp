#include <gtest/gtest.h>
#include <vector>
#include <array>
#include "../src/map/Map.hpp"

namespace MapTestHelpers {
    bool dfs(const Map& map, int x, int y, bool visited[MAP_HEIGHT][MAP_WIDTH]) {
        std::vector<std::array<int, 2>> possibleDirections{};

        Room room = map.getRoom(x, y);

        if ((room.doors & DoorFlags::UP) == DoorFlags::UP) {
            possibleDirections.push_back({0, -1});
        }

        if ((room.doors & DoorFlags::DOWN) == DoorFlags::DOWN){
            possibleDirections.push_back({0, 1});
        }
        
        if ((room.doors & DoorFlags::LEFT) == DoorFlags::LEFT){
            possibleDirections.push_back({-1, 0});
        }
        if ((room.doors & DoorFlags::RIGHT) == DoorFlags::RIGHT){
            possibleDirections.push_back({1, 0});
        }


        for (int i = 0; i < (int)possibleDirections.size(); i++){
            int newX = x + possibleDirections[i][0];
            int newY = y + possibleDirections[i][1];

            if(newX >= MAP_WIDTH || newX < 0 || newY >= MAP_HEIGHT || newY < 0 || visited[newY][newX]){
                continue;
            }

            visited[newY][newX] = true;

            Room newRoom = map.getRoom(newX, newY);

            if(newRoom.type == RoomType::BOSS){
                return true;
            }

            if(newRoom.type == RoomType::NORMAL){
                if(MapTestHelpers::dfs(map, newX, newY, visited)){
                    return true;
                }
            }
        }
        return false;
    }
}

TEST(MapTest, PathGenerateAtLeastMinRooms){
    RoomCord startRoom = {2,2};
    std::vector<RoomCord> path = Map::GeneratePath(startRoom);

    EXPECT_GE(path.size(), MIN_MAP_PATH_LENGTH) << "Path generation should generate at least: " << MIN_MAP_PATH_LENGTH << " rooms, Generated: " << path.size();
}

TEST(MapTest, MapGenerate) {
    Map map;
    map.init();
    map.generate();

    EXPECT_EQ(map.getCurrentX(), map.getStartX());
    EXPECT_EQ(map.getCurrentY(), map.getStartY());
    EXPECT_TRUE(map.isGenerated());
}


TEST(MapTest, MapGenerateTreasureRoom) {
    Map map;

    bool foundTreasure = false;
    for(int i = 0; i < 100; i++){
        map.init();
        map.generate();
        foundTreasure = false;
        for(int x = 0; x < MAP_WIDTH; x++){
            for(int y = 0; y < MAP_HEIGHT; y++){
                if (map.getRoom(x, y).type == RoomType::TREASURE) {
                    foundTreasure = true;
                    break;
                }
            }
        }
        ASSERT_TRUE(foundTreasure) 
            << "Attempt " << i << ": Map should have at least one TREASURE room";
    }
}

TEST(MapTest, RoomsAreConnected) {
    Map map;
    map.init();
    map.generate();

    int startX = map.getStartX();
    int startY = map.getStartY();

    bool visited[MAP_HEIGHT][MAP_WIDTH] = {false}; 

    EXPECT_TRUE(MapTestHelpers::dfs(map, startX, startY, visited)) << "Cannot pass from Start room to BOSS";
}