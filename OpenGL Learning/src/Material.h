#pragma once
#include "glm/glm.hpp"
#include "Shader.h"
struct Material
{
public:
	glm::vec3 ambient = glm::vec3(0);
	glm::vec3 diffuse = glm::vec3(0);
	glm::vec3 specular = glm::vec3(0);

	float shininess = 0;

	Material();
	Material(glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		float shininess);

	void SetTo(Shader& shader);
	void OnGUI();
};