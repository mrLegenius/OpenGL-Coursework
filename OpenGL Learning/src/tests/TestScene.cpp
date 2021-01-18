#include "TestScene.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Renderer.h"
#include "Input.h"
#include "Materials.h"
#include "LandscapeGenerator.h"
#include "TextureGenerator.h"


constexpr float WATER_SPEED = 0.03f;
namespace test 
{
	test::TestScene::TestScene() : m_LightPos(glm::vec3(0, 1, 0))
	{
		m_Camera = std::make_unique<Camera>(glm::vec3(0, 0.0f, 5.0f));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		
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

		//WATER
		water.transform.eulerAngles.x = 90;
		water.transform.scale.x = landScale.x * quality;
		water.transform.scale.y = landScale.z * quality;

		water.SetResolution(100);

		water.SetDiffuseTexture("res/textures/Water.jpg");
		water.SetDuDvMap("res/textures/DuDv_Water_Map.png");
		water.SetNormalMap("res/textures/Normal_Water_Map.png");
		
		water.useDuDvMap = true;
		water.useNormalMap = true;
		water.useDiffuseTexture = true;

		water.UpdateData();
		
		//MAIN OBJECT		
		m_Object = Shape3D::CreateCube();
		m_DiffuseTexture = std::make_shared<Texture>("res/textures/Dark-Green-Grass.jpg");
		m_SpecularTexture = std::make_shared<Texture>("res/textures/Dark-Green-Grass.jpg");
		m_EmissionTexture = std::make_shared<Texture>("res/textures/container2_emission.jpg");
		m_Cookie = std::make_shared<Texture>("res/textures/Cookie.png");
		m_ObjectShader = std::make_unique<Shader>("res/shaders/Lit.shader", 1);
		m_ObjectShader->Bind();

		m_ObjectShader->SetUniform1i("u_Material.useDiffuseMap", 1);
		m_ObjectShader->SetUniform1i("u_Material.useSpecularMap", 0);
		m_ObjectShader->SetUniform1i("u_Material.useNormalMap", 0);
		m_ObjectShader->SetUniform1i("u_Material.useEmissionMap", 0);

		m_ObjectShader->SetUniform1i("u_Material.diffuseMap", 0);
		m_ObjectShader->SetUniform1i("u_Material.specularMap", 1);
		m_ObjectShader->SetUniform1i("u_Material.emissionMap", 2);
		m_ObjectShader->SetUniform1i("u_Material.normalMap", 3);

		m_ObjectShader->SetUniform3f("u_DirLight.ambient", 0.05f, 0.05f, 0.05f);
		m_ObjectShader->SetUniform3f("u_DirLight.diffuse", 0.4f, 0.4f, 0.4f);
		m_ObjectShader->SetUniform3f("u_DirLight.specular", 0.5f, 0.5f, 0.5f);

		m_ObjectShader->SetUniform1i("u_PointLightsCount", 1);
		m_ObjectShader->SetUniform1i("u_SpotLightsCount", 1);

		//TESTING 
		m_TestObject = Shape3D::CreatePlane(100);
		m_TestShader = std::make_unique<Shader>("res/shaders/Unlit_Texture.shader");
		m_TestTexture = std::make_shared<Texture>();
		m_TestNormalTexture = std::make_shared<Texture>();
		m_ColoredHeightTexture = std::make_shared<Texture>();

		m_TestShader->Bind();
		m_TestShader->SetUniform1i("u_Texture", 0);

		//LIGHT SOURCE
		m_LightSource = Shape3D::CreateSphere(90);
		m_LightSourceShader = std::make_unique<Shader>("res/shaders/Unlit_Color.shader");

		GenerateLand();
	}

	test::TestScene::~TestScene()
	{
		m_ObjectShader->Unbind();
	}

	void test::TestScene::OnUpdate(float deltaTime)
	{
		water.OnUpdate(deltaTime);

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

		float velocity = 5.0f * deltaTime;

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
			/*REFLECTION RENDER*/
			water.StartReflectionRender();
			float waterHeight = water.transform.position.y;
			float distance = 2 * (m_Camera->Position.y - waterHeight);

			m_Camera->Position.y -= distance;
			m_Camera->Pitch += 180.0f;
			m_Camera->Yaw += 180.0f;
			m_Camera->FollowMouse(0, 0, false);

			RenderScene(renderer, glm::vec4(0, 1, 0, -waterHeight*10));
			RenderSkybox(renderer);

			m_Camera->Position.y += distance;
			m_Camera->Pitch -= 180.0f;
			m_Camera->Yaw -= 180.0f;
			m_Camera->FollowMouse(0, 0, true);

			/*REFRACTION RENDER*/
			water.StartRefractionRender();
			RenderScene(renderer, glm::vec4(0, -1, 0, waterHeight * 10));
			RenderSkybox(renderer);
			water.FinishRefractionRender();
		}
		
		/*SCENE RENDER*/
		GLCall(glDisable(GL_CLIP_DISTANCE0));
		GLCall(glClearColor(0.06f, 0.02f, 0.13f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		RenderScene(renderer);
		water.OnRender(renderer, *m_Camera, dirLight, m_LightPos);
		RenderSkybox(renderer);
		RenderTestTextures(renderer);
	}

	void test::TestScene::OnImGuiRender()
	{
		water.OnGUI();

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

		ImGui::InputFloat3("Light Pos", &m_LightPos[0]);
		ImGui::SliderFloat3("Dir Light", &dirLight[0], -glm::pi<float>(), glm::pi<float>());

		ImGui::Text("---------------");
		ImGui::Text("Land Generation");
		ImGui::Text("---------------");
		ImGui::InputFloat3("Land Pos", &landPos[0]);
		ImGui::InputFloat3("Land Scale", &landScale[0]);
		ImGui::InputFloat("Scale", &scale, 1, 10, 3);
		ImGui::InputFloat("Lacunarity", &lacunarity, 1, 10, 3);
		ImGui::SliderFloat("Persistence", &persistence, 0.0f, 1.0f);
		ImGui::InputInt("Octaves", &octaves, 1, 10);

		ImGui::InputFloat("Quality", &quality, 32, 256, 3);
	
		ImGui::SliderFloat2("Offset", &offset[0], -100.0f, 100.0f);

		if (ImGui::Button("Generate Seed"))
			seed = rand() * rand();

		std::string str_seed = "Seed = " + std::to_string(seed);
		ImGui::Text(str_seed.data());
			
		ImGui::InputFloat("Height", &height, 1.0f, 10.0f, 2);
		
		if (ImGui::Button(autoGenerate ? "Auto Generation" : "Manual Generation"))
		{
			autoGenerate = !autoGenerate;
		}

		if (octaves < 0)
			octaves = 0;

		if (persistence < 0)
			persistence = 0;

		if (lacunarity < 1)
			lacunarity = 1;

		if (quality < 1)
			quality = 1;

		if (ImGui::Button("Generate") || autoGenerate)
		{
			GenerateLand();
		}
	}

	void TestScene::RenderScene(Renderer renderer, glm::vec4 clippingPlane)
	{
		auto& settings = Settings::GetInstance();

		const float aspect = (float)settings.screenHeight / (float)settings.screenWidth;

		m_Proj = glm::perspective(glm::radians(m_Camera->Zoom), (float)settings.screenWidth / (float)settings.screenHeight, 0.1f, 1000.0f);

		m_View = m_Camera->GetViewMatrix();
		
		m_DiffuseTexture->Bind();
		m_SpecularTexture->Bind(1);
		m_EmissionTexture->Bind(2);
		m_TestNormalTexture->Bind(3);

		//Landscape
		{
			auto& object = *m_Object;
			auto& shader = *m_ObjectShader;

			glm::mat4 model = glm::translate(glm::mat4(1.0f), landPos);
			model = glm::scale(model, landScale);
			shader.Bind();

			Materials::SetBlackRubber(shader);

			shader.SetUniform1f("u_Tiling", 20.0f);
			//shader.SetUniform1f("iTime", glfwGetTime());

			std::string light = "u_PointLights[0]";

			shader.SetUniformVec3f(light + ".position", m_LightPos);
			shader.SetUniformVec3f(light + ".ambient", glm::vec3(0.5f, 0.5f, 0.5f));
			shader.SetUniformVec3f(light + ".diffuse", glm::vec3(0.3f, 0.3f, 0.3f));
			shader.SetUniformVec3f(light + ".specular", glm::vec3(0.3f, 0.3f, 0.3f));
			shader.SetUniform1f(light + ".constant", 1.0f);
			shader.SetUniform1f(light + ".linear", 0.09f);
			shader.SetUniform1f(light + ".quadratic", 0.032f);

			shader.SetUniformVec3f("u_DirLight.direction", dirLight);

			shader.SetUniformVec3f("u_SpotLights[0].position", m_Camera->Position);
			shader.SetUniformVec3f("u_SpotLights[0].direction", m_Camera->Front);
			shader.SetUniform3f("u_SpotLights[0].ambient", 0.0f, 0.0f, 0.0f);
			shader.SetUniform3f("u_SpotLights[0].diffuse", 1.0f, 1.0f, 1.0f);
			shader.SetUniform3f("u_SpotLights[0].specular", 1.0f, 1.0f, 1.0f);
			shader.SetUniform1f("u_SpotLights[0].constant", 1.0f);
			shader.SetUniform1f("u_SpotLights[0].linear", 0.09);
			shader.SetUniform1f("u_SpotLights[0].quadratic", 0.032);
			shader.SetUniform1f("u_SpotLights[0].cutOff", glm::cos(glm::radians(12.5f)));
			shader.SetUniform1f("u_SpotLights[0].outerCutOff", glm::cos(glm::radians(15.0f)));

			shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_View", m_View);
			shader.SetUniformMat4f("u_Projection", m_Proj);
			shader.SetUniformVec4f("u_Plane", clippingPlane);

			shader.SetUniformVec3f("u_ViewPos", m_Camera->Position);

			renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());
		}
		//Light Source
		{
			auto& object = m_LightSource;
			auto& shader = *m_LightSourceShader;
			glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_LightPos);
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glm::mat4 mvp = m_Proj * m_View * model;
			shader.Bind();
			shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_View", m_View);
			shader.SetUniformMat4f("u_Projection", m_Proj);
			shader.SetUniformVec4f("u_Color", color);
			renderer.DrawElementTriangles(shader, object->getObjectVAO(), object->getIndexBuffer());
		}

	}
	void TestScene::RenderSkybox(Renderer renderer)
	{
		m_SkyboxTexture->Bind(0, GL_TEXTURE_CUBE_MAP);
		{
			GLCall(glDepthFunc(GL_LEQUAL));
			auto& object = *m_Skybox;
			auto& shader = *m_SkyboxShader;
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			model = glm::scale(model, glm::vec3(1, 1, 1));
			shader.Bind();
			//shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_View", glm::mat4(glm::mat3(m_View)));
			shader.SetUniformMat4f("u_Projection", m_Proj);
			renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());
			GLCall(glDepthFunc(GL_LESS));
		}
	}
	void TestScene::RenderTestTextures(Renderer renderer)
	{
		m_TestTexture->Bind();
		{
			auto& object = *m_TestObject;
			auto& shader = *m_TestShader;
			glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 5, 0));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 mvp = m_Proj * m_View * model;
			shader.Bind();
			shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_View", m_View);
			shader.SetUniformMat4f("u_Projection", m_Proj);
			renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());
		}
		m_Cookie->Bind();
		{
			auto& object = *m_TestObject;
			auto& shader = *m_TestShader;
			glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1, 5, 0));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 mvp = m_Proj * m_View * model;
			shader.Bind();
			shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_View", m_View);
			shader.SetUniformMat4f("u_Projection", m_Proj);
			renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());
		}
		m_CookiedHeightMapTexture->Bind();
		{
			auto& object = *m_TestObject;
			auto& shader = *m_TestShader;
			glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2, 5, 0));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 mvp = m_Proj * m_View * model;
			shader.Bind();
			shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_View", m_View);
			shader.SetUniformMat4f("u_Projection", m_Proj);
			renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());
		}
		m_ColoredHeightTexture->Bind();
		{
			auto& object = *m_TestObject;
			auto& shader = *m_TestShader;
			glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(3, 5, 0));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 mvp = m_Proj * m_View * model;
			shader.Bind();
			shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_View", m_View);
			shader.SetUniformMat4f("u_Projection", m_Proj);
			renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());
		}
		m_TestNormalTexture->Bind();
		{
			auto& object = *m_TestObject;
			auto& shader = *m_TestShader;
			glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(4, 5, 0));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			shader.Bind();
			shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_View", m_View);
			shader.SetUniformMat4f("u_Projection", m_Proj);
			renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());
		}
	}
	void TestScene::GenerateLand()
	{
		m_TestTexture = TextureGenerator::GenerateHeightMap(glm::vec2(quality), scale, octaves, persistence, lacunarity, seed, offset);
		m_CookiedHeightMapTexture = TextureGenerator::ApplyCookie(m_TestTexture, m_Cookie);
		m_TestNormalTexture = TextureGenerator::GenerateNormalMapFromTexture(m_CookiedHeightMapTexture);
		m_ColoredHeightTexture = TextureGenerator::GenerateColoredHeightMap(m_CookiedHeightMapTexture);
		m_Object = LandscapeGenerator::Generate(m_CookiedHeightMapTexture, height);
		water.transform.position.y = 0.04f * height + landPos.y;
	}
}
