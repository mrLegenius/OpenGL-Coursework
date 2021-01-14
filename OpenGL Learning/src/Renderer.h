#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
private:
public:
	void Clear() const;
	void DrawElementTriangles(const Shader& shader, const VertexArray& va, const IndexBuffer& ib) const;
	void DrawElementLines(const Shader& shader, const VertexArray& va, const IndexBuffer& ib) const;
	void DrawElementLineLoop(const Shader& shader, const VertexArray& va, const IndexBuffer& ib) const;

	void DrawElementTriangleStrip(const Shader& shader, const VertexArray& va, const IndexBuffer& ib) const;
	void DrawElementTriangleFan(const Shader& shader, const VertexArray& va, const IndexBuffer& ib) const;
	void DrawTriangles(const Shader& shader, const VertexArray& va, const GLsizei vertexCount) const;
	void DrawTriangleStrip(const Shader& shader, const VertexArray& va, const GLsizei vertexCount) const;
	void DrawLines(const Shader& shader, const VertexArray& va, const GLsizei vertexCount) const;
	void DrawLineStrip(const Shader& shader, const VertexArray& va, const GLsizei vertexCount) const;
	void DrawLineLoop(const Shader& shader, const VertexArray& va, const GLsizei vertexCount) const;
	void DrawPoints(const Shader& shader, const VertexArray& va, const GLsizei pointsCount) const;
};