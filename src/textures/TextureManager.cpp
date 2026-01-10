#include "TextureManager.hpp"

Texture2D& TextureManager::Get(std::string texturePath){
    static std::unordered_map<std::string, Texture2D> textures;

    std::string path = "resources/textures/" + texturePath;

    if (textures.count(path) == 0){
        textures[path] = LoadTexture(path.c_str());
    }

    return textures[path];
}