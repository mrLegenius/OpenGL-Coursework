#pragma once
#include <memory>
#include "Texture.h"

struct PerlinNoiseData
{
	float scale = 1.0f;
	int octaves = 2;
	float persistence = 0.5f;
	float lacunarity = 0.5f;
	float exp = 1.0f;
	float islandModifier = 1.0f;
	float oceanReachRate = 1.0f;
	float islandHeight = 1.0f;
};

class TextureGenerator
{
	static GLubyte toRGB(const float& pixel);
	static GLfloat toNormalizedRGB(const GLfloat& pixel);

	static float getHeight(const GLfloat* data, const unsigned& width, const unsigned& height, int row, int column);
	static int clamp(const int& value, const int& max);
	static float map(float value, float min1, float max1, float min2, float max2);
	static glm::vec3 calcNormal(const GLfloat* data, const unsigned& width, const unsigned& height, const int& row, const int& column);
public:
	static std::shared_ptr<Texture> GenerateHeightMap(PerlinNoiseData data, glm::ivec2 textureSize = glm::ivec2(128), unsigned long long seed = 0, glm::vec2 offset = glm::vec2(0));
	static std::shared_ptr<Texture> ApplyCookie(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> cookie);
	static std::shared_ptr<Texture> GenerateNormalMapFromTexture(std::shared_ptr<Texture> texture);
};

