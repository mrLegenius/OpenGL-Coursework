#include "WaterSurface.h"
#include "Settings.h"
#include "LightManager.h"
#include "imgui/imgui.h"
WaterSurface::WaterSurface()
{
	m_WaterShader = std::make_shared<Shader>("res/shaders/Lit_Water.shader");

    m_Distortion = std::make_shared<Texture>();
    m_NormalMap = std::make_shared<Texture>();

	m_WaterShader->Bind();
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

void WaterSurface::SetTransparency(float value)
{
	transparency = value;
}

void WaterSurface::SetWaveSpeed(float value)
{
	moveSpeed = value;
}

void WaterSurface::SetWaveStrength(float value)
{
	waveStrength = value;
}

void WaterSurface::SetTiling(unsigned int value)
{
	tiling = value;
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

void WaterSurface::OnRender(Renderer renderer, Camera& camera, std::shared_ptr<Shader> shadowShader)
{
	auto& settings = Settings::GetInstance();

	auto proj = camera.GetProjection();
	auto view = camera.GetViewMatrix();
	auto model = transform.GetModel();

	m_NormalMap->Bind(0);
	m_ReflectionTexture->Bind(1);
	m_RefractionTexture->Bind(2);
	m_Distortion->Bind(3);
	m_RefractionDepthTexture->Bind(4);

	auto& object = *m_Plane;
	auto& shader = shadowShader ? *shadowShader : *m_WaterShader;
	
	shader.Bind();

	shader.SetUniformMat4f("u_Model", model);
	LightManager::GetInstance().SetLightSpaceMatrixTo(shader);
	
	if (shadowShader == nullptr)
	{
		shader.Bind();
		LightManager::GetInstance().SetLightingTo(shader, camera);
		material.SetTo(shader);
		
		shader.SetUniform1i("useDuDvMap", (int)useDuDvMap);
		shader.SetUniform1i("u_UseNormalMap", (int)useNormalMap);
		shader.SetUniform1i("u_UseReflectionAndRefraction", (int)useReflectionAndRefraction);

		shader.SetUniform1f("u_Tiling", tiling);
		shader.SetUniform1f("u_Transparency", transparency);
		shader.SetUniform1f("u_Reflectivity", reflectivity);
		shader.SetUniform1f("u_WaveStrength", waveStrength);
		shader.SetUniform1i("u_UseDepthTesting", useDepth);

		shader.SetUniform1f("u_MoveFactor", moveFactor);

		shader.SetUniformMat4f("u_View", view);
		shader.SetUniformMat4f("u_Projection", proj);

		shader.SetUniformVec3f("u_ViewPos", camera.Position);
	}
	
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
