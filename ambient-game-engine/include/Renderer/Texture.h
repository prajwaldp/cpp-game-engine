#pragma once

#include "Types.h"
#include <string>

namespace Ambient
{
class Texture
{
  public:
    virtual ~Texture() = default;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
};

class Texture2D : public Texture
{
  public:
    static Ref<Texture2D> Create(const std::string& filepath);
};

class OpenGLTexture2D : public Texture2D
{
  public:
    OpenGLTexture2D(const std::string& filepath);
    virtual ~OpenGLTexture2D();

    inline virtual uint32_t GetWidth() const override
    {
        return m_Width;
    }

    inline virtual uint32_t GetHeight() const override
    {
        return m_Height;
    }

    virtual void Bind(uint32_t slot = 0) const override;

  private:
    std::string m_FilePath;
    uint32_t m_Width, m_Height;
    uint32_t m_RendererID;
};
} // namespace Ambient
