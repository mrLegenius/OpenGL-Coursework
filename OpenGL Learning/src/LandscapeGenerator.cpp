#include "LandscapeGenerator.h"
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

std::shared_ptr<Shape3D> LandscapeGenerator::Generate(GLuint dimensions, float amplitude, int frequency)
{

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	GLuint half = dimensions / 2;

	float size = 1.f / dimensions;
	float halfSize = half * size;

	unsigned int index = 0;
	for (GLuint i = 0; i < dimensions; i++)
	{
		for (GLuint j = 0; j < dimensions; j++)
		{
			vertices.push_back(i * size - halfSize);
			vertices.push_back(j * size - halfSize);
			//vertices.push_back(ValueNoise_2D(i, j) * amplitude);

			vertices.push_back(0);
			vertices.push_back(0);
			vertices.push_back(1);

			vertices.push_back(i % 2);
			vertices.push_back(j % 2);
		}
	}

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

	return std::make_shared<Shape3D>(vertices, indices, layout);
}
