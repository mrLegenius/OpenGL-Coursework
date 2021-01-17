#include "RenderBuffer.h"

#include "Renderer.h"

RenderBuffer::RenderBuffer(unsigned int width, unsigned int height)
{
	GLCall(glGenRenderbuffers(1, &m_RendererID));
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	Unbind();
}
RenderBuffer::~RenderBuffer()
{
	GLCall(glDeleteRenderbuffers(1, &m_RendererID));
}

void RenderBuffer::Bind() const
{
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
}
void RenderBuffer::Unbind() const
{
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}