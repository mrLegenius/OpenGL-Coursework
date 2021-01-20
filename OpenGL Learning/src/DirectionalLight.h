#pragma once
#include "Transform.h"
#include "glm/glm.hpp"
class DirectionalLight
{
public:
	glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	
	glm::vec3 ambient = glm::vec3(1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);

	void OnGUI();
};

