#include "TestScene.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Renderer.h"
#include "Input.h"
#include "Materials.h"
#include "LightManager.h"
namespace test 
{
	test::TestScene::TestScene() : m_LightPos(glm::vec3(0, 1, 0))
	{
		m_Camera = std::make_unique<Camera>(glm::vec3(30.f, 100.0f, 5.0f));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		
		dirLight = std::make_shared<DirectionalLight>();
		LightManager::GetInstance().SetDirectionalLight(dirLight);
		
		auto& settings = Settings::GetInstance();

		//SKYBOX
		std::string faces[6] =
		{
			"res/textures/skybox/right.jpg",
			"res/textures/skybox/left.jpg",
			"res/textures/skybox/top.jpg",
			"res/textures/skybox/bottom.jpg",
			"res/textures/skybox/front.jpg",
			"res/textures/skybox/back.jpg"
		};

		m_Skybox = Shape3D::CreateCube();
		m_SkyboxTexture = std::make_shared<Texture>(faces);
		m_SkyboxShader = std::make_shared<Shader>("res/shaders/Unlit_Skybox.shader");

		m_SkyboxShader->Bind();
		m_SkyboxShader->SetUniform1i("u_SkyboxTexture", 0);

		#pragma region WATER_SETTINGS
			water.transform.eulerAngles.x = -90;
			water.transform.position.y = 40;
			water.transform.scale.x = 100000.0f;
			water.transform.scale.y = 100000.0f;
			
			water.material = Materials::GetMaterial(Materials::Type::CyanRubber);
			water.material.shininess = 256.0f;

			water.SetTiling(1000.0f);
			water.SetTransparency(2.750f);
			water.SetWaveSpeed(0.03f);
			water.SetWaveStrength(0.04);

			water.SetResolution(100);

			water.SetDuDvMap("res/textures/DuDv_Water_Map.png");
			water.SetNormalMap("res/textures/Normal_Water_Map.png");
		
			water.useDuDvMap = true;
			water.useNormalMap = true;
			water.useDiffuseTexture = true;

			water.UpdateData();
		#pragma endregion	
		#pragma region LAND_SETTINGS
			land.transform.scale.x = 1000;
			land.transform.scale.y = 10;
			land.transform.scale.z = 1000;

			//land.material = Materials::GetRuby();
		#pragma endregion

		//LIGHT SOURCE
		m_LightSource = Shape3D::CreateSphere(90);
		m_LightSourceShader = std::make_unique<Shader>("res/shaders/Unlit_Color.shader");
	}

	test::TestScene::~TestScene()
	{
		
	}

	void test::TestScene::OnUpdate(float deltaTime)
	{
		water.OnUpdate(deltaTime);
		land.OnUpdate(deltaTime);

		//dirLight->direction = m_Camera->Front;
		water.transform.position.x = m_Camera->Position.x;
		water.transform.position.z = m_Camera->Position.z;

		auto& input = Input::GetInstance();

		if (input.IsKeyPressed(GLFW_KEY_K))
		{
			polygoneModeFill = !polygoneModeFill;
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, polygoneModeFill ? GL_FILL : GL_LINE));
		}

		if (input.IsKeyPressed(GLFW_KEY_C))
			cameraLock = !cameraLock;
		
		if (cameraLock)
			return;

		float velocity = cameraSpeed * deltaTime;

		if (input.IsKeyDown(GLFW_KEY_A))
			m_Camera->Move(Camera_Movement::LEFT, velocity);

		if (input.IsKeyDown(GLFW_KEY_D))
			m_Camera->Move(Camera_Movement::RIGHT, velocity);

		if (input.IsKeyDown(GLFW_KEY_UP))
			m_Camera->Pitch++;

		if (input.IsKeyDown(GLFW_KEY_DOWN))
			m_Camera->Pitch--;

		if (input.IsKeyDown(GLFW_KEY_RIGHT))
			m_Camera->Yaw++;

		if (input.IsKeyDown(GLFW_KEY_LEFT))
			m_Camera->Yaw--;

		if (input.IsKeyDown(GLFW_KEY_W))
			m_Camera->Move(Camera_Movement::FORWARD, velocity);

		if (input.IsKeyDown(GLFW_KEY_S))
			m_Camera->Move(Camera_Movement::BACKWARD, velocity);

		if (input.IsKeyDown(GLFW_KEY_SPACE))
			m_Camera->Move(Camera_Movement::UP, velocity);

		if (input.IsKeyDown(GLFW_KEY_LEFT_CONTROL))
			m_Camera->Move(Camera_Movement::DOWN, velocity);

		if (input.IsKeyPressed(GLFW_KEY_EQUAL))
			m_Camera->ProcessMouseScroll(1);

		if (input.IsKeyPressed(GLFW_KEY_MINUS))
			m_Camera->ProcessMouseScroll(-1);

		m_Camera->FollowMouse(input.GetMouseDeltaX(), input.GetMouseDeltaY());
	}

	void test::TestScene::OnRender()
	{
		GLCall(glClearColor(0.06f, 0.02f, 0.13f, 1.0f));
		GLCall(glEnable(GL_CLIP_DISTANCE0));
		Renderer renderer;
			
		if (water.useReflectionAndRefraction)
		{
			float waterHeight = water.transform.position.y;
			float distance = 2 * (m_Camera->Position.y - waterHeight);

			/*REFLECTION RENDER*/
			water.StartReflectionRender();

			m_Camera->Position.y -= distance;
			m_Camera->Yaw += 180.0f;
			m_Camera->Pitch += 180.0f;
			m_Camera->FollowMouse(0, 0, false);

			RenderScene(renderer, glm::vec4(0, (distance < 0 ? -1 : 1), 0, -waterHeight));
			RenderSkybox(renderer);

			m_Camera->Position.y += distance;
			m_Camera->Yaw -= 180.0f;
			m_Camera->Pitch -= 180.0f;
			m_Camera->FollowMouse(0, 0, true);

			water.FinishReflectionRender();
			/*REFRACTION RENDER*/
			water.StartRefractionRender();
			
			RenderScene(renderer, glm::vec4(0, distance < 0 ? 1 : -1, 0, waterHeight));
			RenderSkybox(renderer);

			water.FinishRefractionRender();

			
		}
		
		/*SCENE RENDER*/
		GLCall(glDisable(GL_CLIP_DISTANCE0));
		GLCall(glClearColor(0.06f, 0.02f, 0.13f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		
		RenderScene(renderer);
		water.OnRender(renderer, *m_Camera);
		RenderSkybox(renderer);
	}

	void test::TestScene::OnImGuiRender()
	{
		ImGui::PushID(0);
		water.OnGUI();

		ImGui::PushID(1);
		land.OnGUI();

		ImGui::Separator();
		if (ImGui::Button(polygoneModeFill ? "Polygon Mode = Fill" : "Polygon Mode = Line"))
		{
			polygoneModeFill = !polygoneModeFill;

			GLCall(glPolygonMode(GL_FRONT_AND_BACK, polygoneModeFill ? GL_FILL : GL_LINE));
		}

		if (ImGui::Button(cameraLock ? "Camera is locked" : "Camera is Unlocked"))
		{
			cameraLock = !cameraLock;
		}

		ImGui::Text("L - Show/Hide Cursor");
		ImGui::Text("C - Lock/Unclock Camera");
		ImGui::Text("K - Polygon Mode");
		ImGui::Separator();

		ImGui::DragFloat("Camera Speed", &cameraSpeed, 1.0f, 0.0f, 10000.0f);
		ImGui::InputFloat3("Light Pos", &m_LightPos[0]);
		ImGui::DragFloat("ClippingHeight", &clippingPlaneHeight, 0.1f, -100000.0f, 10000.0f);
		
		ImGui::PushID(2);

		dirLight->OnGUI();
		ImGui::Separator();

		ImGui::PopID();
		ImGui::PopID();
		ImGui::PopID();
	}

	void TestScene::RenderScene(Renderer renderer, glm::vec4 clippingPlane)
	{
		auto& settings = Settings::GetInstance();

		const float aspect = (float)settings.screenHeight / (float)settings.screenWidth;

		auto proj = glm::perspective(glm::radians(m_Camera->Zoom), (float)settings.screenWidth / (float)settings.screenHeight, 0.1f, 1000.0f);
		auto view = m_Camera->GetViewMatrix();

		land.OnRender(renderer, *m_Camera, clippingPlane);

		/*
		//Light Source
		{
			auto& object = m_LightSource;
			auto& shader = *m_LightSourceShader;
			glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_LightPos);
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader.Bind();
			shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_View", view);
			shader.SetUniformMat4f("u_Projection", proj);
			shader.SetUniformVec4f("u_Color", color);
			renderer.DrawElementTriangles(shader, object->getObjectVAO(), object->getIndexBuffer());
		}*/
	}
	void TestScene::RenderSkybox(Renderer renderer)
	{
		auto& settings = Settings::GetInstance();

		auto proj = glm::perspective(glm::radians(m_Camera->Zoom), (float)settings.screenWidth / (float)settings.screenHeight, 0.1f, 1000.0f);
		auto view = m_Camera->GetViewMatrix();

		m_SkyboxTexture->Bind(0, GL_TEXTURE_CUBE_MAP);
		{
			GLCall(glDepthFunc(GL_LEQUAL));
			auto& object = *m_Skybox;
			auto& shader = *m_SkyboxShader;
			shader.Bind();
			shader.SetUniformMat4f("u_View", glm::mat4(glm::mat3(view)));
			shader.SetUniformMat4f("u_Projection", proj);
			renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());
			GLCall(glDepthFunc(GL_LESS));
		}
	}
}
