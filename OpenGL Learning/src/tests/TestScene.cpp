#include "TestScene.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Renderer.h"
#include "Input.h"

namespace test 
{
	test::TestScene::TestScene()
	{
		m_Camera = std::make_unique<Camera>(glm::vec3(0, 0.0f, 5.0f));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		m_ObjectShader = std::make_unique<Shader>("res/shaders/Unlit_Color.shader");

		m_Object = Shape3D::CreatePlane(10);
	}

	test::TestScene::~TestScene()
	{
		m_ObjectShader->Unbind();
	}

	void test::TestScene::OnUpdate(float deltaTime)
	{
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

		float velocity = 1.0f * deltaTime;

		if (input.IsKeyDown(GLFW_KEY_A))
			m_Camera->Move(Camera_Movement::LEFT, velocity);

		if (input.IsKeyDown(GLFW_KEY_D))
			m_Camera->Move(Camera_Movement::RIGHT, velocity);

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
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		auto& settings = Settings::GetInstance();

		const float aspect = (float)settings.screenHeight / (float)settings.screenWidth;

		m_Proj = glm::perspective(glm::radians(m_Camera->Zoom), (float)settings.screenWidth / (float)settings.screenHeight, 0.1f, 100.0f);

		m_View = m_Camera->GetViewMatrix();
		Renderer renderer;

		//TestObject
		{
			auto& shader = *m_ObjectShader;
			glm::vec4 color = glm::vec4(0.8f, 0.25f, 0.25f, 1.f);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			glm::mat4 mvp = m_Proj * m_View * model;
			shader.Bind();
			shader.SetUniformMat4f("u_MVP", mvp);
			shader.SetUniformVec4f("u_Color", color);
			renderer.DrawElementTriangles(shader, m_Object->getObjectVAO(), m_Object->getIndexBuffer());
		}
	}

	void test::TestScene::OnImGuiRender()
	{
		if (ImGui::Button(polygoneModeFill ? "Polygon Mode = Fill" : "Polygon Mode = Line"))
		{
			polygoneModeFill = !polygoneModeFill;

			GLCall(glPolygonMode(GL_FRONT_AND_BACK, polygoneModeFill ? GL_FILL : GL_LINE));
		}

		if (ImGui::Button(cameraLock ? "Camera is locked" : "Camera is Unlocked"))
		{
			cameraLock = !cameraLock;
		}

		ImGui::Text("---------------");
		ImGui::Text("L - Show/Hide Cursor");
		ImGui::Text("C - Lock/Unclock Camera");
		ImGui::Text("K - Polygon Mode");
		ImGui::Text("---------------");
	}
	
}
