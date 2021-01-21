#pragma once
#include <memory>

#include "DirectionalLight.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"


#include "glm/gtc/type_ptr.hpp"

class LightManager
{
public:
	void SetDirectionalLight(std::shared_ptr<DirectionalLight> light)
	{
		dirLight = light;
	}
	void AddPointLight(Transform light)
	{
		pointLights.push_back(light);
	}
	void AddSpotLight(Transform light)
	{
		spotLights.push_back(light);
	}

	void ClearAll()
	{
		dirLight = nullptr;
		pointLights.clear();
		spotLights.clear();
	}

	std::vector<Transform> pointLights;
	std::vector<Transform> spotLights;

	static LightManager& GetInstance()
	{
		static LightManager inst;
		return inst;
	}

	std::shared_ptr<Texture> GetShadowMap()
	{
		return dirLight->shadowTexture;
	}
	void SetLightSpaceMatrixTo(Shader& shader)
	{
		if (dirLight)
		{
			//shader.SetUniformMat4fv("u_LightSpaceMatrix", glm::value_ptr(dirLight->lightSpaceMatrix));
			shader.SetUniformMat4f("u_LightSpaceMatrix", dirLight->lightSpaceMatrix);
		}
	}
	void SetLightingTo(Shader& shader, Camera& camera)
	{
		shader.SetUniform1i("u_PointLightsCount", pointLights.size());
		shader.SetUniform1i("u_SpotLightsCount", spotLights.size());

		if(dirLight)
		{ 
			shader.SetUniformVec3f("u_DirLight.direction", glm::radians(dirLight->direction));
			shader.SetUniformVec3f("u_DirLight.ambient", dirLight->ambient);
			shader.SetUniformVec3f("u_DirLight.diffuse", dirLight->diffuse);
			shader.SetUniformVec3f("u_DirLight.specular", dirLight->specular);
		}

		for (size_t i = 0; i < pointLights.size(); i++)
		{
			std::string light = "u_PointLights[" + std::to_string(i) + "]";

			shader.SetUniformVec3f(light + ".position", pointLights[i].position);
			shader.SetUniformVec3f(light + ".ambient", glm::vec3(0.5f, 0.5f, 0.5f));
			shader.SetUniformVec3f(light + ".diffuse", glm::vec3(0.3f, 0.3f, 0.3f));
			shader.SetUniformVec3f(light + ".specular", glm::vec3(0.3f, 0.3f, 0.3f));
			shader.SetUniform1f(light + ".constant", 1.0f);
			shader.SetUniform1f(light + ".linear", 0.09f);
			shader.SetUniform1f(light + ".quadratic", 0.032f);
		}
		
		for (size_t i = 0; i < spotLights.size(); i++)
		{
			std::string light = "u_SpotLights[" + std::to_string(i) + "]";
			shader.SetUniformVec3f(light + ".position", camera.Position);
			shader.SetUniformVec3f(light + ".direction", camera.Front);
			shader.SetUniform3f(light + ".ambient", 0.0f, 0.0f, 0.0f);
			shader.SetUniform3f(light + ".diffuse", 1.0f, 1.0f, 1.0f);
			shader.SetUniform3f(light + ".specular", 1.0f, 1.0f, 1.0f);
			shader.SetUniform1f(light + ".constant", 1.0f);
			shader.SetUniform1f(light + ".linear", 0.09);
			shader.SetUniform1f(light + ".quadratic", 0.032);
			shader.SetUniform1f(light + ".cutOff", glm::cos(glm::radians(12.5f)));
			shader.SetUniform1f(light + ".outerCutOff", glm::cos(glm::radians(15.0f)));
		}
	}
private:
	LightManager() { }
	LightManager(LightManager& other) = delete;
	void operator=(const LightManager&) = delete;

	std::shared_ptr<DirectionalLight> dirLight;
};

