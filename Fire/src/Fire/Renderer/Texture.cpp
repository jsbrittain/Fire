#include "frpch.h"
#include "Fire/Renderer/Texture.h"

#include "Fire/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Fire {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
 	{
 		switch (Renderer::GetAPI())
 		{
 			case RendererAPI::API::None:    FR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
 			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(width, height);
 		}

 		FR_CORE_ASSERT(false, "Unknown RendererAPI!");
 		return nullptr;
 	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    FR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
		}

		FR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
