#pragma once
#include <memory>

#include "glm/glm.hpp"
#include "Texture.h"

class DirectionalLight
{
public:
	glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	
	glm::vec3 ambient = glm::vec3(1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);

	glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);

	std::shared_ptr<Texture> shadowTexture;

	void OnGUI();
};

