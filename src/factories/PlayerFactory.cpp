#include "PlayerFactory.hpp"
#include <raylib.h>

static constexpr float PLAYER_RADIUS = 20.0f;
static constexpr int PLAYER_MAX_HP = 100;
static constexpr float PLAYER_FIRE_RATE = 1000.0f;
static constexpr float PLAYER_BULLET_SPEED = BULLET_SPEED;
static constexpr float PLAYER_SHOOT_COOLDOWN = SHOOT_COOLDOWN;

int PlayerFactory::create(ECS& ecs, const Vector2 position) {
    int playerId = ecs.createEntity();
    ecs.addTransform(playerId, Vector2{static_cast<float>(position.x), static_cast<float>(position.y)});
    ecs.addVelocity(playerId, Vector2{0, 0});
    ecs.addRenderable(playerId, 20.0f, GREEN);
    ecs.addHealth(playerId, 100, 100);
    ecs.addShootable(playerId, 1000.0f, BULLET_SPEED, SHOOT_COOLDOWN);
    Sound shoot = LoadSound("resources/sounds/shoot.wav");
    ecs.addAudio(playerId, shoot);
    ecs.getEntities()[playerId].tags = EntityTag::PLAYER;
    return playerId;
}

