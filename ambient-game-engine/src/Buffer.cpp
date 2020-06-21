#include "Renderer/Buffer.h"

#include "Log.h"
#include "Renderer/Renderer.h"

namespace Ambient
{

    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
        default:
            AM_CORE_ERROR("Renderer API not implemented yet.");
        }

        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL:
            return new OpenGLIndexBuffer(indices, count);
        default:
            AM_CORE_ERROR("Renderer API not implemented yet.");
        }

        return nullptr;
    }

} // namespace Ambient
