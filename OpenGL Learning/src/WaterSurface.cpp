#include "WaterSurface.h"
#include "Settings.h"

WaterSurface::WaterSurface()
{
	m_WaterShader = std::make_shared<Shader>("res/shaders/Lit_Water.shader");

    m_Distortion = std::make_shared<Texture>();
    m_NormalMap = std::make_shared<Texture>();

	m_WaterShader->Bind();
	m_WaterShader->SetUniform3f("u_DirLight.ambient", 0.05f, 0.05f, 0.05f);
	m_WaterShader->SetUniform3f("u_DirLight.diffuse", 0.4f, 0.4f, 0.4f);
	m_WaterShader->SetUniform3f("u_DirLight.specular", 0.5f, 0.5f, 0.5f);

	m_WaterShader->SetUniform1i("u_PointLightsCount", 1);
	m_WaterShader->SetUniform1i("u_SpotLightsCount", 1);

	m_WaterShader->SetUniform1i("normalMap", 0);
	m_WaterShader->SetUniform1i("reflectionTexture", 1);
	m_WaterShader->SetUniform1i("refractionTexture", 2);
	m_WaterShader->SetUniform1i("dudvMap", 3);
	m_WaterShader->SetUniform1i("depthMap", 4);

	auto& settings = Settings::GetInstance();

	m_ReflectionBuffer = std::make_shared<FrameBuffer>();
	m_ReflectionTexture = std::make_shared<Texture>(settings.screenWidth, settings.screenHeight);
	m_ReflectionRenderBuffer = std::make_shared<RenderBuffer>(settings.screenWidth, settings.screenHeight);

	m_ReflectionBuffer->AttachTexture(m_ReflectionTexture->GetID());
	m_ReflectionBuffer->AttachRenderBuffer(m_ReflectionRenderBuffer->GetID());

	if (!m_ReflectionBuffer->IsComplete())
		std::cout << "Reflection Frame buffer is not completed!" << std::endl;

	m_ReflectionBuffer->Unbind();

	m_RefractionBuffer = std::make_shared<FrameBuffer>();
	m_RefractionTexture = std::make_shared<Texture>(settings.screenWidth, settings.screenHeight);
	m_RefractionDepthTexture = std::make_shared<Texture>(settings.screenWidth, settings.screenHeight, GL_DEPTH_COMPONENT);

	m_RefractionBuffer->AttachTexture(m_RefractionTexture->GetID());
	m_RefractionBuffer->AttachDepthTexture(m_RefractionDepthTexture->GetID());

	if (!m_RefractionBuffer->IsComplete())
		std::cout << "Refraction Frame buffer is not completed!" << std::endl;

	m_RefractionBuffer->Unbind();

	UpdateData();
}

WaterSurface::~WaterSurface()
{
	m_WaterShader->Unbind();
}

void WaterSurface::SetResolution(unsigned int value)
{
	resolution = value;
}

void WaterSurface::SetDuDvMap(const std::string& path)
{
	assert(path.size() < 64);
	strcpy_s(dudvMap, path.c_str());
}

void WaterSurface::SetNormalMap(const std::string& path)
{
	assert(path.size() < 64);
	strcpy_s(normalMap, path.c_str());
}

void WaterSurface::StartReflectionRender()
{
	m_ReflectionBuffer->Bind();
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void WaterSurface::FinishReflectionRender()
{
	m_ReflectionBuffer->Unbind();
}

void WaterSurface::StartRefractionRender()
{
	m_RefractionBuffer->Bind();
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void WaterSurface::FinishRefractionRender()
{
	m_RefractionBuffer->Unbind();
}

void WaterSurface::UpdateData()
{
	std::string dudvName = dudvMap;
	if (dudvName != prev_dudvMap)
	{
		m_Distortion = std::make_shared<Texture>(dudvName);
		prev_dudvMap = dudvName;
	}

	std::string normalName = normalMap;
	if (normalName != prev_normalMap)
	{
		m_NormalMap = std::make_shared<Texture>(normalName);
		prev_normalMap = normalName;
	}

	if (resolution != prev_resolution)
	{
		m_Plane = Shape3D::CreatePlane(resolution);
		prev_resolution = resolution;
	}
}

void WaterSurface::OnUpdate(float deltaTime)
{
	moveFactor += moveSpeed * deltaTime;
	moveFactor = glm::fract(moveFactor);
}

void WaterSurface::OnRender(Renderer renderer, Camera& camera, glm::vec3 dirLight, glm::vec3 lightPos)
{
	auto& settings = Settings::GetInstance();

	const float aspect = (float)settings.screenHeight / (float)settings.screenWidth;

	auto proj = glm::perspective(glm::radians(camera.Zoom), (float)settings.screenWidth / (float)settings.screenHeight, 0.1f, 100000.0f);
	auto view = camera.GetViewMatrix();
	auto model = transform.GetModel();

	m_NormalMap->Bind(0);
	m_ReflectionTexture->Bind(1);
	m_RefractionTexture->Bind(2);
	m_Distortion->Bind(3);
	m_RefractionDepthTexture->Bind(4);

	auto& object = *m_Plane;
	auto& shader = *m_WaterShader;
	
	shader.Bind();

	material.SetTo(shader);

	m_WaterShader->Bind();
	m_WaterShader->SetUniform1i("useDuDvMap", (int)useDuDvMap);
	m_WaterShader->SetUniform1i("u_UseNormalMap", (int)useNormalMap);
	m_WaterShader->SetUniform1i("u_UseReflectionAndRefraction", (int)useReflectionAndRefraction);

	m_WaterShader->SetUniform1f("u_Tiling", tiling);
	m_WaterShader->SetUniform1f("u_Transparency", transparency);
	m_WaterShader->SetUniform1f("u_Reflectivity", reflectivity);
	m_WaterShader->SetUniform1f("u_WaveStrength", waveStrength);
	m_WaterShader->SetUniform1i("u_UseDepthTesting", useDepth);

	shader.SetUniform1f("u_Time", (float)glfwGetTime());
	shader.SetUniform1f("u_MoveFactor", moveFactor);

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

	shader.SetUniformVec3f("u_ViewPos", camera.Position);

	renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());

}

void WaterSurface::OnGUI()
{
	if (ImGui::CollapsingHeader("Water Surface"))
	{
		transform.OnGUI();

		/*MAIN SETTINGS*/
		//Size and Texturing
		ImGui::DragInt("Resolution", &resolution, 1.0f);
		if (resolution < 1)
			resolution = 1;

		ImGui::DragFloat("Tiling", &tiling);

		if (tiling < 1)
			tiling = 1;

		//Attributes
		ImGui::DragFloat("Transparency", &transparency, 0.01f);
		if (transparency <= 0)
			transparency = 0.000001f;

		ImGui::DragFloat("Reflectivity", &reflectivity, 0.01f);

		if (reflectivity < 0)
			reflectivity = 0;

		//Moving
		ImGui::DragFloat("Move Speed", &moveSpeed, 0.001f);

		ImGui::DragFloat("Wave Strength", &waveStrength, 0.001f);

		if (waveStrength < 0)
			waveStrength = 0;

		material.OnGUI();
		/*DUDV MAP*/
		if (ImGui::Button(useDuDvMap ? "Do Distortion" : "Do not Distortion"))
			useDuDvMap = !useDuDvMap;

		if (useDuDvMap)
			ImGui::InputText("DuDv Map", dudvMap, 64);

		/*NORMAL MAP*/
		if (ImGui::Button(useNormalMap ? "Using Normal Map" : "Not using Normal Map"))
			useNormalMap = !useNormalMap;

		if (useNormalMap)
			ImGui::InputText("Normal Map", normalMap, 64);

		/*REFLECTION AND REFRACTION*/
		if (ImGui::Button(useReflectionAndRefraction ? "Using Reflect/Refract Renderer" : "Not using Reflect/Refract Renderer"))
			useReflectionAndRefraction = !useReflectionAndRefraction;

		/*DEPTH TESTING*/
		if (ImGui::Button(useDepth ? "Do Depth Testing" : "Do not Depth Testing"))
			useDepth = !useDepth;

		if (ImGui::Button("Update"))
		{
			UpdateData();
		}

		ImGui::Separator();
	}
}
