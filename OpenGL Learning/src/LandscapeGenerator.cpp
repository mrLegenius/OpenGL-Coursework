#include "LandscapeGenerator.h"
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>


std::shared_ptr<Shape3D> LandscapeGenerator::Generate(GLuint dimensions, std::shared_ptr<Texture> heightMap, float maxHeight)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	GLfloat* data = new GLfloat[heightMap->GetWidth() * heightMap->GetHeight() * sizeof(GLfloat)];

	GLuint width = heightMap->GetWidth();
	GLuint height = heightMap->GetHeight();

	GLuint half = dimensions / 2;

	float sizeX = 1.f / width;
	float sizeY = 1.f / height;
	float halfSizeX = width / 2 * sizeX;
	float halfSizeY = height / 2 * sizeY;

	
	heightMap->GetPixels(data);
	std::cout << "------------------------------------------"<< std::endl;
	for (GLuint i = 0; i < width; i++)
	{
		for (GLuint j = 0; j < height; j++)
		{
			vertices.push_back(i * sizeX - halfSizeX);
			vertices.push_back(j * sizeY - halfSizeY);

			GLuint index = (i + j * width) * 4;

			//std::cout << "height from map = " << data[index] << " | result height = " << data[index] * maxHeight << std::endl;
			GLfloat resultHeight = data[index];
			vertices.push_back(-resultHeight);
	
			vertices.push_back(0);
			vertices.push_back(0);
			vertices.push_back(1);

			vertices.push_back(i % 2);
			vertices.push_back(j % 2);
		}
	}
	std::cout << "------------------------------------------" << std::endl;
	for (GLuint row = 0; row < dimensions - 1; row++)
	{
		for (GLuint col = 0; col < dimensions - 1; col++)
		{
			indices.push_back(dimensions * row + col);
			indices.push_back(dimensions * row + col + dimensions);
			indices.push_back(dimensions * row + col + dimensions + 1);

			indices.push_back(dimensions * row + col);
			indices.push_back(dimensions * row + col + dimensions + 1);
			indices.push_back(dimensions * row + col + 1);
		}
	}

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);

	delete[] data;
	return std::make_shared<Shape3D>(vertices, indices, layout);
}

float LandscapeGenerator::map(float value, float min1, float max1, float min2, float max2) {
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
