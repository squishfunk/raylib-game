#ifndef BULLET_SYSTEM_HPP
#define BULLET_SYSTEM_HPP

class ECS;

class BulletSystem {
public:
    static void update(ECS& ecs, int screenWidth, int screenHeight);
};

#endif // BULLET_SYSTEM_HPP

