#include "Material.h"
#include "Materials.h"
#include "imgui/imgui.h"

 Material::Material() { }
 Material::Material(glm::vec3 ambient,
	 glm::vec3 diffuse,
	 glm::vec3 specular,
	 float shininess)
	 : ambient(ambient)
	 , diffuse(diffuse)
	 , specular(specular)
	 , shininess(shininess) { }

 void  Material::SetTo(Shader& shader)
 {
	 shader.SetUniformVec3f("u_Material.ambient", ambient);
	 shader.SetUniformVec3f("u_Material.diffuse", diffuse);
	 shader.SetUniformVec3f("u_Material.specular", specular);
	 shader.SetUniform1f("u_Material.shininess", shininess);
 }

 void Material::OnGUI()
 {
	 if (ImGui::CollapsingHeader("Material"))
	 {
		 ImGui::Separator();
		 ImGui::DragFloat3("Ambient", &this->ambient[0], 0.01f, 0.0f, 1.0f);
		 ImGui::DragFloat3("Diffuse", &this->diffuse[0], 0.01f, 0.0f, 1.0f);
		 ImGui::DragFloat3("Specular", &this->specular[0], 0.01f, 0.0f, 1.0f);
		 ImGui::DragFloat("Shininess", &this->shininess, 0.1f, 1.0f, 1000.0f);
		 ImGui::Separator();
		 const char* items[] = { "Emerald", "Jade", "Obisidian", "Pearl", "Ruby", "Turquoise",
	 "Brass", "Bronze", "Chrome", "Copper", "Gold", "Silver",
	 "BlackPlastic", "CyanPlastic", "GreenPlastic", "RedPlastic", "WhitePlastic", "YellowPlastic",
	 "BlackRubber", "CyanRubber", "GreenRubber", "RedRubber", "WhiteRubber", "YellowRubber" };
		 static int item_current_idx = 0;                    // Here our selection data is an index.

		 ImGui::Combo("Materials", &item_current_idx, items, IM_ARRAYSIZE(items));

		 ImGui::SameLine();
		 if (ImGui::Button("Set"))
			 *this = Materials::GetMaterial((Materials::Type)item_current_idx);

		 ImGui::Separator();
	 }
 }