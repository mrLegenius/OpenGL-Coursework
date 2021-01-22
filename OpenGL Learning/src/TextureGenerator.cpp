#include "TextureGenerator.h"
#include "glm/gtc/noise.hpp"
#include <iostream>
std::shared_ptr<Texture> TextureGenerator::GenerateHeightMap(glm::ivec2 textureSize, float scale, int octaves, float persistence, float lacunarity, float exp, unsigned long long seed, glm::vec2 offset)
{
	srand(seed);
	GLfloat* data = new GLfloat[textureSize.x * textureSize.y * sizeof(GLfloat)];

	glm::vec2 *octaveOffsets = new glm::vec2[octaves];
	
	for (int i = 0; i < octaves; i++)
	{
		float offsetX = rand() % 20000 - 10000;
		float offsetY = rand() % 20000 - 10000;

		octaveOffsets[i] = glm::vec2(offsetX, offsetY) + offset;
	}

	if (scale <= 1)
		scale = 1.00001f;

	float maxNoiseHeight = -99999999.0f;
	float minNoiseHeight =  99999999.0f;
	
	float halfWidth = textureSize.x / 2.0f;
	float halfHeight = textureSize.y / 2.0f;

	for (float y = 0.0f; y < textureSize.y; y++)
		for (float x = 0.0f; x < textureSize.x; x++)
		{
			float amplitude = 1;
			float frequency = 1;
			float noiseHeight = 0;

			for (int i = 0; i < octaves; i++)
			{
				float xCoord = (x - halfWidth) / scale * frequency + octaveOffsets[i].x;
				float yCoord = (y - halfHeight) / scale * frequency + octaveOffsets[i].y;

				//float xCoord = (x / textureSize.x - 0.5) / scale * frequency + octaveOffsets[i].x;
				//float yCoord = (y / textureSize.y - 0.5) / scale * frequency + octaveOffsets[i].y;
				//std::cout << "Perlin" << glm::perlin(glm::vec2(xCoord, yCoord)) * 0.5 + 0.5 << std::endl;
				float perlinValue = glm::perlin(glm::vec2(xCoord, yCoord)) * 0.5 + 0.5;
				noiseHeight += perlinValue * amplitude;
				
				amplitude *= persistence;
				frequency *= lacunarity;
			}
			//std::cout << "Height before exp " << noiseHeight;
			noiseHeight = glm::pow(noiseHeight, exp);
			//std::cout << " Height after exp " << noiseHeight << std::endl;

			if (noiseHeight > maxNoiseHeight)
				maxNoiseHeight = noiseHeight;
			else if (noiseHeight < minNoiseHeight)
				minNoiseHeight = noiseHeight;
			
			
			unsigned int index = ((int)y * textureSize.x + (int)x) * 4;
			data[index + 0] = noiseHeight;
			data[index + 1] = noiseHeight;
			data[index + 2] = noiseHeight;
			data[index + 3] = 1.0f;
		}
	/*
	for (float y = 0.0f; y < textureSize.y; y++)
		for (float x = 0.0f; x < textureSize.x; x++)
		{
			unsigned int index = ((int)y * textureSize.x + (int)x) * 4;
			float value = 0;
			if (data[index] != 0)
				value = (data[index] - minNoiseHeight) / (maxNoiseHeight - minNoiseHeight);

			data[index + 0] = value;
			data[index + 1] = value;
			data[index + 2] = value;
			data[index + 3] = 1.0f;
		}*/

	auto texture = std::make_shared<Texture>(data, textureSize.x, textureSize.y);

	delete[] octaveOffsets;
	delete[] data;
	return texture;
}
std::shared_ptr<Texture> TextureGenerator::ApplyCookie(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> cookie)
{
	GLuint width = texture->GetWidth();
	GLuint height = texture->GetHeight();

	GLuint cookieWidth = cookie->GetWidth();
	GLuint cookieHeight = cookie->GetHeight();

	GLfloat* pixels = new GLfloat[cookieWidth * cookieHeight * sizeof(GLfloat)];
	GLfloat* data = new GLfloat[width * height * sizeof(GLfloat)];
	cookie->GetPixels(pixels);
	texture->GetPixels(data);

	for (unsigned w = 0; w < width; ++w)
	{
		for (unsigned h = 0; h < height; ++h)
		{
			float ix = map(w, 0, width, 0, cookieWidth);
			float iy = map(h, 0, height, 0, cookieHeight);
			float cookieValue = pixels[((int)iy * cookieWidth + (int)ix)*4];

			unsigned index = (w * width + h) * 4;
			data[index + 0] *= cookieValue;
			data[index + 1] *= cookieValue;
			data[index + 2] *= cookieValue;
			data[index + 3] = 1.0f;
		}
	}

	auto result = std::make_shared<Texture>(data, width, height);
	delete[] data;
	delete[] pixels;

	return result;
}
std::shared_ptr<Texture> TextureGenerator::GenerateNormalMapFromTexture(std::shared_ptr<Texture> texture)
{
	unsigned int width = texture->GetWidth();
	unsigned int height = texture->GetHeight();

	GLfloat *pixels = new GLfloat[width * height * sizeof(GLfloat)];
	GLfloat *data = new GLfloat[width * height * sizeof(GLfloat)];
	texture->GetPixels(pixels);

	for (unsigned w = 0; w < width; ++w)
	{
		for (unsigned h = 0; h < height; ++h)
		{
			glm::vec3 normal = calcNormal(pixels, width, height, w, h);

			unsigned index = (w * width + h) * 4;
			data[index] = toNormalizedRGB(normal.x);
			data[index + 1] = toNormalizedRGB(normal.y);
			data[index + 2] = toNormalizedRGB(normal.z);
			data[index + 3] = 1.0f;
		}
	}

	auto result = std::make_shared<Texture>(data, width, height);
	delete[] data;
	delete[] pixels;

	return result;
}

glm::vec3 TextureGenerator::calcNormal(const GLfloat* data, const unsigned& width, const unsigned& height, const int& row, const int& column)
{
	const float strength = 2.0f;

	float topLeft = getHeight(data, width, height, row - 1, column - 1);
	float top = getHeight(data, width, height, row - 1, column);
	float topRight = getHeight(data, width, height, row - 1, column + 1);
	float right = getHeight(data, width, height, row, column + 1);
	float bottomRight = getHeight(data, width, height, row + 1, column + 1);
	float bottom = getHeight(data, width, height, row + 1, column);
	float bottomLeft = getHeight(data, width, height, row + 1, column - 1);
	float left = getHeight(data, width, height, row, column - 1);

	float sobelX = (topRight + 2.0f * right + bottomRight) - (topLeft + 2.0f * left + bottomLeft);
	float sobelY = (bottomLeft + 2.0f * bottom + bottomRight) - (topLeft + 2.0f * top + topRight);
	float sobelZ = 1.0f / strength;

	glm::vec3 normal(sobelX, sobelY, sobelZ);
	return glm::normalize(normal);
}

GLubyte TextureGenerator::toRGB(const float& pixel)
{
	return (GLubyte)((pixel + 1.0f) * (255.0f / 2.0f));
}

GLfloat TextureGenerator::toNormalizedRGB(const GLfloat& pixel)
{
	return pixel * 0.5f + 0.5f;
}

float TextureGenerator::getHeight(const GLfloat* data, const unsigned& width, const unsigned& height, int row, int column)
{

	if (row >= width)  row %= width;
	while (row < 0)    row += width;
	if (column >= height) column %= height;
	while (column < 0)    column += height;

	return data[(column * width + row)* 4];
}

int TextureGenerator::clamp(const int& value, const int& max)
{
	if (value > max)
		return max;
	else if (value < 0)
		return 0;
	return value;
}

float TextureGenerator::map(float value, float min1, float max1, float min2, float max2) {
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

