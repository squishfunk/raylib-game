#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>

class TextureManager {
public:
    static Texture2D& Get(std::string texturePath);
};