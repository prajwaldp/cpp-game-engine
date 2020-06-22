#pragma once

#include "Types.h"

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Ambient
{
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0; // For debugging

        virtual void SetFloat3(const std::string& name, glm::vec3 vector) = 0;

        virtual void SetFloat4(const std::string& name, glm::vec4 vector) = 0;

        virtual void SetMat3(const std::string& name, glm::mat3 matrix) = 0;

        virtual void SetMat4(const std::string& name, glm::mat4 matrix) = 0;

        virtual void SetInt(const std::string& name, int value) = 0;

        virtual const std::string& GetName() const = 0;

        static Ref<Shader> Create(const std::string& filepath);

        static Ref<Shader> Create(const std::string& name, const std::string& vertexShaderSrc,
                const std::string& fragmentShaderSrc);
    };

    class ShaderLibrary
    {
    public:
        void Add(const Ref<Shader>& shader, const std::string& name);

        void Add(const Ref<Shader>& shader);

        Ref<Shader> Load(const std::string& filepath, const std::string& name);

        Ref<Shader> Load(const std::string& filepath);

        Ref<Shader> Get(const std::string& name);

    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);

        OpenGLShader(const std::string& name, const std::string& vertex_shader_src,
                const std::string& fragment_shader_src);

        virtual ~OpenGLShader();

        void Bind() const override;

        void Unbind() const override;

        void SetFloat3(const std::string& name, glm::vec3 vector) override;

        void SetFloat4(const std::string& name, glm::vec4 vector) override;

        void SetMat3(const std::string& name, glm::mat3 matrix) override;

        void SetMat4(const std::string& name, glm::mat4 matrix) override;

        void SetInt(const std::string& name, int value) override;

        inline virtual const std::string& GetName() const override
        {
            return m_Name;
        }

        void UploadUniformInt(const std::string& name, int value);

        void UploadUniformFloat(const std::string& name, float value);

        void UploadUniformFloat2(const std::string& name, const glm::vec2& vector);

        void UploadUniformFloat3(const std::string& name, const glm::vec3& vector);

        void UploadUniformFloat4(const std::string& name, const glm::vec4& vector);

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);

        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    private:
        std::string ReadFile(const std::string& filepath);

        std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);

        void Compile(const std::unordered_map<GLenum, std::string>& shaders);

        uint32_t m_RendererID;

        std::string m_Name;
    };
} // namespace Ambient
