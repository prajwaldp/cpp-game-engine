#include "Renderer/VertexArray.h"
#include "Log.h"
#include "Renderer/Renderer.h"

namespace Ambient
{
VertexArray *VertexArray::Create()
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::OpenGL:
        return new OpenGLVertexArray();
    default:
        AM_CORE_ERROR("Renderer API not implemented yet.");
    }

    return nullptr;
}
} // namespace Ambient
