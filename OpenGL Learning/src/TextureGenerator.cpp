#include "TextureGenerator.h"
#include "PerlinNoiseGenerator.h"

std::shared_ptr<Texture> TextureGenerator::GenerateHeightMap(glm::ivec2 textureSize, GLuint octaves, float persistence, float scale, unsigned long long seed)
{
	PerlinNoiseGenerator gen(textureSize.x, textureSize.y, octaves, persistence, seed);

	GLfloat* data = new GLfloat[textureSize.x * textureSize.y * sizeof(GLfloat)];
	srand(seed * 10);
	//glm::vec2 offset(rand() % 20000 - 10000, rand() % 20000 - 10000);
	glm::vec2 offset(0, 0);
	// For each pixel in the texture...
	float y = 0.0f;


	if (scale <= 0)
		scale = 0.00001f;

	while (y < textureSize.y)
	{
		float x = 0.0F;
		while (x < textureSize.x)
		{
			float xCoord = offset.x + x / (textureSize.x * scale);
			float yCoord = offset.y + y / (textureSize.y * scale);
			float sample = gen.ValueNoise_2D(xCoord, yCoord);

			GLfloat result = sample;// *0.5f + 0.5f;
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
	//return data[(clamp(row, width) * width + clamp(column, height)) * 4];
}

int TextureGenerator::clamp(const int& value, const int& max)
{
	if (value > max)
		return max;
	else if (value < 0)
		return 0;
	return value;
}

