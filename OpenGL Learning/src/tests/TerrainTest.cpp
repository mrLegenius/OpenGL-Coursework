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

		heightMapGenerator = std::make_shared<HeightMapGenerator>(256);

		int size = radius % 2 ? radius : radius + 1;

		int halfSize = size / 2;
		for (int i = -halfSize; i <= halfSize; i++)
		{
			std::map<int, std::shared_ptr<Shape3D>> row;
			for (int j = -halfSize; j <= halfSize; j++)
			{
				auto mesh = GenerateMesh(heightMapGenerator->GetHeightMap(i, j));
				row.emplace(std::pair<int, std::shared_ptr<Shape3D>>(j, mesh));
				
			}
			chunkMap.emplace(std::pair<int, std::map<int, std::shared_ptr<Shape3D>>>(i, row));
		}
	}

	test::TerrainTest::~TerrainTest()
	{
		
	}

	void test::TerrainTest::OnUpdate(float deltaTime)
	{
		glm::vec2 currentChunk = glm::vec2(m_Camera->Position.x / 256, m_Camera->Position.z / 256);

		//std::cout << "Current chunk = " << currentChunk.x << ", " << currentChunk.y << std::endl;
		int size = radius % 2 ? radius : radius + 1;
		int halfSize = size / 2;
		for (int i = (int)currentChunk.y - halfSize; i < (int)currentChunk.y + halfSize; i++)
		{
			std::cout << "Chunks already generated " << (chunkMap.find(i) != chunkMap.end()) << std::endl;
			if (chunkMap.find(i) == chunkMap.end())
			{
				std::cout << "Create chunk " << (chunkMap.find(i) != chunkMap.end()) << std::endl;
				AddRow(i);
			}
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
		
		for (auto& rows : chunkMap)
		{
			auto& row = rows.second;
			for (auto& chunk : row)
			{
				auto& object = *chunk.second;
				auto& shader = *m_Shader;

				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(rows.first*(resolution - 1), 0, chunk.first*(resolution - 1)));
				model = glm::scale(model, glm::vec3(1.0f, 20.0f, 1.0f));
				shader.Bind();
				shader.SetUniformMat4f("u_Model", model);
				shader.SetUniformMat4f("u_View", view);
				shader.SetUniformMat4f("u_Projection", proj);

				shader.SetUniformVec3f("u_ViewPos", camera.Position);
				renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());
			}
		}
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

	std::shared_ptr<Shape3D> TerrainTest::GenerateMesh(noise::utils::NoiseMap& heightMap)
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		GLuint width = heightMap.GetWidth();
		GLuint height = heightMap.GetHeight();

		GLfloat halfX = (width - 1) / 2.0f;
		GLfloat halfZ = (height - 1) / 2.0f;

		int vertexIndex = 0;

		for (GLuint x = 0; x < width; x++)
		{
			for (GLuint z = 0; z < height; z++, vertexIndex++)
			{
				vertices.push_back(x - halfX); // X

				GLfloat resultHeight = heightMap.GetValue(x, z);
				//std::cout << "Height is " << resultHeight << std::endl;
				vertices.push_back(resultHeight); // Y
				vertices.push_back(z - halfZ); // Z

				//Default Normals
				vertices.push_back(0);
				vertices.push_back(1);
				vertices.push_back(0);

				//texture coords
				vertices.push_back(x / (float)width);
				vertices.push_back(z / (float)height);

				//indices
				if (x < height - 1 && z < height - 1)
				{
					indices.push_back(vertexIndex);
					indices.push_back(vertexIndex + height + 1);
					indices.push_back(vertexIndex + height);

					indices.push_back(vertexIndex + height + 1);
					indices.push_back(vertexIndex);
					indices.push_back(vertexIndex + 1);
				}
			}
		}

		/* Calculate normals */
		for (int i = 0; i < indices.size(); i += 3)
		{
			auto index1 = indices[i]*8;
			glm::vec3 p1(vertices[index1], vertices[index1 + 1], vertices[index1 + 2]);

			auto index2 = indices[i + 1]*8;
			glm::vec3 p2(vertices[index2], vertices[index2 + 1], vertices[index2 + 2]);

			auto index3 = indices[i + 2]*8;
			glm::vec3 p3(vertices[index3], vertices[index3 + 1], vertices[index3 + 2]);

			glm::vec3 vec1 = p2 - p1;
			glm::vec3 vec2 = p3 - p1;

			glm::vec3 norm = glm::normalize(glm::cross(vec1, vec2));

			vertices[index1 + 3] = vertices[index2 + 3] = vertices[index3 + 3] = norm.x;
			vertices[index1 + 4] = vertices[index2 + 4] = vertices[index3 + 4] = norm.y;
			vertices[index1 + 5] = vertices[index2 + 5] = vertices[index3 + 5] = norm.z;
		}

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);

		return std::make_shared<Shape3D>(vertices, indices, layout);
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

		if (ImGui::Button("Add Row 2"))
		{
			AddRow(2);
		}

		if (ImGui::Button("Add Column 2"))
		{
			AddColumn(2);
		}

		ImGui::PopID();
		ImGui::PopID();
	}
	void TerrainTest::AddRow(int index)
	{
		std::map<int, std::shared_ptr<Shape3D>> row;
		int size = radius % 2 ? radius : radius + 1;
		int halfSize = size / 2;
		
		for (int j = -halfSize; j <= halfSize; j++)
		{
			auto mesh = GenerateMesh(heightMapGenerator->GetHeightMap(index, j));
			row.emplace(std::pair<int, std::shared_ptr<Shape3D>>(j, mesh));
		}

		chunkMap.emplace(std::pair<int, std::map<int, std::shared_ptr<Shape3D>>>(index, row));
	}
	void TerrainTest::AddColumn(int index)
	{
		int size = radius % 2 ? radius : radius + 1;
		int halfSize = size / 2;

		for (int j = -halfSize; j <= halfSize; j++)
		{
			auto mesh = GenerateMesh(heightMapGenerator->GetHeightMap(j, index));
			chunkMap[j].emplace(std::pair<int, std::shared_ptr<Shape3D>>(index, mesh));
		}
	}
}
