#include "DoorFactory.hpp"
#include <cstdio>
#include <raylib.h>

int DoorFactory::create(ECS& ecs, Vector2 position, DoorFlags doorFlag, bool opened) {
    if (doorFlag == DoorFlags::NONE) return -1;

    int doorId = ecs.createEntity();
    if (doorId < 0) return -1;

    float width = 100.0f; 
    float height = 100.0f;

    // position.x -= width/2;
    // position.y -= height/2;

    ecs.addTransform(doorId, position);
    ecs.addDoor(doorId, width, height, doorFlag, opened);
    ecs.addBoxCollider(doorId, width, height);
    Sound doorSound = LoadSound("resources/sounds/door.wav");
    ecs.addAudio(doorId, "DOOR_SOUND", doorSound);

    return doorId;
}