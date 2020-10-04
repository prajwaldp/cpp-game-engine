#include "Renderer/VertexArray.h"
#include "Log.h"
#include "Renderer/Renderer.h"

namespace Ambient
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexArray>();
        default:
            AM_CORE_ERROR("Renderer API not implemented yet.");
        }

        return nullptr;
    }
} // namespace Ambient
