#include "Log.h"
#include "Renderer/Texture.h"

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Ambient
{
OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath) : m_FilePath(filepath)
{
    int width, height, channels;

    stbi_set_flip_vertically_on_load(1);
    stbi_uc* buffer = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

    if (!buffer)
    {
        AM_CORE_ERROR("Failed to load texture image");
    }

    m_Width = width;
    m_Height = height;

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    GLenum internalFormat = 0, format = 0;

    if (channels == 4)
    {
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
    }
    else if (channels == 3)
    {
        internalFormat = GL_RGB8;
        format = GL_RGB;
    }

    if (internalFormat == 0 || format == 0)
    {
        AM_CORE_ERROR("Image structure cannot be determined. Channels = {}", channels);
    }

    // Allocating space on the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    // No need to retain the texture in the CPU memory
    stbi_image_free(buffer);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
} // namespace Ambient
