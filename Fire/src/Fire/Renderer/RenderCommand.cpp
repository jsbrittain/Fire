#include "frpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Fire {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}