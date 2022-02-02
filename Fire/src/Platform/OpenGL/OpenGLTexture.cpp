#include "frpch.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>

namespace Fire {

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
 		: m_Width(width), m_Height(height)
 	{
 		FR_PROFILE_FUNCTION();

 		m_InternalFormat = GL_RGBA8;
 		m_DataFormat = GL_RGBA;

 		/* /// OpenGL 4.5
 		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
 		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

 		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

 		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
 		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);*/

 		glGenTextures(1, &m_RendererID);		// m_RendererID is the texture ID
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
 		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);

 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

 	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		FR_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		FR_CORE_ASSERT(data, "Failed to load image: ", path);
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		FR_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		/* /// OpenGL 4.5
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
		*/

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		FR_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
 	{
 		FR_PROFILE_FUNCTION();
 		
 		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
 		FR_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
 		/* /// OpenGL 4.5
 		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);*/
 		glBindTexture(GL_TEXTURE_2D, m_RendererID);
 		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
 	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		FR_PROFILE_FUNCTION();
		
		glActiveTexture(GL_TEXTURE0+slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}
}
