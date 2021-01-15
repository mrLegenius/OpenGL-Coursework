#pragma once
#include <memory>
#include "Texture.h"

class TextureGenerator
{
public:
	static std::shared_ptr<Texture> GenerateHeightMap(glm::ivec2 textureSize, GLuint octaves = 8, float persistence = 0.5f, float scale = 100, glm::vec2 offset = glm::vec2(0));
};

