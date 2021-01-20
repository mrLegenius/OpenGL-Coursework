#pragma once

#include <string>
#include <iostream>

#include "Transform.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"

#include "Shader.h"
#include "Texture.h"
#include "Shape3D.h"

#include "Camera.h"

#include "Material.h"

class WaterSurface
{
private:
	std::string prev_dudvMap;
	std::string prev_normalMap;
	unsigned int prev_resolution = 0;

	int resolution = 100;
	float tiling = 10;

	float transparency = 0.5;
	float reflectivity = 0.33;
	
	float moveSpeed = 0.03;
	float waveStrength = 0.04;
	float moveFactor = 0;

	char dudvMap[64];
	char normalMap[64];

	Material material;

	std::shared_ptr<FrameBuffer> m_ReflectionBuffer;
	std::shared_ptr<RenderBuffer> m_ReflectionRenderBuffer;
	std::shared_ptr<Texture> m_ReflectionTexture;

	std::shared_ptr<FrameBuffer> m_RefractionBuffer;
	std::shared_ptr<Texture> m_RefractionTexture;
	std::shared_ptr<Texture> m_RefractionDepthTexture;

	std::shared_ptr<Shape3D> m_Plane;
	std::shared_ptr<Shader> m_WaterShader;
	std::shared_ptr<Texture> m_Distortion;
	std::shared_ptr<Texture> m_NormalMap;

public:
	Transform transform;

	bool useDiffuseTexture = false;
	bool useSpecularTexture = false;
	bool useNormalMap = false;
	bool useDuDvMap = false;

	bool useReflectionAndRefraction = true;
	bool useDepth = true;

	WaterSurface();
	~WaterSurface();

	void SetResolution(unsigned int value);
	void SetDuDvMap(const std::string& path);
	void SetNormalMap(const std::string& path);

	void StartReflectionRender();
	void FinishReflectionRender();
	void StartRefractionRender();
	void FinishRefractionRender();

	void UpdateData();
	void OnUpdate(float deltaTime);
	void OnRender(Renderer renderer, Camera& camera, glm::vec3 dirLight, glm::vec3 lightPos);
	void OnGUI();
};

