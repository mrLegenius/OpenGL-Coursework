#pragma once
#include "glm/glm.hpp"
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

	void OnGUI()
	{
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Separator();
			ImGui::DragFloat3("Position", &position[0]);
			ImGui::DragFloat3("Rotation", &eulerAngles[0]);
			ImGui::DragFloat3("Scale", &scale[0]);
			ImGui::Separator();
		}
	}
};

