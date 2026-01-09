#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <string>

class ECS;
class Map;

class RenderSystem {
private:
    static void renderHealthbar(const ECS& ecs, int entityId);
    static void renderMinimap(const Map& map, int screenX, int screenY);
    static void renderECS(const ECS&);
    
public:
    static void render(const ECS& ecs, const Map &map);
    static void renderUI(int screenWidth, int screenHeight);
    static void setPickedUpItemName(const std::string& name);
};

#endif // RENDER_SYSTEM_HPP

