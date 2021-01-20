#pragma once
#include "glm/glm.hpp"

class Transform
{
public:
	glm::vec3 position;
	glm::vec3 eulerAngles;
	glm::vec3 scale;

	Transform(glm::vec3 position = glm::vec3(0), glm::vec3 eulerAngles = glm::vec3(0), glm::vec3 scale = glm::vec3(1));

	glm::mat4 GetModel();

	void OnGUI();
};

