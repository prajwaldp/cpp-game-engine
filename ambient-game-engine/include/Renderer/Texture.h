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

        virtual void Unbind() const = 0;

        virtual void SetData(void* data, uint32_t size) = 0;
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(const std::string& filepath);

        static Ref<Texture2D> Create(uint32_t width, uint32_t height);
    };

    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height);

        OpenGLTexture2D(const std::string& filepath);

        virtual ~OpenGLTexture2D();

        void SetData(void* data, uint32_t size) override;

        inline uint32_t GetWidth() const override
        {
            return m_Width;
        }

        inline uint32_t GetHeight() const override
        {
            return m_Height;
        }

        void Bind(uint32_t slot = 0) const override;

        void Unbind() const override;

    private:
        std::string m_FilePath;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
    };
} // namespace Ambient
