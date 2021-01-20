#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

class Transform
{
public:
	glm::vec3 position;
	glm::vec3 eulerAngles;
	glm::vec3 scale;

	Transform( glm::vec3 position = glm::vec3(0), 
			   glm::vec3 eulerAngles = glm::vec3(0), 
			   glm::vec3 scale = glm::vec3(1) )
			: position(position)
			, eulerAngles(eulerAngles)
			, scale(scale)
	{ }

	glm::mat4 GetModel()
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
		model = glm::rotate(model, glm::radians(eulerAngles.x), glm::vec3(1.0f, 0, 0));
		model = glm::rotate(model, glm::radians(eulerAngles.y), glm::vec3(0, 1.0f, 0));
		model = glm::rotate(model, glm::radians(eulerAngles.z), glm::vec3(0, 0, 1.0f));
		model = glm::scale(model, scale);

		return model;
	}

	void OnGUI()
	{

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Separator();
			ImGui::DragFloat3("Position", &this->position[0]);
			ImGui::DragFloat3("Rotation", &this->eulerAngles[0]);
			ImGui::DragFloat3("Scale", &this->scale[0]);
			ImGui::Separator();
		}
	}
};

