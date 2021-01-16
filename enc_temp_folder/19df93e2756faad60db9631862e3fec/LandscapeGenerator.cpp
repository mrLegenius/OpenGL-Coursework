#include "LandscapeGenerator.h"
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>


std::shared_ptr<Shape3D> LandscapeGenerator::Generate(std::shared_ptr<Texture> heightMap, float heightMultiplier)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	GLuint width = heightMap->GetWidth();
	GLuint height = heightMap->GetHeight();

	GLfloat* data = new GLfloat[width * height * sizeof(GLfloat)]; 

	float cornerX = width / -2.0f - (width % 2 ? 0.5f : 0);
	float cornerZ = height / 2.0f + (height % 2 ? 0.5f : 0);;
	
	int vertexIndex = 0;
	heightMap->GetPixels(data);

	const int heightModifiersCount = 8;
	struct heightModifier
	{
		float modifier;
		float height;
	};
	heightModifier heightModifiers[heightModifiersCount] = {
		{ 0.0f, 0.2f }, //DEEP WATER
		{ 0.1f, 0.4f }, //WATER
		{ 0.3f, 0.45f }, //SAND
		{ 0.6f, 0.55f }, //GRASSLAND
		{ 0.8f, 0.6f }, //HIGH GRASS
		{ 0.9f, 0.7f }, //HILL
		{ 0.95f, 0.9f }, //MOUNTAIN
		{ 1.0f, 1.0f } //PEAK
	};

	std::cout << "size of landscape = " << width << "x" << height << std::endl;
	std::cout << "corner = " << cornerX << "," << cornerZ << std::endl;
	for (GLuint x = 0; x < width; x++)
	{
		for (GLuint y = 0; y < height; y++, vertexIndex++)
		{
			vertices.push_back(cornerX + x);

			GLuint index = (x + y * width) * 4;
			
			float modifier = 1.0f;
			/*
			for (int i = 0; i < heightModifiersCount; i++)
			{
				if (data[index] > heightModifiers[i].height) continue;

				float a = i ? heightModifiers[i-1].modifier : 0;
				float b = heightModifiers[i].modifier;
				float d = 1.0f - (heightModifiers[i].height - data[index]);
				//modifier = (b - a) * d + a;

				//modifier = (modifier == 0.0) ? modifier : pow(2, 10 * (modifier - 1));
				//modifier = heightModifiers[i].modifier;
				break;
			}*/
			GLfloat resultHeight = data[index] * heightMultiplier * modifier;
			vertices.push_back(resultHeight);

			vertices.push_back(cornerZ - y - 1);

			//Default Normals
			vertices.push_back(0);
			vertices.push_back(0);
			vertices.push_back(1);

			//texture coords
			vertices.push_back(x / (float)width);
			vertices.push_back(y / (float)height);

			//indices
			if (x < width - 1 && y < height - 1)
			{
				indices.push_back(vertexIndex);
				indices.push_back(vertexIndex + width + 1);
				indices.push_back(vertexIndex + width);

				indices.push_back(vertexIndex + width + 1);
				indices.push_back(vertexIndex);
				indices.push_back(vertexIndex + 1);
			}
		}
	}
	//Calculate Normals
	for (GLuint i = 0; i < indices.size(); i += 3)
	{
		int i1 = indices[i];
		int i2 = indices[i+1];
		int i3 = indices[i+2];

		glm::vec3 p1(vertices[i1], vertices[i1 + 1], vertices[i1 + 2]);
		glm::vec3 p2(vertices[i2], vertices[i2 + 1], vertices[i2 + 2]);
		glm::vec3 p3(vertices[i3], vertices[i3 + 1], vertices[i3 + 2]);

		glm::vec3 vec1(p2 - p1);
		glm::vec3 vec2(p3 - p1);

		glm::vec3 norm(glm::cross(vec1, vec2));

		vertices[i1 + 3] = norm.x;
		vertices[i1 + 4] = norm.y;
		vertices[i1 + 5] = norm.z;

		vertices[i2 + 3] = norm.x;
		vertices[i2 + 4] = norm.y;
		vertices[i2 + 5] = norm.z;

		vertices[i3 + 3] = norm.x;
		vertices[i3 + 4] = norm.y;
		vertices[i3 + 5] = norm.z;
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
