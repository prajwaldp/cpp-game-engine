#include "Renderer/SubTexture2D.h"

namespace Ambient
{
    SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
            : m_Texture(texture)
    {
        m_TexCoords[0] = { min.x, min.y };
        m_TexCoords[1] = { max.x, min.y };
        m_TexCoords[2] = { max.x, max.y };
        m_TexCoords[3] = { min.x, max.y };
    }

    Ref<SubTexture2D> SubTexture2D::CreateFromCoords(
            const Ref<Texture2D>& texture,
            float x, float y,
            float sprite_width, float sprite_height)
    {
        int texture_width = texture->GetWidth(), texture_height = texture->GetHeight();

        glm::vec2 min = { x * sprite_width / texture_width, y * sprite_height / texture_height };
        glm::vec2 max = { (x + 1) * sprite_width / texture_width, (y + 1) * sprite_height / texture_height };

        return std::make_shared<SubTexture2D>(texture, min, max);
    }
}