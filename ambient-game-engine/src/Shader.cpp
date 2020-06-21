#include "Renderer/Shader.h"
#include "Log.h"
#include "Renderer/Renderer.h"

namespace Ambient
{
Shader* Shader::Create(const std::string& filepath)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::OpenGL:
        return new OpenGLShader(filepath);
    default:
        AM_CORE_ERROR("Renderer API not implemented yet.");
    }

    return nullptr;
}

Shader* Shader::Create(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::OpenGL:
        return new OpenGLShader(vertexShaderSrc, fragmentShaderSrc);
    default:
        AM_CORE_ERROR("Renderer API not implemented yet.");
    }

    return nullptr;
}
} // namespace Ambient
