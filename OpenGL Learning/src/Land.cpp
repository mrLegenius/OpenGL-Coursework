#include "Land.h"

Land::Land()
{
	m_Deep = Shape3D::CreatePlane(100);
	m_DeepShader = std::make_unique<Shader>("res/shaders/Unlit_Texture.shader");

	m_LandShader = std::make_unique<Shader>("res/shaders/Lit_Land.shader");
	m_LandShader->Bind();

	m_PeakTexture = std::make_shared<Texture>("res/textures/land/peak.jpg");
	m_MountainTexture = std::make_shared<Texture>("res/textures/land/mountain.jpg");
	m_MiddleTexture = std::make_shared<Texture>("res/textures/land/middle.jpg");
	m_BeachTexture = std::make_shared<Texture>("res/textures/land/beach.jpg");
	m_DeepTexture = std::make_shared<Texture>("res/textures/land/deep.jpg");
	m_Cookie = std::make_shared<Texture>("res/textures/Cookies/Circle.png");

	m_LandShader->SetUniform1f("peakHeight", peakHeight);
	m_LandShader->SetUniform1f("mountainHeight", mountainHeight);
	m_LandShader->SetUniform1f("middleHeight", middleHeight);
	m_LandShader->SetUniform1f("beachHeight", beachHeight);
	m_LandShader->SetUniform1f("deepHeight", deepHeight);

	m_LandShader->SetUniform1i("peakTexture", 0);
	m_LandShader->SetUniform1i("mountainTexture", 1);
	m_LandShader->SetUniform1i("middleTexture", 2);
	m_LandShader->SetUniform1i("beachTexture", 3);
	m_LandShader->SetUniform1i("deepTexture", 4);
	m_LandShader->SetUniform1i("heightMap", 5);
	m_LandShader->SetUniform1i("normalMap", 6);

	m_LandShader->SetUniform3f("u_DirLight.ambient", 0.05f, 0.05f, 0.05f);
	m_LandShader->SetUniform3f("u_DirLight.diffuse", 0.4f, 0.4f, 0.4f);
	m_LandShader->SetUniform3f("u_DirLight.specular", 0.5f, 0.5f, 0.5f);

	m_LandShader->SetUniform1i("u_PointLightsCount", 1);
	m_LandShader->SetUniform1i("u_SpotLightsCount", 1);

	GenerateLand();
}

void Land::OnUpdate(float deltaTime)
{

}

void Land::OnRender(Renderer renderer, Camera& camera, glm::vec3 dirLight, glm::vec3 lightPos, glm::vec4 clippingPlane)
{
	auto& settings = Settings::GetInstance();

	const float aspect = (float)settings.screenHeight / (float)settings.screenWidth;

	auto proj = glm::perspective(glm::radians(camera.Zoom), (float)settings.screenWidth / (float)settings.screenHeight, 0.1f, 100000.0f);

	auto view = camera.GetViewMatrix();
	glm::mat4 model = transform.GetModel();

	m_PeakTexture->Bind(0);
	m_MountainTexture->Bind(1);
	m_MiddleTexture->Bind(2);
	m_BeachTexture->Bind(3);
	m_DeepTexture->Bind(4);
	m_HeightMap->Bind(5);
	m_NormalMap->Bind(6);
	auto& object = *m_Plane;
	auto& shader = *m_LandShader;

	shader.Bind();

	material.SetTo(shader);

	shader.SetUniform1f("u_Tiling", tiling);

	std::string light = "u_PointLights[0]";

	shader.SetUniformVec3f(light + ".position", lightPos);
	shader.SetUniformVec3f(light + ".ambient", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.SetUniformVec3f(light + ".diffuse", glm::vec3(0.3f, 0.3f, 0.3f));
	shader.SetUniformVec3f(light + ".specular", glm::vec3(0.3f, 0.3f, 0.3f));
	shader.SetUniform1f(light + ".constant", 1.0f);
	shader.SetUniform1f(light + ".linear", 0.09f);
	shader.SetUniform1f(light + ".quadratic", 0.032f);

	shader.SetUniformVec3f("u_DirLight.direction", dirLight);

	shader.SetUniformVec3f("u_SpotLights[0].position", camera.Position);
	shader.SetUniformVec3f("u_SpotLights[0].direction", camera.Front);
	shader.SetUniform3f("u_SpotLights[0].ambient", 0.0f, 0.0f, 0.0f);
	shader.SetUniform3f("u_SpotLights[0].diffuse", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("u_SpotLights[0].specular", 1.0f, 1.0f, 1.0f);
	shader.SetUniform1f("u_SpotLights[0].constant", 1.0f);
	shader.SetUniform1f("u_SpotLights[0].linear", 0.09);
	shader.SetUniform1f("u_SpotLights[0].quadratic", 0.032);
	shader.SetUniform1f("u_SpotLights[0].cutOff", glm::cos(glm::radians(12.5f)));
	shader.SetUniform1f("u_SpotLights[0].outerCutOff", glm::cos(glm::radians(15.0f)));

	shader.SetUniformMat4f("u_Model", model);
	shader.SetUniformMat4f("u_View", view);
	shader.SetUniformMat4f("u_Projection", proj);
	shader.SetUniformVec4f("u_Plane", clippingPlane);

	shader.SetUniformVec3f("u_ViewPos", camera.Position);

	renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());

	m_DeepTexture->Bind(0);
	{
		auto& object = m_Deep;
		auto& shader = *m_DeepShader;
		glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
		model = glm::translate(model, glm::vec3(0, -0.05f, 0));
		model = glm::rotate(model, glm::radians(transform.eulerAngles.x + 90), glm::vec3(1.0f, 0, 0));
		model = glm::rotate(model, glm::radians(transform.eulerAngles.y), glm::vec3(0, 1.0f, 0));
		model = glm::rotate(model, glm::radians(transform.eulerAngles.z), glm::vec3(0, 0, 1.0f));
		model = glm::scale(model, glm::vec3(10 * transform.scale.x, 10 * transform.scale.z, 1));
		shader.Bind();
		shader.SetUniformMat4f("u_Model", model);
		shader.SetUniformMat4f("u_View", view);
		shader.SetUniformMat4f("u_Projection", proj);
		shader.SetUniform1i("u_Texture", 0);
		shader.SetUniformVec4f("u_Plane", clippingPlane);
		shader.SetUniform1f("u_Tiling", tiling * 10);
		renderer.DrawElementTriangles(shader, object->getObjectVAO(), object->getIndexBuffer());
	}
}

 void Land::OnGUI()
{
	if (ImGui::CollapsingHeader("Land"))
	{
		transform.OnGUI();

		/*MAIN SETTINGS*/
		//Size and Texturing
		ImGui::DragInt("Resolution", &resolution, 1.0f);
		if (resolution < 0)
			resolution = 0;

		ImGui::DragFloat("Tiling", &tiling);

		if (tiling < 0)
			tiling = 0;

		ImGui::DragFloat("Scale", &scale, 1);

		ImGui::DragFloat("Lacunarity", &lacunarity, 0.1f);

		if (lacunarity < 1)
			lacunarity = 1;

		ImGui::DragFloat("Persistence", &persistence, 0.01f);
		if (persistence < 0)
			persistence = 0;


		ImGui::DragInt("Octaves", &octaves, 1);

		if (octaves < 0)
			octaves = 0;

		ImGui::DragInt("Quality", &quality, 1, 1);

		if (quality < 1)
			quality = 1;

		ImGui::DragFloat2("Offset", &offset[0], 1.0f);

		if (ImGui::Button("Generate Seed"))
			seed = rand() * rand();

		std::string str_seed = "Seed = " + std::to_string(seed);
		ImGui::Text(str_seed.c_str());

		material.OnGUI();

		ImGui::DragFloat("Height", &height, 1.0f, 0);

		if (height < 0)
			height = 0;

		if (ImGui::Button(autoGenerate ? "Auto Generation" : "Manual Generation"))
		{
			autoGenerate = !autoGenerate;
		}

		if (ImGui::Button("Generate") || autoGenerate)
		{
			GenerateLand();
		}

		ImGui::Separator();
	}
}

void Land::GenerateLand()
{
	m_HeightMap = TextureGenerator::GenerateHeightMap(glm::vec2(quality), scale, octaves, persistence, lacunarity, seed, offset);
	m_HeightMap = TextureGenerator::ApplyCookie(m_HeightMap, m_Cookie);
	m_NormalMap = TextureGenerator::GenerateNormalMapFromTexture(m_HeightMap);
	m_Plane = GenerateMesh(resolution, m_HeightMap, height);
}

std::shared_ptr<Shape3D> Land::GenerateMesh(int resolution, std::shared_ptr<Texture> heightMap, float heightMultiplier)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	glm::vec2 startPoint(-0.5f, 0.5f);

	GLuint width = heightMap->GetWidth();
	GLuint height = heightMap->GetHeight();

	GLfloat* data = new GLfloat[width * height * sizeof(GLfloat)];

	float cornerX = width / -2.0f - (width % 2 ? 0.5f : 0);
	float cornerZ = height / 2.0f + (height % 2 ? 0.5f : 0);

	GLuint half = resolution / 2;

	float size = 1.f / resolution;
	float halfSize = half * size;


	int vertexIndex = 0;
	heightMap->GetPixels(data);

	for (GLuint x = 0; x < resolution; x++)
	{
		for (GLuint y = 0; y < resolution; y++, vertexIndex++)
		{
			vertices.push_back(x * size - halfSize);

			//vertices.push_back(cornerX + x);

			int xTex = map(x, 0, resolution, 0, width);
			int yTex = map(y, 0, resolution, 0, height);

			GLuint index = (xTex + yTex * width) * 4;

			GLfloat resultHeight = data[index] * heightMultiplier;
			vertices.push_back(resultHeight);

			//vertices.push_back(cornerZ - y - 1);
			vertices.push_back(y * size - halfSize);
			//Default Normals
			vertices.push_back(0);
			vertices.push_back(1);
			vertices.push_back(0);

			//texture coords
			vertices.push_back(x / (float)resolution);
			vertices.push_back(y / (float)resolution);

			//indices
			if (x < resolution - 1 && y < resolution - 1)
			{
				indices.push_back(vertexIndex);
				indices.push_back(vertexIndex + resolution + 1);
				indices.push_back(vertexIndex + resolution);

				indices.push_back(vertexIndex + resolution + 1);
				indices.push_back(vertexIndex);
				indices.push_back(vertexIndex + 1);
			}
		}
	}

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);

	delete[] data;
	return std::make_shared<Shape3D>(vertices, indices, layout);
}

float Land::map(float value, float min1, float max1, float min2, float max2) {
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
