#pragma once
#include <memory>

#include "Shape3D.h"
#include "Texture.h"
#include "Shader.h"

#include "Camera.h"

class Skybox
{
	std::shared_ptr<Shape3D> m_Skybox;
	std::shared_ptr<Texture> m_SkyboxTexture;
	std::shared_ptr<Shader> m_SkyboxShader;
public:
	Skybox();
	void OnRender(Camera& camera);
};

