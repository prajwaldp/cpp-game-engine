#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Ambient
{
class Shader
{
  public:
    Shader(const std::string& vertex_shader_src, const std::string& fragment_shader_src);
    ~Shader();

    void Bind() const;
    void UnBind() const; // For debugging

    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

  private:
    uint32_t m_RendererID;
};
} // namespace Ambient
