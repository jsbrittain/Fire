#include "frpch.h"
#include "Fire/Renderer/GraphicsContext.h"

#include "Fire/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Fire {

    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    FR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        FR_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} 