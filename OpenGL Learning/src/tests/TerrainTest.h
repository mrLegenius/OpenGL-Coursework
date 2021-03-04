#pragma once
#include <ctime>
#include <map>
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

#include "HeightMapGenerator.h"

#include "Skybox.h"
#include "DirectionalLight.h"
#include "PointLight.h"
namespace test
{
	class TerrainTest : public Test
	{
	public:
		TerrainTest();
		~TerrainTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		void AddRow(int index);
		void AddColumn(int index);

		std::shared_ptr<Shape3D> GenerateMesh(noise::utils::NoiseMap& heightMap);
		float cameraSpeed = 100.0f;
		int resolution = 256;
		int radius = 3;
		bool cameraLock = true;
		bool polygoneModeFill = true;

		std::map<int, std::map<int, std::shared_ptr<Shape3D>>> chunkMap;
		std::shared_ptr<HeightMapGenerator> heightMapGenerator;


		std::shared_ptr<Shader> m_Shader;
		Skybox skybox;
		std::shared_ptr<DirectionalLight> light;
		std::shared_ptr<PointLight> pointLight;
		std::shared_ptr<Shape3D> pointLightMesh;

		std::unique_ptr<Camera> m_Camera;
	};
}
