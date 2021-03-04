#include "LightManager.h"

void LightManager::SetDirectionalLight(std::shared_ptr<DirectionalLight> light)
{
	dirLight = light;
}

void LightManager::AddPointLight(std::shared_ptr<PointLight> light)
{
	pointLights.push_back(light);
}

void LightManager::AddSpotLight(Transform light)
{
	spotLights.push_back(light);
}

void LightManager::ClearAll()
{
	dirLight = nullptr;
	pointLights.clear();
	spotLights.clear();
}

std::shared_ptr<Texture> LightManager::GetShadowMap()
{
	return dirLight->shadowTexture;
}
void LightManager::ApplyLightSpaceMatrixTo(Shader& shader)
{
	if (dirLight)
	{
		//shader.SetUniformMat4fv("u_LightSpaceMatrix", glm::value_ptr(dirLight->lightSpaceMatrix));
		shader.SetUniformMat4f("u_LightSpaceMatrix", dirLight->lightSpaceMatrix);
	}
}

void LightManager::ApplyLightingTo(Shader& shader, Camera& camera)
{
	shader.Bind();
	shader.SetUniform1i("u_PointLightsCount", pointLights.size());
	shader.SetUniform1i("u_SpotLightsCount", spotLights.size());

	if (dirLight)
	{
		shader.SetUniformVec3f("u_DirLight.direction", dirLight->direction);
		shader.SetUniformVec3f("u_DirLight.ambient", dirLight->ambient);
		shader.SetUniformVec3f("u_DirLight.diffuse", dirLight->diffuse);
		shader.SetUniformVec3f("u_DirLight.specular", dirLight->specular);
	}

	for (size_t i = 0; i < pointLights.size(); i++)
	{
		std::string light = "u_PointLights[" + std::to_string(i) + "]";
		auto& pointLight = *pointLights[i];
		shader.SetUniformVec3f(light + ".position", pointLight.transform.position);

		shader.SetUniformVec3f(light + ".ambient", pointLight.ambient);
		shader.SetUniformVec3f(light + ".diffuse", pointLight.diffuse);
		shader.SetUniformVec3f(light + ".specular", pointLight.specular);

		shader.SetUniform1f(light + ".constant", pointLight.constant);
		shader.SetUniform1f(light + ".linear", pointLight.linear);
		shader.SetUniform1f(light + ".quadratic", pointLight.quadratic);
	}

	for (size_t i = 0; i < spotLights.size(); i++)
	{
		std::string light = "u_SpotLights[" + std::to_string(i) + "]";

		shader.SetUniformVec3f(light + ".position", spotLights[i].position);
		shader.SetUniformVec3f(light + ".direction", spotLights[i].Front());

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
