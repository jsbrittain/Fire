#include "frpch.h"
#include "Fire/Renderer/Buffer.h"

#include "Fire/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Fire
{

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	FR_CORE_ASSERT(false,"RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size);
		}

		FR_CORE_ASSERT(false,"Unknown RendererAPI specified!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	FR_CORE_ASSERT(false,"RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices,size);
		}

		FR_CORE_ASSERT(false,"Unknown RendererAPI specified!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	FR_CORE_ASSERT(false,"RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices,count);
		}

		FR_CORE_ASSERT(false,"Unknown RendererAPI specified!");
		return nullptr;
	}

}