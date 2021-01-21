#include "FrameBuffer.h"

#include "Renderer.h"

FrameBuffer::FrameBuffer()
{
	GLCall(glGenFramebuffers(1, &m_RendererID));
	Bind();
}
FrameBuffer::~FrameBuffer()
{
	GLCall(glDeleteFramebuffers(1, &m_RendererID));
}

void FrameBuffer::BlockDrawingColors()
{
	Bind();
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}
void FrameBuffer::AttachTexture(unsigned int texture)
{
	Bind();
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0));
}
void FrameBuffer::AttachDepthTexture(unsigned int texture)
{
	Bind();
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0));
}

void FrameBuffer::AttachRenderBuffer(unsigned int rbo)
{
	Bind();
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo););
}

bool FrameBuffer::IsComplete()
{
	Bind();
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FrameBuffer::Bind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}
void FrameBuffer::Unbind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}