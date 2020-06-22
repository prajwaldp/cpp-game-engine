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
    };

    struct Renderer2Ddata
    {
        // Max quads that can be drawn in a single draw call
        const uint32_t MAX_QUADS = 10000;
        const uint32_t MAX_VERTICES = 4 * 10000;
        const uint32_t MAX_INDICES = 6 * 10000;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> Shader;
        Ref<Texture> StandardWhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;
    };

    static Renderer2Ddata s_Data;

    void Renderer2D::Init()
    {
        s_Data.QuadVertexArray = VertexArray::Create();
        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MAX_VERTICES * sizeof(QuadVertex));

        s_Data.QuadVertexBuffer->SetLayout({
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" },
                { ShaderDataType::Float2, "a_TexCoord" }
        });

        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MAX_VERTICES];

        uint32_t* quad_indices = new uint32_t[s_Data.MAX_INDICES]; // avoid stack overflow. Remember to delete

        for (uint32_t i = 0, offset = 0; i < s_Data.MAX_INDICES; i += 6, offset += 4)
        {
            quad_indices[i + 0] = offset + 0;
            quad_indices[i + 1] = offset + 1;
            quad_indices[i + 2] = offset + 2;

            quad_indices[i + 3] = offset + 2;
            quad_indices[i + 4] = offset + 3;
            quad_indices[i + 5] = offset + 0;
        }

        Ref<IndexBuffer> quad_ib = IndexBuffer::Create(quad_indices, s_Data.MAX_INDICES);
        s_Data.QuadVertexArray->SetIndexBuffer(quad_ib);
        delete[] quad_indices;

        s_Data.StandardWhiteTexture = Texture2D::Create(1, 1);
        uint32_t white_texture_data = 0xffffffff;
        s_Data.StandardWhiteTexture->SetData(&white_texture_data, sizeof(uint32_t));

        // TODO Generate absolute path
        s_Data.Shader = Shader::Create("../../assets/shaders/Combined.glsl");
        s_Data.Shader->Bind();
        s_Data.Shader->SetInt("u_Texture", 0);
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
    }

    void Renderer2D::EndScene()
    {
        uint32_t data_size = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, data_size);
        Flush();
    }

    void Renderer2D::Flush()
    {
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0 }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        s_Data.QuadVertexBufferPtr->Position = position;
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };;
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;

        /*
        s_Data.Shader->SetFloat4("u_Color", color);
        s_Data.StandardWhiteTexture->Bind();

        glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), position) *  // translation
                glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 }); // scale

        s_Data.Shader->SetMat4("u_Transform", transform);
        s_Data.Shader->SetFloat("u_TileRepeatCount", 1.0f);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);

        s_Data.StandardWhiteTexture->Unbind();
        */
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        DrawQuad({ position.x, position.y, 0 }, size, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        s_Data.Shader->Bind();
        texture->Bind();

        glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), position) *  // translation
                glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 }); // scale

        s_Data.Shader->SetMat4("u_Transform", transform);
        s_Data.Shader->SetFloat4("u_Color", glm::vec4(1.0f));
        s_Data.Shader->SetFloat("u_TileRepeatCount", 1.0f);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);

        texture->Unbind();
    }
} // namespace Ambient