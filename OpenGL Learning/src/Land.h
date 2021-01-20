#pragma once
#include <memory>
#include <string>

#include "glm/glm.hpp"

#include "Transform.h"

#include "Shader.h"
#include "Texture.h"
#include "Shape3D.h"

#include "Camera.h"
#include "Settings.h"
#include "Materials.h"
#include "TextureGenerator.h"

class Land
{
public:
	Transform transform;

	int octaves = 4;
	float scale = 20.0f;
	float persistence = 0.8f;
	float lacunarity = 1.1f;
	glm::vec2 offset;
	float height = 30.0f;

	int quality = 128;
	int resolution = 100;

	unsigned long long seed;
	bool autoGenerate = false;

	std::shared_ptr<Shape3D> m_Plane;
	std::shared_ptr<Shape3D> m_Deep;
	std::unique_ptr<Shader> m_LandShader;
	std::shared_ptr<Shader> m_DeepShader;

	float tiling = 5.0f;

	float peakHeight = 0.9f;
	float mountainHeight = 0.7f;
	float middleHeight = 0.35f;
	float beachHeight = 0.15f;
	float deepHeight = 0.1f;

	Material material;
	std::shared_ptr<Texture> m_PeakTexture;
	std::shared_ptr<Texture> m_MountainTexture;
	std::shared_ptr<Texture> m_MiddleTexture;
	std::shared_ptr<Texture> m_BeachTexture;
	std::shared_ptr<Texture> m_DeepTexture;

	std::shared_ptr<Texture> m_Cookie;

	std::shared_ptr<Texture> m_NormalMap;
	std::shared_ptr<Texture> m_HeightMap;

	Land();

	void OnUpdate(float deltaTime);
	
	void OnRender(Renderer renderer, Camera& camera, glm::vec3 dirLight, glm::vec3 lightPos, glm::vec4 clippingPlane = glm::vec4(0));

	void OnGUI();

	void GenerateLand();

	std::shared_ptr<Shape3D> GenerateMesh(int resolution, std::shared_ptr<Texture> heightMap, float heightMultiplier);

	float map(float value, float min1, float max1, float min2, float max2);
};

