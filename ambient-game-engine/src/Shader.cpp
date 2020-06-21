#include "Renderer/Shader.h"
#include "Log.h"
#include "Renderer/Renderer.h"

namespace Ambient
{
    Ref<Shader> Shader::Create(const std::string& filepath)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLShader>(filepath);
        default:
            AM_CORE_ERROR("Renderer API not implemented yet.");
        }

        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexShaderSrc,
            const std::string& fragmentShaderSrc)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLShader>(name, vertexShaderSrc, fragmentShaderSrc);
        default:
            AM_CORE_ERROR("Renderer API not implemented yet.");
        }

        return nullptr;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader, const std::string& name)
    {
        if (m_Shaders.find(name) != m_Shaders.end())
        {
            AM_CORE_ERROR("Shader {} already exists", name);
            return;
        }

        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        const std::string& name = shader->GetName();
        Add(shader, name);
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath, const std::string& name)
    {
        auto shader = Shader::Create(filepath);
        Add(shader, name);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        if (m_Shaders.find(name) == m_Shaders.end())
        {
            AM_CORE_ERROR("Shader {} not found", name);
            return nullptr;
        }

        return m_Shaders[name];
    }
} // namespace Ambient
