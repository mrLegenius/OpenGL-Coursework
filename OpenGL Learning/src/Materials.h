#pragma once
#include "Shader.h"

static class Materials
{
public:
	inline static void SetEmerald(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.0215f, 0.1745f, 0.0215f);
		shader.SetUniform3f("u_Material.diffuse", 0.07568f, 0.89f, 0.63f);
		shader.SetUniform3f("u_Material.specular", 0.633f, 0.727811f, 0.633f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.6f);
	}
	inline static void SetJade(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.135f, 0.2225f, 0.1575f);
		shader.SetUniform3f("u_Material.diffuse", 0.54f, 0.89f, 0.63f);
		shader.SetUniform3f("u_Material.specular", 0.316228f, 0.316228f, 0.316228f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.1f);
	}
	inline static void SetObsidian(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.05375f, 0.05f, 0.06625f);
		shader.SetUniform3f("u_Material.diffuse", 0.18275f, 0.17f, 0.22525f);
		shader.SetUniform3f("u_Material.specular", 0.332741f, 0.328634f, 0.346435f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.3f);
	}
	inline static void SetPearl(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.25f, 0.20725f, 0.20725f);
		shader.SetUniform3f("u_Material.diffuse", 1.0f, 0.829f, 0.829f);
		shader.SetUniform3f("u_Material.specular", 0.296648f, 0.296648f, 0.296648f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.088f);
	}
	inline static void SetRuby(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.1745f, 0.01175f, 0.01175f);
		shader.SetUniform3f("u_Material.diffuse", 0.61424f, 0.04136f, 0.04136f);
		shader.SetUniform3f("u_Material.specular", 0.727811f, 0.626959f, 0.626959f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.6f);
	}
	inline static void SetTurquoise(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.1f, 0.18725f, 0.1745f);
		shader.SetUniform3f("u_Material.diffuse", 0.396f, 0.74151f, 0.69102f);
		shader.SetUniform3f("u_Material.specular", 0.297254f, 0.30829f, 0.306678f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.1f);
	}
	inline static void SetBrass(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.329412f, 0.223529f, 0.027451f);
		shader.SetUniform3f("u_Material.diffuse", 0.780392f, 0.568627f, 0.113725f);
		shader.SetUniform3f("u_Material.specular", 0.992157f, 0.941176f, 0.807843f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.21794872f);
	}
	inline static void SetBronze(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.2125f, 0.1275f, 0.054f);
		shader.SetUniform3f("u_Material.diffuse", 0.714f, 0.4284f, 0.18144f);
		shader.SetUniform3f("u_Material.specular", 0.393548f, 0.271906f, 0.166721f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.2f);
	}
	inline static void SetChrome(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.25f, 0.25f, 0.25f);
		shader.SetUniform3f("u_Material.diffuse", 0.4f, 0.4f, 0.4f);
		shader.SetUniform3f("u_Material.specular", 0.774597f, 0.774597f, 0.774597f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.6f);
	}
	inline static void SetCopper(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.19125f, 0.0735f, 0.0225f);
		shader.SetUniform3f("u_Material.diffuse", 0.7038f, 0.27048f, 0.0828f);
		shader.SetUniform3f("u_Material.specular", 0.256777f, 0.137622f, 0.086014f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.1f);
	}
	inline static void SetGold(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.24725f, 0.1995f, 0.0745f);
		shader.SetUniform3f("u_Material.diffuse", 0.75164f, 0.60648f, 0.22648f);
		shader.SetUniform3f("u_Material.specular", 0.628281f, 0.555802f, 0.366065f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.4f);
	}
	inline static void SetSilver(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.19225f, 0.19225f, 0.19225f);
		shader.SetUniform3f("u_Material.diffuse", 0.50754f, 0.50754f, 0.50754f);
		shader.SetUniform3f("u_Material.specular", 0.508273f, 0.508273f, 0.508273f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.4f);
	}
	inline static void SetBlackPlastic(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.0f, 0.0f, 0.0f);
		shader.SetUniform3f("u_Material.diffuse", 0.01f, 0.01f, 0.01f);
		shader.SetUniform3f("u_Material.specular", 0.50f, 0.50f, 0.50f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.25f);
	}
	inline static void SetCyanPlastic(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.0f, 0.1f, 0.20725f);
		shader.SetUniform3f("u_Material.diffuse", 1.0f, 0.829f, 0.829f);
		shader.SetUniform3f("u_Material.specular", 0.296648f, 0.296648f, 0.296648f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.25f);
	}
	inline static void SetGreenPlastic(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.0f, 0.0f, 0.0f);
		shader.SetUniform3f("u_Material.diffuse", 0.1f, 0.35f, 0.1f);
		shader.SetUniform3f("u_Material.specular", 0.45f, 0.55f, 0.45f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.25f);
	}
	inline static void SetRedPlastic(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.0f, 0.0f, 0.0f);
		shader.SetUniform3f("u_Material.diffuse", 0.5f, 0.0f, 0.0f);
		shader.SetUniform3f("u_Material.specular", 0.7f, 0.6f, 0.6f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.25f);
	}
	inline static void SetWhitePlastic(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.0f, 0.0f, 0.0f);
		shader.SetUniform3f("u_Material.diffuse", 0.55f, 0.55f, 0.55f);
		shader.SetUniform3f("u_Material.specular", 0.70f, 0.70f, 0.70f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.25f);
	}
	inline static void SetYellowPlastic(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.0f, 0.0f, 0.0f);
		shader.SetUniform3f("u_Material.diffuse", 0.5f, 0.5f, 0.0f);
		shader.SetUniform3f("u_Material.specular", 0.60f, 0.60f, 0.50f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.25f);
	}
	inline static void SetBlackRubber(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.02f, 0.02f, 0.02f);
		shader.SetUniform3f("u_Material.diffuse", 0.01f, 0.01f, 0.01f);
		shader.SetUniform3f("u_Material.specular", 0.4f, 0.4f, 0.4f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.078125f);
	}
	inline static void SetCyanRubber(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.0f, 0.05f, 0.05f);
		shader.SetUniform3f("u_Material.diffuse", 0.4f, 0.5f, 0.5f);
		shader.SetUniform3f("u_Material.specular", 0.04f, 0.7f, 0.7f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.078125f);
	}
	inline static void SetGreenRubber(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.0f, 0.05f, 0.0f);
		shader.SetUniform3f("u_Material.diffuse", 0.4f, 0.5f, 0.4f);
		shader.SetUniform3f("u_Material.specular", 0.04f, 0.7f, 0.04f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.078125f);
	}
	inline static void SetRedRubber(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.05f, 0.0f, 0.0f);
		shader.SetUniform3f("u_Material.diffuse", 0.5f, 0.4f, 0.4f);
		shader.SetUniform3f("u_Material.specular", 0.7f, 0.04f, 0.04f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.078125f);
	}
	inline static void SetWhiteRubber(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.05f, 0.05f, 0.05f);
		shader.SetUniform3f("u_Material.diffuse", 0.5f, 0.5f, 0.5f);
		shader.SetUniform3f("u_Material.specular", 0.7f, 0.7f, 0.7f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.078125f);
	}
	inline static void SetYellowRubber(Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f("u_Material.ambient", 0.05f, 0.05f, 0.0f);
		shader.SetUniform3f("u_Material.diffuse", 0.5f, 0.5f, 0.4f);
		shader.SetUniform3f("u_Material.specular", 0.7f, 0.7f, 0.04f);
		shader.SetUniform1f("u_Material.shininess", 128.0f * 0.078125f);
	}
};