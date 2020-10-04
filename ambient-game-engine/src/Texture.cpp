#include "Renderer/Texture.h"
#include "Log.h"
#include "Renderer/Renderer.h"

namespace Ambient
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(width, height);
        default:
            AM_CORE_ERROR("Renderer API not implemented yet.");
        }

        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& filepath)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(filepath);
        default:
            AM_CORE_ERROR("Renderer API not implemented yet.");
        }

        return nullptr;
    }
} // namespace Ambient
