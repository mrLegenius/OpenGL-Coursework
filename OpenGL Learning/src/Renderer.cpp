#include <iostream>

#include "Renderer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawElementTriangles(const Shader& shader, const VertexArray & va, const IndexBuffer & ib) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

 	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
void Renderer::DrawElementTriangleFan(const Shader& shader, const VertexArray& va, const IndexBuffer& ib) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLE_FAN, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
void Renderer::DrawElementTriangleStrip(const Shader& shader, const VertexArray& va, const IndexBuffer& ib) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLE_STRIP, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
void Renderer::DrawElementLines(const Shader& shader, const VertexArray& va, const IndexBuffer& ib) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_LINES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
void Renderer::DrawElementLineLoop(const Shader& shader, const VertexArray& va, const IndexBuffer& ib) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_LINE_LOOP, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawTriangles(const Shader& shader, const VertexArray& va, const GLsizei vertexCount) const
{
	shader.Bind();
	va.Bind();

	GLCall(glDrawArrays(GL_TRIANGLES, 0, vertexCount));
}
void Renderer::DrawTriangleStrip(const Shader& shader, const VertexArray& va, const GLsizei vertexCount) const
{
	shader.Bind();
	va.Bind();

	GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount));
}
void Renderer::DrawLines(const Shader& shader, const VertexArray& va, const GLsizei vertexCount) const
{
	shader.Bind();
	va.Bind();

	GLCall(glDrawArrays(GL_LINES, 0, vertexCount));
}
void Renderer::DrawLineLoop(const Shader& shader, const VertexArray& va, const GLsizei vertexCount) const
{
	shader.Bind();
	va.Bind();

	GLCall(glDrawArrays(GL_LINE_LOOP, 0, vertexCount));
}
void Renderer::DrawLineStrip(const Shader& shader, const VertexArray& va, const GLsizei vertexCount) const
{
	shader.Bind();
	va.Bind();

	GLCall(glDrawArrays(GL_LINE_STRIP, 0, vertexCount));
}

void Renderer::DrawPoints(const Shader& shader, const VertexArray& va, const GLsizei pointsCount) const
{
	shader.Bind();
	va.Bind();

	GLCall(glDrawArrays(GL_POINTS, 0, pointsCount));
}
