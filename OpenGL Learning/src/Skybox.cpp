#include "Skybox.h"

#include "Renderer.h"
#include "Settings.h"

Skybox::Skybox()
{
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
}

void Skybox::OnRender(Camera& camera)
{
	Renderer renderer;
	auto& settings = Settings::GetInstance();

	auto proj = camera.GetProjection();
	auto view = camera.GetViewMatrix();

	m_SkyboxTexture->Bind(0, GL_TEXTURE_CUBE_MAP);
	{
		GLCall(glDepthFunc(GL_LEQUAL));
		auto& object = *m_Skybox;
		auto& shader = *m_SkyboxShader;
		shader.Bind();
		shader.SetUniformMat4f("u_View", glm::mat4(glm::mat3(view)));
		shader.SetUniformMat4f("u_Projection", proj);
		renderer.DrawElementTriangles(shader, object.getObjectVAO(), object.getIndexBuffer());
		GLCall(glDepthFunc(GL_LESS));
	}
}
