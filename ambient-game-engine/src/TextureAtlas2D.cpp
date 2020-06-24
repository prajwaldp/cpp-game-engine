#include "Renderer/TextureAtlas2D.h"
#include "Log.h"

namespace Ambient
{
    TextureAtlas2D::TextureAtlas2D(Ref<Texture2D>& texture, const glm::vec2& cell_size)
            : m_Texture(texture), m_CellSize(cell_size)
    {

    }

    Ref<SubTexture2D> TextureAtlas2D::GetSubTexture(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        uint32_t texture_width = m_Texture->GetWidth(), texture_height = m_Texture->GetHeight(); // in pixels

        glm::vec2 min = {
                x * m_CellSize.x / texture_width,
                y * m_CellSize.y / texture_height
        };

        glm::vec2 max = {
                (float)(x + width) * m_CellSize.x  / texture_width,
                (float)(y + height) * m_CellSize.y  / texture_height
        };

        return std::make_shared<SubTexture2D>(m_Texture, min, max);
    }
}
