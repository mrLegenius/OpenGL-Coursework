#pragma once
#include <ctime>
#include "Test.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "Camera.h"
#include "Shape3D.h"

#include <memory>

#include "WaterSurface.h"
#include "Land.h"
#include "DirectionalLight.h"

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
		void RenderScene(Renderer renderer, glm::vec4 clippingPlane = glm::vec4(0), std::shared_ptr<Shader> shadowMapShader = nullptr);
		void RenderSkybox(Renderer renderer);

		void RenderDebug(Renderer renderer);

		float cameraSpeed = 100.0f;
		float lightDistance = 10.0f;
		std::shared_ptr<DirectionalLight> dirLight;
		WaterSurface water;
		Land land;

		bool cameraLock = true;
		bool polygoneModeFill = true;

		float clippingPlaneHeight;

		//SHADOW MAPPING
		std::shared_ptr<FrameBuffer> m_ShadowMapBuffer;
		std::shared_ptr<Shader> m_ShadowMapShader;
		//SHADOW MAPPING END


		std::shared_ptr<Shape3D> m_Plane;
		std::shared_ptr<Shader> m_TestTextureShader;


		std::shared_ptr<Shape3D> m_Skybox;
		std::shared_ptr<Texture> m_SkyboxTexture;
		std::shared_ptr<Shader> m_SkyboxShader;

		std::unique_ptr<Camera> m_Camera;
		
		std::shared_ptr<Shape3D> m_LightSource;
		std::unique_ptr<Shader> m_LightSourceShader;
		glm::vec3 m_LightPos;
	};
}
