#include "DoorFactory.hpp"
#include <cstdio>

static constexpr int SCREEN_WIDTH = 1000;
static constexpr int SCREEN_HEIGHT = 650;
static constexpr int DOOR_WIDTH = 80.0f;
static constexpr int DOOR_HEIGHT = 20.0f;

int DoorFactory::create(ECS& ecs, DoorFlags doorFlag) {
    if (doorFlag == DoorFlags::NONE) return -1;

    int doorId = ecs.createEntity();
    if (doorId < 0) return -1;

    Vector2 pos;
    float width = 0.0f; 
    float height = 0.0f;

    if(doorFlag == DoorFlags::UP){
        pos = {
            static_cast<float>(SCREEN_WIDTH) / 2.0f - DOOR_WIDTH / 2.0f, 
            0
        };
        width = 80.0f; height = 20.0f;

    }else if(doorFlag == DoorFlags::DOWN){
        pos = {
            static_cast<float>(SCREEN_WIDTH) / 2.0f - DOOR_WIDTH / 2.0f, 
            static_cast<float>(SCREEN_HEIGHT) - DOOR_HEIGHT
        };
        width = 80.0f; height = 20.0f;

    }else if(doorFlag == DoorFlags::LEFT){
        pos = {
            0,
            static_cast<float>(SCREEN_HEIGHT) / 2.0f - DOOR_WIDTH / 2.0f
        };
        width = 20.0f; height = 80.0f;

    }else if(doorFlag == DoorFlags::RIGHT){
        pos = {
            static_cast<float>(SCREEN_WIDTH) - DOOR_HEIGHT, 
            static_cast<float>(SCREEN_HEIGHT) / 2.0f - DOOR_WIDTH / 2.0f
        };
        width = 20.0f; height = 80.0f;
    }else{
        return -1; /* TODO */
    }

    ecs.addTransform(doorId, pos);
    ecs.addDoor(doorId, width, height, doorFlag);

    return doorId;
}