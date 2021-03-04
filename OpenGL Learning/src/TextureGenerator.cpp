#include "TextureGenerator.h"
#include "glm/gtc/noise.hpp"
#include <iostream>


std::shared_ptr<Texture> TextureGenerator::GenerateHeightMap(PerlinNoiseData data, glm::ivec2 textureSize, unsigned long long seed, glm::vec2 offset)
{
	

	srand(seed);
	GLfloat* texData = new GLfloat[textureSize.x * textureSize.y * sizeof(GLfloat)];

	glm::vec2 *octaveOffsets = new glm::vec2[data.octaves];
	
	for (int i = 0; i < data.octaves; i++)
	{
		float offsetX = rand() % 20000 - 10000;
		float offsetY = rand() % 20000 - 10000;

		octaveOffsets[i] = glm::vec2(offsetX, offsetY) + offset;
	}

	if (data.scale <= 1)
		data.scale = 1.00001f;

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
			
			for (int i = 0; i < data.octaves; i++)
			{
				float xCoord = (x - halfWidth) / data.scale * frequency + octaveOffsets[i].x;
				float yCoord = (y - halfHeight) / data.scale * frequency + octaveOffsets[i].y;

				//float xCoord = (x / textureSize.x - 0.5) / data.scale * frequency + octaveOffsets[i].x;
				//float yCoord = (y / textureSize.y - 0.5) / data.scale * frequency + octaveOffsets[i].y;


				float perlinValue = glm::perlin(glm::vec2(xCoord, yCoord));
				
				noiseHeight += perlinValue * amplitude;
				
				amplitude *= data.persistence;
				frequency *= data.lacunarity;
			}
			

			//std::cout << noiseHeight << std::endl;
			//if (noiseHeight > maxNoiseHeight)
			//	maxNoiseHeight = noiseHeight;
			//else if (noiseHeight < minNoiseHeight)
			//	minNoiseHeight = noiseHeight;
			
			unsigned int index = ((int)y * textureSize.x + (int)x) * 4;
			texData[index + 0] = noiseHeight;
			texData[index + 1] = noiseHeight;
			texData[index + 2] = noiseHeight;
			texData[index + 3] = 1.0f;
		}
	/*CLAMPING HEIGHT TO 0-1 RANGE*/
	/*
	
	for (float y = 0.0f; y < textureSize.y; y++)
		for (float x = 0.0f; x < textureSize.x; x++)
		{
			float nx = x / textureSize.x - 0.5f;
			float ny = y / textureSize.y - 0.5f;

			unsigned int index = ((int)y * textureSize.x + (int)x) * 4;
			float value = (texData[index] - minNoiseHeight) / (maxNoiseHeight - minNoiseHeight);
			//std::cout << "Height before exp " << value;


			float distance = glm::sqrt(nx * nx + ny * ny) * glm::sqrt(2.0f);

			
			//value *= data.islandHeight * glm::pow(data.oceanReachRate - distance, data.islandModifier);

			value = glm::pow(value, data.exp); 
			//std::cout << " Height after exp " << value << std::endl;
			//value = (1 + value - distance) / 2;
			//std::cout << "value after tweaking " << value << std::endl;
			texData[index + 0] = value;
			texData[index + 1] = value;
			texData[index + 2] = value;
			texData[index + 3] = 1.0f;
		}
	*/
	auto texture = std::make_shared<Texture>(texData, textureSize.x, textureSize.y);

	delete[] octaveOffsets;
	delete[] texData;
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

	for (unsigned w = 0; w < width; w++)
	{
		for (unsigned h = 0; h < height; h++)
		{
			glm::vec3 normal = calcNormal(pixels, width, height, w, h);

			unsigned index = (w + h * width) * 4;
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

