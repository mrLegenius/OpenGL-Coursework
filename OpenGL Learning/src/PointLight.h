#pragma once
#include "Transform.h"
#include "glm/glm.hpp"

class PointLight
{
public:
	Transform transform;

	glm::vec3 ambient = glm::vec3(1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);

	float constant =  1.0f;
	float linear = 0.09f;
	float quadratic =  0.032f;

	void OnGUI();
};

