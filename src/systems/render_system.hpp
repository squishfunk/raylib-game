#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

class ECS;

class RenderSystem {
private:
    static void renderHealthbar(const ECS& ecs, int entityId);
    
public:
    static void render(const ECS& ecs);
};

#endif // RENDER_SYSTEM_HPP

