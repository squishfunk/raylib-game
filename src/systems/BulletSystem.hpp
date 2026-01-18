#ifndef BULLET_SYSTEM_HPP
#define BULLET_SYSTEM_HPP

class ECS;
class Map;

class BulletSystem {
public:
    static void update(ECS& ecs, Map& map);
};

#endif // BULLET_SYSTEM_HPP

