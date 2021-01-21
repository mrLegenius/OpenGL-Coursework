#pragma once
class FrameBuffer
{
private:
	unsigned int m_RendererID;
public:
	FrameBuffer();
	~FrameBuffer();

	void BlockDrawingColors();

	void AttachTexture(unsigned int texture);
	void AttachDepthTexture(unsigned int texture);
	void AttachRenderBuffer(unsigned int rbo);

	bool IsComplete();
	void Bind() const;
	void Unbind() const;
};

