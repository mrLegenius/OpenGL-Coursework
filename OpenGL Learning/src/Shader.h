#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID = 0;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath, const int pointLightsCount = 0, const int spotLightsCount = 0);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1b(const std::string& name, bool value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformVec2f(const std::string& name, const glm::vec2& vector);
	void SetUniformVec3f(const std::string& name, const glm::vec3& vector);
	void SetUniformVec4f(const std::string& name, const glm::vec4& vector);
	void SetUniformMat2f(const std::string& name, const glm::mat2& matrix);
	void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	ShaderProgramSource ParseShader(const std::string & filePath, const int pointLightsCount, const int spotLightsCount);
	unsigned int CompileShader(unsigned int type, const std::string & source);
	unsigned int CreateShader(const std::string & vertexShader, const std::string & fragmentShader);
	
	int GetUniformLocation(const std::string& name);
};