#include "Texture.h"
#include "stb_image/stb_image.h"
#include <iostream>

Texture::Texture(const std::string & path) : m_FilePath(path)
{
	stbi_set_flip_vertically_on_load(1);

	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	/*
	int i = 0;
	for(; i < m_Width * m_Height; i+=4)
	{ 
		std::cout << "r = " << (int)m_LocalBuffer[i] << " ";
		std::cout << "g = " << (int)m_LocalBuffer[i+1] << " ";
		std::cout << "b = " << (int)m_LocalBuffer[i+2] << " ";
		std::cout << "a = " << (int)m_LocalBuffer[i+3] << " ";
		std::cout << std::endl;
	}
	std::cout << "Out of color" << std::endl;
	for (; i < 100; i += 4)
	{
		
		std::cout << "r = " << (int)m_LocalBuffer[i] << " ";
		std::cout << "g = " << (int)m_LocalBuffer[i + 1] << " ";
		std::cout << "b = " << (int)m_LocalBuffer[i + 2] << " ";
		std::cout << "a = " << (int)m_LocalBuffer[i + 3] << " ";
		std::cout << std::endl;
	}*/


	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}
Texture::Texture()
{
	GLCall(glGenTextures(1, &m_RendererID));

	GLubyte data[] = { 255, 255, 255, 255 };

	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

	m_Width = 1;
	m_Height = 1;
}


Texture::Texture(GLfloat data[], int width, int height)
{
	GLCall(glGenTextures(1, &m_RendererID));

	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, data));

	m_Width = width;
	m_Height = height;
}

Texture::Texture(int width, int height)
{
	GLCall(glGenTextures(1, &m_RendererID));

	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	m_Width = width;
	m_Height = height;
}


Texture::Texture(int width, int height, GLenum format)
{
	GLCall(glGenTextures(1, &m_RendererID));

	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);

	m_Width = width;
	m_Height = height;
}


Texture::Texture(std::string paths[6])
{
	GLCall(glGenTextures(1, &m_RendererID));

	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

	int width, height, nrChannels;
	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			m_Width += width;
			m_Height += height;

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << paths[i] << std::endl;
			stbi_image_free(data);
		}
	}

	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::GetPixels(GLfloat* data)
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data));
}
void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot))
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Bind(unsigned int slot, GLenum target) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot))
	GLCall(glBindTexture(target, m_RendererID));
}


void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
