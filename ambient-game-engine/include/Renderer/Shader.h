#pragma once

#include <string>

namespace Ambient {
class Shader {
  public:
    Shader(const std::string& vertex_shader_src,
           const std::string& fragment_shader_src);
    ~Shader();

    void Bind() const;
    void UnBind() const; // For debugging

  private:
    uint32_t m_RendererID;
};
} // namespace Ambient
