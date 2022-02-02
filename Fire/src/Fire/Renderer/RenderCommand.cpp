#include "frpch.h"
#include "Fire/Renderer/RenderCommand.h"

namespace Fire {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}