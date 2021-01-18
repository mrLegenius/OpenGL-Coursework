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

#include "Settings.h"
#include "Camera.h"
#include "Shape3D.h"

#include <memory>

#include "WaterSurface.h"

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
		void RenderTestTextures(Renderer renderer);
		void RenderScene(Renderer renderer, glm::vec4 clippingPlane = glm::vec4(0));
		void RenderSkybox(Renderer renderer);
		void GenerateLand();

		WaterSurface water;


		glm::vec3 landScale = glm::vec3(0.1);
		glm::vec3 landPos;
		bool autoGenerate = false;
		float lacunarity = 1;
		int octaves = 4;
		float scale = 20.0f;
		float persistence = 0.8f;
		glm::vec2 offset;
		float height = 30.0f;
		unsigned long long seed;
		float quality = 128;
		glm::vec3 dirLight;

		float heightForTextures = 0.5f;

		bool cameraLock = true;
		bool polygoneModeFill = true;

		std::shared_ptr<Shape3D> m_Skybox;
		std::shared_ptr<Texture> m_SkyboxTexture;
		std::shared_ptr<Shader> m_SkyboxShader;

		std::unique_ptr<Camera> m_Camera;
		std::shared_ptr<Shape3D> m_Object;
		std::shared_ptr<Shape3D> m_LightSource;
		
		std::shared_ptr<Texture> m_PeakTexture;
		std::shared_ptr<Texture> m_MountainTexture;
		std::shared_ptr<Texture> m_MiddleTexture;
		std::shared_ptr<Texture> m_BeachTexture;
		std::shared_ptr<Texture> m_DeepTexture;

		glm::vec3 m_LightPos;

		std::unique_ptr<Shader> m_ObjectShader;
		std::unique_ptr<Shader> m_LightSourceShader;
		std::shared_ptr<Texture> m_DiffuseTexture;
		std::shared_ptr<Texture> m_SpecularTexture;
		std::shared_ptr<Texture> m_EmissionTexture;
		std::shared_ptr<Texture> m_Cookie;
		std::shared_ptr<Texture> m_CookiedHeightMapTexture;

		std::shared_ptr<Shape3D> m_TestObject;
		std::unique_ptr<Shader> m_TestShader;
		std::shared_ptr<Texture> m_TestTexture;
		std::shared_ptr<Texture> m_ColoredHeightTexture;

		std::shared_ptr<Texture> m_TestNormalTexture;

		glm::mat4 m_Proj, m_View;
	};
}
