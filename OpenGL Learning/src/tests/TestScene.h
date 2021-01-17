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
		void RenderWater(Renderer renderer);
		void RenderTestTextures(Renderer renderer);
		void RenderScene(Renderer renderer, glm::vec4 clippingPlane = glm::vec4(0));
		void RenderSkybox(Renderer renderer);
		void GenerateLand();

		float waterTransparency = 10.0f;
		float waterMoveFactor;

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

		bool cameraLock = true;
		bool polygoneModeFill = true;

		float waterHeight;

		std::shared_ptr<Shape3D> m_BufferObject;
		std::shared_ptr<FrameBuffer> m_FrameBuffer;
		std::shared_ptr<RenderBuffer> m_RenderBuffer;
		std::shared_ptr<Texture> m_BufferTexture;

		std::shared_ptr<FrameBuffer> m_ReflectionBuffer;
		std::shared_ptr<RenderBuffer> m_ReflectionRenderBuffer;
		std::shared_ptr<Texture> m_ReflectionTexture;

		std::shared_ptr<FrameBuffer> m_RefractionBuffer;
		std::shared_ptr<Texture> m_RefractionTexture;
		std::shared_ptr<Texture> m_RefractionDepthTexture;

		std::shared_ptr<Shape3D> m_Skybox;
		std::shared_ptr<Texture> m_SkyboxTexture;
		std::shared_ptr<Shader> m_SkyboxShader;

		std::shared_ptr<Shape3D> m_Water;
		std::unique_ptr<Shader> m_WaterShader;
		std::shared_ptr<Texture> m_WaterTexture;
		std::shared_ptr<Texture> m_WaterDistortion;
		std::shared_ptr<Texture> m_WaterNormalMap;


		std::unique_ptr<Camera> m_Camera;
		std::shared_ptr<Shape3D> m_Object;
		std::shared_ptr<Shape3D> m_LightSource;
		

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
