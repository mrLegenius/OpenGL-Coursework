#include "TerrainTest.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Renderer.h"
#include "Input.h"

#include "Settings.h"
#include "LightManager.h"
#include "Material.h"

#include <noise/noise.h>

using namespace noise;

namespace test 
{
	test::TerrainTest::TerrainTest()
	{
		m_Camera = std::make_unique<Camera>(glm::vec3(30.f, 10.0f, 5.0f));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		auto& settings = Settings::GetInstance();
		light = std::make_shared<DirectionalLight>();
		LightManager::GetInstance().SetDirectionalLight(light);
		pointLight = std::make_shared<PointLight>();
		pointLightMesh = Shape3D::CreateSphere(45);
		LightManager::GetInstance().AddPointLight(pointLight);

		m_Shader = std::make_shared<Shader>("res/shaders/Lit_Color.shader");
		m_Shader->Bind();

		int size = radius % 2 ? radius : radius + 1;

		int halfSize = size / 2;
		for (int x = -halfSize; x <= halfSize; x++)
		{
			for (int z = -halfSize; z <= halfSize; z++)
			{
				terrain.AddChunk(glm::ivec2(x, z));
			}
		}
	}

	test::TerrainTest::~TerrainTest()
	{
		
	}
	void test::TerrainTest::OnUpdate(float deltaTime)
	{
		auto currentChunk = glm::ivec2(glm::round(m_Camera->Position.x) / 256, glm::round(m_Camera->Position.z) / 256);

		if (!terrain.IsChunkGenerated(currentChunk))
		{
			terrain.AddChunk(currentChunk);
		}

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

	void test::TerrainTest::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		auto& settings = Settings::GetInstance();
		auto& camera = *m_Camera;
		Renderer renderer;
		
		/*SCENE RENDER*/
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		const float aspect = (float)settings.screenHeight / (float)settings.screenWidth;

		auto proj = camera.GetProjection();
		auto view = camera.GetViewMatrix();

		m_Shader->Bind();
		Materials::GetEmerald().SetTo(*m_Shader);
		LightManager::GetInstance().ApplyLightingTo(*m_Shader, *m_Camera);
		
		terrain.OnRender(*m_Camera);

		{
			auto& object = *pointLightMesh;
			auto& shader = *m_Shader;

			glm::mat4 model = glm::translate(glm::mat4(1.0f), pointLight->transform.position);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			shader.Bind();
			shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_View", view);
			shader.SetUniformMat4f("u_Projection", proj);

			shader.SetUniformVec3f("u_ViewPos", camera.Position);
			renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());
		}

		skybox.OnRender(*m_Camera);
	}
	void test::TerrainTest::OnImGuiRender()
	{
		ImGui::PushID(0);
		light->OnGUI();
		ImGui::PushID(1);
		pointLight->OnGUI();
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
		m_Camera->OnGUI();
		ImGui::DragFloat("Camera Speed", &cameraSpeed, 1.0f, 0.0f, 10000.0f);

		ImGui::Separator();

		ImGui::PopID();
		ImGui::PopID();
	}
	
}
