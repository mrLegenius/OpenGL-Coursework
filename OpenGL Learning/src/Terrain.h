#pragma once
#include <memory>
#include <vector>

#include "Chunk.h"
#include "HeightMapGenerator.h"
#include "Camera.h"
#include "Materials.h"
#include "LightManager.h"

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

class Terrain
{
public:
	bool IsChunkGenerated(glm::ivec2 coords)
	{
		return chunksMap.find(coords) != chunksMap.end();
	}

	Terrain()
	{
		heightMapGenerator = std::make_shared<HeightMapGenerator>(256);
		terrainShader = std::make_shared<Shader>("res/shaders/Lit_Color.shader");
	}
	
	void AddChunk(glm::ivec2 coords)
	{
		auto chunk = Chunk(coords.x, coords.y);
		chunk.GenerateMesh(heightMapGenerator->GetHeightMap(coords.x, coords.y));

		chunksMap.emplace(coords, chunk);
		chunks.push_back(chunk);
	}

	void OnRender(Camera& camera)
	{
		Renderer renderer;
		auto& shader = *terrainShader;
		auto proj = camera.GetProjection();
		auto view = camera.GetViewMatrix();

		shader.Bind();
		Materials::GetEmerald().SetTo(shader);
		LightManager::GetInstance().ApplyLightingTo(shader, camera);

		int resolution = heightMapGenerator->GetResolution();

		for (auto& chunk : chunksMap)
		{
			auto& object = chunk.second;
			auto& mesh = object.GetMesh();

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(object.GetX() * (resolution - 1), 0, object.GetZ() * (resolution - 1)));
			model = glm::scale(model, glm::vec3(1.0f, 20.0f, 1.0f));
			shader.Bind();
			shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_View", view);
			shader.SetUniformMat4f("u_Projection", proj);

			shader.SetUniformVec3f("u_ViewPos", camera.Position);
			renderer.DrawElementTriangles(shader, mesh.getObjectVAO(), mesh.getIndexBuffer());
		}
	}
private:
	std::unordered_map<glm::ivec2, Chunk> chunksMap;
	std::vector<Chunk> chunks;
	std::shared_ptr<HeightMapGenerator> heightMapGenerator;

	std::shared_ptr<Shader> terrainShader;
};

