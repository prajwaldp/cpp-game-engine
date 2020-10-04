#pragma once

#include "Renderer/Texture.h"
#include "Renderer/SubTexture2D.h"
#include <glm/glm.hpp>

namespace Ambient
{
    class TextureAtlas2D
    {
    public:
        TextureAtlas2D(Ref<Texture2D>& texture, const glm::vec2& cell_size);

        Ref<SubTexture2D> GetSubTexture(uint32_t x, uint32_t y, uint32_t width, uint32_t height);


    private:
        Ref<Texture2D> m_Texture;
        glm::vec2 m_CellSize = { 1.0f, 1.0f };
    };
}
