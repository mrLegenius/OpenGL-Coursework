#include "PointLight.h"
#include "imgui/imgui.h"

void PointLight::OnGUI()
{
	if (ImGui::CollapsingHeader("Point Light"))
	{	
		ImGui::Separator();
		transform.OnGUI();

		ImGui::ColorEdit3("Ambient", &this->ambient[0]);
		ImGui::ColorEdit3("Diffuse", &this->diffuse[0]);
		ImGui::ColorEdit3("Specular", &this->specular[0]);

		ImGui::DragFloat("Constant", &this->constant);
		ImGui::DragFloat("Linear", &this->linear);
		ImGui::DragFloat("Quadratic", &this->quadratic);
		ImGui::Separator();
	}
}
