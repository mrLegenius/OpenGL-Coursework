#pragma once
#include <time.h>
#include "Test.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Settings.h"
#include "Camera.h"
#include "Shape3D.h"
#include <memory>

namespace test
{
	class TestScene : public Test
	{
	public:
		TestScene();
		~TestScene();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:

		bool cameraLock = true;
		bool polygoneModeFill = true;

		std::unique_ptr<Camera> m_Camera;
		std::shared_ptr<Shape3D> m_Object;

		std::unique_ptr<Shader> m_ObjectShader;

		glm::mat4 m_Proj, m_View;
	};
}
