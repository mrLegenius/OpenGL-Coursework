#include "DirectionalLight.h"
#include "LightManager.h"
#include "imgui/imgui.h"

void DirectionalLight::OnGUI()
{
	if (ImGui::CollapsingHeader("Directional Light"))
	{
		ImGui::Separator();
		ImGui::DragFloat3("Direction", &this->direction[0], 0.01f, -1.0f, 1.0f);

		ImGui::ColorEdit3("Ambient", &this->ambient[0]);
		ImGui::ColorEdit3("Diffuse", &this->diffuse[0]);
		ImGui::ColorEdit3("Specular", &this->specular[0]);
		ImGui::Separator();
	}
}