#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID = 0;
	std::string m_FilePath = "";
	unsigned char* m_LocalBuffer = nullptr;
	int m_Width = 0, m_Height = 0, m_BPP = 0;
public:
	Texture();
	Texture(const std::string& path);
	Texture(GLfloat data[], int width, int height);
	Texture(int width, int height);
	Texture(int width, int height, GLenum format);
	Texture(std::string paths[6]);
	
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Bind(unsigned int slot, GLenum target) const;
	void Unbind() const;

	inline int GetID() const { return m_RendererID; }
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	void GetPixels(GLfloat* data);
};