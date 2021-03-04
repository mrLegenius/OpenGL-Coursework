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
	std::string prev_Cookie;
	char cookie[64];
public:
	Transform transform;

	int octaves = 4;
	float scale = 20.0f;
	float persistence = 0.5f;
	float lacunarity = 2.0f;
	float exponent = 1.0f;
	float islandModifier = 1.0f;
	float oceanReachRate = 1.0f;
	float islandHeight = 1.0f;

	glm::vec2 offset;

	int quality = 128;
	int resolution = 128;

	unsigned long long seed;
	bool autoGenerate = false;

	std::shared_ptr<Shape3D> m_Plane;
	std::shared_ptr<Shape3D> m_Deep;
	std::unique_ptr<Shader> m_LandShader;
	std::shared_ptr<Shader> m_DeepShader;

	float tiling = 5.0f;

	float peakHeight = 1.0f;
	float mountainHeight = 0.99f;
	float middleHeight = 0.7f;
	float beachHeight = 0.3f;
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
	
	void OnRender(Renderer renderer, Camera& camera, glm::vec4 clippingPlane = glm::vec4(0), std::shared_ptr<Shader> shadowMapShader = nullptr);
	
	void OnGUI();

	void GenerateLand();

	
	/// <summary>
	/// 1x1 Plane with y from height map
	/// </summary>
	/// <param name="resolution"> Number of vertices in row/column </param>
	/// <returns> Land mesh </returns>
	std::shared_ptr<Shape3D> GenerateMesh(int resolution, std::shared_ptr<Texture> heightMap, std::shared_ptr<Texture> cookieMap = nullptr);
	/// <summary>
	/// Width x Height size of height Map
	/// </summary>
	/// <returns> Land mesh </returns>
	std::shared_ptr<Shape3D> GenerateMesh(std::shared_ptr<Texture> heightMap, std::shared_ptr<Texture> cookieMap = nullptr);

	std::shared_ptr<Shape3D> GenerateMesh(int width, int height);

	void SetCookie(const std::string& path)
	{
		assert(path.size() < 64);
		strcpy_s(cookie, path.c_str());
	}


	inline float map(float value, float min1, float max1, float min2, float max2);
};

