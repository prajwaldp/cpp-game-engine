#include "Renderer/OpenGLRenderer.h"
#include "Renderer/Renderer.h"

namespace Ambient
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
} // namespace Ambient
