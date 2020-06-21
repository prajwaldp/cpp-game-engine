#pragma once

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
    virtual void UnBind() const = 0; // For debugging

    static Shader* Create(const std::string& filepath);
    static Shader* Create(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
};

class OpenGLShader : public Shader
{
  public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& vertex_shader_src, const std::string& fragment_shader_src);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void UnBind() const override;

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
};
} // namespace Ambient
