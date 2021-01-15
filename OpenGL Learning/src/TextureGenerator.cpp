#include "TextureGenerator.h"
#include "PerlinNoiseGenerator.h"

std::shared_ptr<Texture> TextureGenerator::GenerateHeightMap(glm::ivec2 textureSize, GLuint octaves, float persistence, float scale, glm::vec2 offset)
{
	PerlinNoiseGenerator gen(textureSize.x, textureSize.y, octaves, persistence);

	GLfloat* data = new GLfloat[textureSize.x * textureSize.y * sizeof(GLfloat)];
	// For each pixel in the texture...
	float y = 0.0f;

	while (y < textureSize.y)
	{
		float x = 0.0F;
		while (x < textureSize.x)
		{
			float xCoord = offset.x + x / textureSize.x * scale;
			float yCoord = offset.y + y / textureSize.y * scale;
			float sample = gen.ValueNoise_2D(xCoord, yCoord);

			GLfloat result = sample * 0.5f + 0.5f;
			unsigned int index = ((int)y * textureSize.x + (int)x) * 4;

			data[index + 0] = result;
			data[index + 1] = result;
			data[index + 2] = result;
			data[index + 3] = 1.0f;

			x++;
		}
		y++;
	}
	auto texture = std::make_shared<Texture>(data, textureSize.x, textureSize.y);

	delete[] data;
	return texture;
}