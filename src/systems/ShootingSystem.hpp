#ifndef SHOOTING_SYSTEM_HPP
#define SHOOTING_SYSTEM_HPP

class ECS;
class Map;

class ShootingSystem {
private:
    static void shoot(ECS &ecs);
public:
    static void update(ECS& ecs);
};

#endif // ROOM_SYSTEM_HPP