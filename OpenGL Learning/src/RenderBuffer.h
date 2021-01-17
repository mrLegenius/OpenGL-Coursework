#pragma once
class RenderBuffer
{
private:
	unsigned int m_RendererID;
public:
	RenderBuffer(unsigned int width, unsigned int height);
	~RenderBuffer();

	
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetID() { return m_RendererID; }
};

