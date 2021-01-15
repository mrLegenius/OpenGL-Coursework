#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "Shader.h"


Shader::Shader(const std::string& filepath, const int pointLightsCount, const int spotLightsCount) : m_FilePath(filepath)
{
	auto source = ParseShader(filepath, pointLightsCount, spotLightsCount);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

	std::cout << "Created shader(" << m_RendererID << ") in " << filepath << std::endl;
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath, const int pointLightsCount, const int spotLightsCount)
{
	std::ifstream stream(filePath);
	if (!stream.is_open())
	{
		std::cout << "ERROR: Can't open " + filePath + " shader file!" << std::endl;
		ASSERT(false);
	}
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line.c_str() << "\n";
			if (line.find("#version") != std::string::npos)
			{
				ss[(int)type] << "#define POINT_LIGHTS_NUM " << std::to_string(pointLightsCount) << "\n";
				ss[(int)type] << "#define SPOT_LIGHTS_NUM " << std::to_string(spotLightsCount) << "\n";
			}
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1b(const std::string& name, bool value)
{
	GLCall(glUniform1i(GetUniformLocation(name), (int)value));
}

void Shader::SetUniform1i(const std::string & name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string & name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
	GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}
void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}
void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
void Shader::SetUniformVec2f(const std::string& name, const glm::vec2& vector)
{
	GLCall(glUniform2fv(GetUniformLocation(name), 1, &vector[0]));
}
void Shader::SetUniformVec3f(const std::string& name, const glm::vec3& vector)
{
	GLCall(glUniform3fv(GetUniformLocation(name), 1, &vector[0]));
}
void Shader::SetUniformVec4f(const std::string& name, const glm::vec4& vector)
{
	GLCall(glUniform4fv(GetUniformLocation(name), 1, &vector[0]));
}
void Shader::SetUniformMat2f(const std::string& name, const glm::mat2& matrix)
{
	GLCall(glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}
void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
	GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 & matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));

	if (location == -1)
		std::cout << "Warning: " << "In Shader(" << m_RendererID << ") uniform '" << name << "' doesn't exist!" << std::endl;
	
	m_UniformLocationCache[name] = location;
	return location;
}
