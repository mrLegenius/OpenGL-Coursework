#pragma once
#include <memory>
#include "Texture.h"

class TextureGenerator
{
	static GLubyte toRGB(const float& pixel);
	static GLfloat toNormalizedRGB(const GLfloat& pixel);

	static float getHeight(const GLfloat* data, const unsigned& width, const unsigned& height, int row, int column);
	static int clamp(const int& value, const int& max);
	static glm::vec3 calcNormal(const GLfloat* data, const unsigned& width, const unsigned& height, const int& row, const int& column);
public:
	static std::shared_ptr<Texture> GenerateHeightMap(glm::ivec2 textureSize = glm::ivec2(256), GLuint octaves = 8, float persistence = 0.5f, float scale = 1000, unsigned long long seed = 0);
	static std::shared_ptr<Texture> GenerateNormalMapFromTexture(std::shared_ptr<Texture> texture);
	
};

