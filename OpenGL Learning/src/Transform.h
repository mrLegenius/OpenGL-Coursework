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

	glm::vec3 Front()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(eulerAngles.y)) * cos(glm::radians(eulerAngles.x));
		front.y = sin(glm::radians(eulerAngles.x));
		front.z = sin(glm::radians(eulerAngles.y)) * cos(glm::radians(eulerAngles.x));
		return glm::normalize(front);
	}

	void OnGUI();
};

