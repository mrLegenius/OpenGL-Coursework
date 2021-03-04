#include "Land.h"
#include "LightManager.h"
#include "imgui/imgui.h"

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

	SetCookie("res/textures/Cookies/Circle.png");

	m_LandShader->SetUniform1i("peakTexture", 0);
	m_LandShader->SetUniform1i("mountainTexture", 1);
	m_LandShader->SetUniform1i("middleTexture", 2);
	m_LandShader->SetUniform1i("beachTexture", 3);
	m_LandShader->SetUniform1i("deepTexture", 4);
	m_LandShader->SetUniform1i("heightMap", 5);
	m_LandShader->SetUniform1i("normalMap", 6);
	m_LandShader->SetUniform1i("shadowMap", 7);

	PerlinNoiseData data = {};
	data.scale = scale;
	data.octaves = octaves;
	data.persistence = persistence;
	data.lacunarity = lacunarity;
	data.exp = exponent;
	data.islandModifier = islandModifier;
	data.oceanReachRate = oceanReachRate;
	data.islandHeight = islandHeight;

	m_HeightMap = TextureGenerator::GenerateHeightMap(data, glm::vec2(quality), seed, offset);
	m_NormalMap = TextureGenerator::GenerateNormalMapFromTexture(m_HeightMap);
	GenerateLand();
}

void Land::OnUpdate(float deltaTime)
{

}

void Land::OnRender(Renderer renderer, Camera& camera, glm::vec4 clippingPlane, std::shared_ptr<Shader> shadowMapShader)
{
	auto& settings = Settings::GetInstance();

	const float aspect = (float)settings.screenHeight / (float)settings.screenWidth;
	if(transform.scale.y != 0)
		clippingPlane.w /= transform.scale.y;

	auto proj = camera.GetProjection();
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
	auto& shader = shadowMapShader ? *shadowMapShader : *m_LandShader;

	shader.Bind();
	shader.SetUniformMat4f("u_Model", model);
	LightManager::GetInstance().ApplyLightSpaceMatrixTo(shader);
	if (shadowMapShader == nullptr)
	{
		LightManager::GetInstance().GetShadowMap()->Bind(7);
		LightManager::GetInstance().ApplyLightingTo(shader, camera);
		material.SetTo(shader);
		shader.SetUniform1f("u_Tiling", tiling);
	
		shader.SetUniform1f("peakHeight", peakHeight);
		shader.SetUniform1f("mountainHeight", mountainHeight);
		shader.SetUniform1f("middleHeight", middleHeight);
		shader.SetUniform1f("beachHeight", beachHeight);
		shader.SetUniform1f("deepHeight", deepHeight);

		shader.SetUniformMat4f("u_View", view);
		shader.SetUniformMat4f("u_Projection", proj);
		shader.SetUniformVec4f("u_Plane", clippingPlane);

		shader.SetUniformVec3f("u_ViewPos", camera.Position);
	}
	
	renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());
}

 void Land::OnGUI()
{
	if (ImGui::TreeNode("Land"))
	{
		transform.OnGUI();
		material.OnGUI();
		/*MAIN SETTINGS*/
		//Size and Texturing
		if (ImGui::TreeNode("Mesh and Texturing"))
		{
			ImGui::DragInt("Resolution", &resolution, 32.0f, 2.0f, 10000.0f);
			if (resolution < 2)
				resolution = 2;

			ImGui::DragFloat("Tiling", &tiling, 1.0f, 0.0f, 100000.0f);

			if (tiling < 0)
				tiling = 0;

			ImGui::DragFloat("Deep", &deepHeight, 0.001f, 0.0f, beachHeight - 0.001f);
			ImGui::DragFloat("Beach", &beachHeight, 0.001f, deepHeight + 0.001f, middleHeight - 0.001f);
			ImGui::DragFloat("Middle", &middleHeight, 0.001f, beachHeight + 0.001f, mountainHeight - 0.001f);
			ImGui::DragFloat("Mountain", &mountainHeight, 0.001f, middleHeight + 0.001f, peakHeight - 0.001f);
			ImGui::DragFloat("Peak", &peakHeight, 0.001f, mountainHeight + 0.001f, 1.0f);

			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Height/Normal Map Generation"))
		{
			ImGui::DragFloat("Scale", &scale, 1);

			ImGui::DragFloat("Exponent", &exponent, 0.1f);

			ImGui::DragFloat("Lacunarity", &lacunarity, 0.1f);
			if (lacunarity < 1)
				lacunarity = 1;

			ImGui::DragFloat("Persistence", &persistence, 0.01f, 0.0f, 10.0f);
			if (persistence < 0)
				persistence = 0;

			ImGui::DragInt("Octaves", &octaves, 1, 0, 16);
			if (octaves < 0)
				octaves = 0;

			ImGui::DragFloat("Island", &islandModifier, 0.1f);
			ImGui::DragFloat("Ocean", &oceanReachRate, 0.1f);
			ImGui::DragFloat("Island Height", &islandHeight, 0.1f);

			ImGui::DragInt("Quality", &quality, 1, 1, 10000);
			if (quality < 1)
				quality = 1;

			ImGui::DragFloat2("Offset", &offset[0], 1.0f);

			if (ImGui::Button("Generate Seed"))
				seed = (long long)rand() * rand();

			std::string str_seed = "Seed = " + std::to_string(seed);
			ImGui::Text(str_seed.c_str());

			ImGuiIO& io = ImGui::GetIO();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImTextureID my_tex_id = m_HeightMap->GetID();
			float my_tex_w = 128.0f;
			float my_tex_h = 128.0f;

			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
			ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
			ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);

			ImGui::SameLine();
			ImGui::Image(m_NormalMap->GetID(), ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
			ImGui::Separator();

			ImGui::InputText("Cookie", cookie, 64);

			if (ImGui::Button("Generate Map"))
			{
				PerlinNoiseData data;
				data.scale = scale;
				data.octaves = octaves;
				data.persistence = persistence;
				data.lacunarity = lacunarity;
				data.exp = exponent;
				data.islandModifier = islandModifier;
				data.oceanReachRate = oceanReachRate;
				data.islandHeight = islandHeight;

				m_HeightMap = TextureGenerator::GenerateHeightMap(data, glm::vec2(quality), seed, offset);
				m_NormalMap = TextureGenerator::GenerateNormalMapFromTexture(m_HeightMap);
			}
			ImGui::TreePop();
		}
		
		//if (ImGui::Button(autoGenerate ? "Auto Generation" : "Manual Generation"))
		//{
		//	autoGenerate = !autoGenerate;
		//}
		if (ImGui::Button("Generate") || autoGenerate)
		{
			GenerateLand();
		}

		ImGui::Separator();
		ImGui::TreePop();
	}
}

void Land::GenerateLand()
{
	std::string cookieName = cookie;
	if (cookieName != prev_Cookie)
	{
		m_Cookie = std::make_shared<Texture>(cookieName);
		prev_Cookie = cookieName;
	}
	//m_HeightMap = TextureGenerator::ApplyCookie(m_HeightMap, m_Cookie);
	//m_NormalMap = TextureGenerator::GenerateNormalMapFromTexture(m_HeightMap);
	m_Plane = GenerateMesh(m_HeightMap);
	//m_Plane = GenerateMesh(resolution, resolution);
}

std::shared_ptr<Shape3D> Land::GenerateMesh(int resolution, std::shared_ptr<Texture> heightMap, std::shared_ptr<Texture> cookieMap)
{
	assert(resolution >= 2 && "Resolution must be greater or equal to 2");
	
	int time = glfwGetTime();

	GLuint width = heightMap->GetWidth();
	GLuint height = heightMap->GetHeight();

	GLuint cookieWidth = cookieMap ? cookieMap->GetWidth() : 0;
	GLuint cookieHeight = cookieMap ? cookieMap->GetHeight() : 0;

	GLfloat* data = new GLfloat[width * height * sizeof(GLfloat)];
	GLfloat* cookieData = new GLfloat[cookieWidth * cookieHeight * sizeof(GLfloat)];
	GLuint squaresInRow = (resolution - 1);
	GLfloat half = squaresInRow / 2.0f;
	float size = 1.f / squaresInRow;
	float halfSize = half * size;

	int vertexIndex = 0;
	heightMap->GetPixels(data);
	std::cout << "Pixels from height map got in " << glfwGetTime() - time << " sec." << std::endl;

	if (cookieMap)
		cookieMap->GetPixels(cookieData);

	std::vector<float> vertices(resolution * resolution * 8);
	std::vector<unsigned int> indices(squaresInRow * squaresInRow * 6);

	for (GLuint x = 0; x < resolution; x++)
	{
		for (GLuint z = 0; z < resolution; z++, vertexIndex++)
		{
			vertices.push_back(x * size - halfSize); // X

			float cookieModifier = 1.0f;
			if (cookieMap)
			{
				int xCookie = map(x, 0, resolution, 0, cookieWidth);
				int yCookie = map(z, 0, resolution, 0, cookieHeight);
				cookieModifier = cookieData[(xCookie + yCookie * cookieWidth) * 4];
			}

			int xTex = map(x, 0, resolution, 0, width);
			int yTex = map(z, 0, resolution, 0, height);
			GLuint index = (xTex + yTex * width) * 4;

			GLfloat resultHeight = data[index] * cookieModifier; // Y
			vertices.push_back(resultHeight);
			vertices.push_back(z * size - halfSize); // Z

			//Default Normals
			vertices.push_back(0);
			vertices.push_back(1);
			vertices.push_back(0);

			//texture coords
			vertices.push_back(x / (float)resolution);
			vertices.push_back(z / (float)resolution);

			//indices
			if (x < squaresInRow && z < squaresInRow)
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
	delete[] cookieData;

	std::cout << "Mesh generated in " << glfwGetTime() - time << " sec." << std::endl;
	return std::make_shared<Shape3D>(vertices, indices, layout);
}

std::shared_ptr<Shape3D> Land::GenerateMesh(std::shared_ptr<Texture> heightMap, std::shared_ptr<Texture> cookieMap)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	GLuint width = heightMap->GetWidth();
	GLuint height = heightMap->GetHeight();

	GLuint cookieWidth = cookieMap ? cookieMap->GetWidth() : 0;
	GLuint cookieHeight = cookieMap ? cookieMap->GetHeight() : 0;

	GLfloat* data = new GLfloat[width * height * sizeof(GLfloat)];
	GLfloat* cookieData = new GLfloat[cookieWidth * cookieHeight * sizeof(GLfloat)];

	GLfloat halfX = (width - 1) / 2.0f;
	GLfloat halfZ = (height - 1) / 2.0f;

	int vertexIndex = 0;
	heightMap->GetPixels(data);

	if (cookieMap)
		cookieMap->GetPixels(cookieData);

	for (GLuint x = 0; x < width; x++)
	{
		for (GLuint z = 0; z < height; z++, vertexIndex++)
		{
			vertices.push_back(x - halfX); // X

			float cookieModifier = 1.0f;
			if (cookieMap)
			{
				int xCookie = map(x, 0, width, 0, cookieWidth);
				int yCookie = map(z, 0, height, 0, cookieHeight);
				cookieModifier = cookieData[(xCookie + yCookie * cookieWidth) * 4];
			}

			GLuint index = (x + z * width) * 4;

			GLfloat resultHeight = data[index] * cookieModifier; 
			vertices.push_back(resultHeight); // Y
			vertices.push_back(z - halfZ); // Z

			//Default Normals
			vertices.push_back(0);
			vertices.push_back(1);
			vertices.push_back(0);

			//texture coords
			vertices.push_back(x / (float)width);
			vertices.push_back(z / (float)height);

			//indices
			if (x < resolution - 1 && z < resolution - 1)
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
	delete[] cookieData;
	return std::make_shared<Shape3D>(vertices, indices, layout);
}

float Land::map(float value, float min1, float max1, float min2, float max2) {
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
