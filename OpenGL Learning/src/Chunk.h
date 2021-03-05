#pragma once
#include "Shape3D.h"
#include <libnoiseutils/noiseutils.h>

class Chunk
{
public:
	Chunk(int x, int y) : x(x), y(y) { }

	inline int GetX() { return x; }
	inline int GetZ() { return y; }
	inline Shape3D& GetMesh() { return *mesh; }

	Shape3D& GenerateMesh(noise::utils::NoiseMap& heightMap)
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		GLuint width = heightMap.GetWidth();
		GLuint height = heightMap.GetHeight();

		GLfloat halfX = (width - 1) / 2.0f;
		GLfloat halfZ = (height - 1) / 2.0f;

		int vertexIndex = 0;

		for (GLuint x = 0; x < width; x++)
		{
			for (GLuint z = 0; z < height; z++, vertexIndex++)
			{
				vertices.push_back(x - halfX); // X

				GLfloat resultHeight = heightMap.GetValue(x, z);
				//std::cout << "Height is " << resultHeight << std::endl;
				vertices.push_back(resultHeight); // Y
				vertices.push_back(z - halfZ); // Z

				//Default Normals
				vertices.push_back(0);
				vertices.push_back(1);
				vertices.push_back(0);

				//texture coords
				vertices.push_back(x / (float)width);
				vertices.push_back(z / (float)height);

				//indices
				if (x < height - 1 && z < height - 1)
				{
					indices.push_back(vertexIndex);
					indices.push_back(vertexIndex + height + 1);
					indices.push_back(vertexIndex + height);

					indices.push_back(vertexIndex + height + 1);
					indices.push_back(vertexIndex);
					indices.push_back(vertexIndex + 1);
				}
			}
		}

		/* Calculate normals */
		for (int i = 0; i < indices.size(); i += 3)
		{
			auto index1 = indices[i] * 8;
			glm::vec3 p1(vertices[index1], vertices[index1 + 1], vertices[index1 + 2]);

			auto index2 = indices[i + 1] * 8;
			glm::vec3 p2(vertices[index2], vertices[index2 + 1], vertices[index2 + 2]);

			auto index3 = indices[i + 2] * 8;
			glm::vec3 p3(vertices[index3], vertices[index3 + 1], vertices[index3 + 2]);

			glm::vec3 vec1 = p2 - p1;
			glm::vec3 vec2 = p3 - p1;

			glm::vec3 norm = glm::normalize(glm::cross(vec1, vec2));

			vertices[index1 + 3] = vertices[index2 + 3] = vertices[index3 + 3] = norm.x;
			vertices[index1 + 4] = vertices[index2 + 4] = vertices[index3 + 4] = norm.y;
			vertices[index1 + 5] = vertices[index2 + 5] = vertices[index3 + 5] = norm.z;
		}

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		mesh = std::make_shared<Shape3D>(vertices, indices, layout);
		return *mesh;
	}

private:
	int x;
	int y;
	std::shared_ptr<Shape3D> mesh;
};

