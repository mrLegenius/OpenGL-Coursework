#pragma once
#include <memory>
#include "Transform.h"
#include "PointLight.h"

#include "glm/gtc/type_ptr.hpp"

#include "DirectionalLight.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

class LightManager
{
public:
	static LightManager& GetInstance()
	{
		static LightManager inst;
		return inst;
	}
	void SetDirectionalLight(std::shared_ptr<DirectionalLight> light);
	void AddPointLight(std::shared_ptr<PointLight> light);
	void AddSpotLight(Transform light);

	void ClearAll();

	std::shared_ptr<Texture> GetShadowMap();

	void ApplyLightSpaceMatrixTo(Shader& shader);
	void ApplyLightingTo(Shader& shader, Camera& camera);
private:
	LightManager() { }
	LightManager(LightManager& other) = delete;
	void operator=(const LightManager&) = delete;

	std::vector<std::shared_ptr<PointLight>> pointLights;
	std::vector<Transform> spotLights;
	std::shared_ptr<DirectionalLight> dirLight;
};

