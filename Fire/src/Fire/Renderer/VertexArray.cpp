#include "frpch.h"
#include "Fire/Renderer/VertexArray.h"

#include "Fire/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Fire {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    FR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		FR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}