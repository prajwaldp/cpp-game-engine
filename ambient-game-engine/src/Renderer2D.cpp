#include "Renderer/Renderer2D.h"

#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Types.h"

#include "glm/gtc/matrix_transform.hpp"


namespace Ambient
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float RepeatCount;
    };

    struct Renderer2Ddata
    {
        // Max quads that can be drawn in a single draw call
        static const uint32_t MAX_QUADS = 10000;
        static const uint32_t MAX_VERTICES = 4 * MAX_QUADS;
        static const uint32_t MAX_INDICES = 6 * MAX_QUADS;

        static const uint32_t MAX_TEXTURE_SLOTS = 16; // Check renderer capability

        Ref<VertexArray> QuadVertexArray = nullptr;
        Ref<VertexBuffer> QuadVertexBuffer = nullptr;
        Ref<Shader> Shader = nullptr;
        Ref<Texture2D> StandardWhiteTexture = nullptr;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 = white texture

        glm::vec4 QuadVertexPositions[4] = {};

        Renderer2D::Statistics Stats;
    };

    static Renderer2Ddata s_Data;

    void Renderer2D::Init()
    {
        s_Data.QuadVertexArray = VertexArray::Create();
        s_Data.QuadVertexBuffer = VertexBuffer::Create(Renderer2Ddata::MAX_VERTICES * sizeof(QuadVertex));

        s_Data.QuadVertexBuffer->SetLayout({
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" },
                { ShaderDataType::Float2, "a_TexCoord" },
                { ShaderDataType::Float,  "a_TexIndex" },
                { ShaderDataType::Float,  "a_RepeatCount" }
        });

        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
        s_Data.QuadVertexBufferBase = new QuadVertex[Renderer2Ddata::MAX_VERTICES];

        auto quad_indices = new uint32_t[Renderer2Ddata::MAX_INDICES]; // avoid stack overflow. Remember to delete

        for (uint32_t i = 0, offset = 0; i < Renderer2Ddata::MAX_INDICES; i += 6, offset += 4)
        {
            quad_indices[i + 0] = offset + 0;
            quad_indices[i + 1] = offset + 1;
            quad_indices[i + 2] = offset + 2;

            quad_indices[i + 3] = offset + 2;
            quad_indices[i + 4] = offset + 3;
            quad_indices[i + 5] = offset + 0;
        }

        Ref<IndexBuffer> quad_ib = IndexBuffer::Create(quad_indices, Renderer2Ddata::MAX_INDICES);
        s_Data.QuadVertexArray->SetIndexBuffer(quad_ib);
        delete[] quad_indices;

        s_Data.StandardWhiteTexture = Texture2D::Create(1, 1);
        uint32_t white_texture_data = 0xffffffff;
        s_Data.StandardWhiteTexture->SetData(&white_texture_data, sizeof(uint32_t));

        int32_t samplers[Renderer2Ddata::MAX_TEXTURE_SLOTS];
        for (uint32_t i = 0; i < Renderer2Ddata::MAX_TEXTURE_SLOTS; i++)
            samplers[i] = i;

        // TODO Generate absolute path
        s_Data.Shader = Shader::Create("../../assets/shaders/Combined.glsl");
        s_Data.Shader->Bind();
        s_Data.Shader->SetIntArray("u_Textures", samplers, s_Data.MAX_TEXTURE_SLOTS);

        s_Data.TextureSlots[0] = s_Data.StandardWhiteTexture;

        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[1] = { +0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[2] = { +0.5f, +0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f, +0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::Shutdown()
    {
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_Data.Shader->Bind();
        s_Data.Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::EndScene()
    {
        uint32_t data_size = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, data_size);
        Flush();
    }

    void Renderer2D::Flush()
    {
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        {
            s_Data.TextureSlots[i]->Bind(i);
        }

        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

        s_Data.Stats.DrawCalls++;
    }

    void Renderer2D::FlushAndReset()
    {
        EndScene();
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0 }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        if (s_Data.QuadIndexCount >= Renderer2Ddata::MAX_INDICES)
        {
            FlushAndReset();
        }

        float texture_index = 0.0f, repeat_count = 1.0f;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                              * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        DrawQuad({ position.x, position.y, 0 }, size, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        if (s_Data.QuadIndexCount >= Renderer2Ddata::MAX_INDICES)
        {
            FlushAndReset();
        }

        constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        float texture_index = 0.0f, repeat_count = 1.0f;

        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                texture_index = (float)i;
                break;
            }
        }

        if (texture_index == 0.0f)
        {
            texture_index = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                              * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::DrawRotatedQuad(
            const glm::vec2& position,
            const glm::vec2& size,
            float angle,
            const glm::vec4& color
    )
    {
        DrawRotatedQuad({ position.x, position.y, 0 }, size, angle, color);
    }

    void Renderer2D::DrawRotatedQuad(
            const glm::vec3& position,
            const glm::vec2& size,
            float angle,
            const glm::vec4& color
    )
    {
        if (s_Data.QuadIndexCount >= Renderer2Ddata::MAX_INDICES)
        {
            FlushAndReset();
        }

        float texture_index = 0.0f, repeat_count = 1.0f;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                              * glm::rotate(glm::mat4(1.0f), glm::radians(angle), { 0.0f, 0.0f, 1.0f })
                              * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::DrawRotatedQuad(
            const glm::vec2& position,
            const glm::vec2& size,
            float angle,
            const Ref<Texture2D>& texture
    )
    {
        DrawRotatedQuad({ position.x, position.y, 0 }, size, angle, texture);
    }

    void Renderer2D::DrawRotatedQuad(
            const glm::vec3& position,
            const glm::vec2& size,
            float angle,
            const Ref<Texture2D>& texture
    )
    {
        if (s_Data.QuadIndexCount >= Renderer2Ddata::MAX_INDICES)
        {
            FlushAndReset();
        }

        constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        float texture_index = 0.0f, repeat_count = 1.0f;

        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                texture_index = (float)i;
                break;
            }
        }

        if (texture_index == 0.0f)
        {
            texture_index = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                              * glm::rotate(glm::mat4(1.0f), glm::radians(angle), { 0.0f, 0.0f, 1.0f })
                              * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexIndex = texture_index;
        s_Data.QuadVertexBufferPtr->RepeatCount = repeat_count;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadCount++;
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }

    void Renderer2D::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Renderer2D::Statistics));
    }
} // namespace Ambient