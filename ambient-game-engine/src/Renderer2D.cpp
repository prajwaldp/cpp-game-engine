#include "Renderer/Renderer2D.h"

#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Types.h"

#include "glm/gtc/matrix_transform.hpp"


namespace Ambient
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> Shader;
        Ref<Texture> StandardWhiteTexture;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init()
    {
        s_Data = new Renderer2DStorage();
        s_Data->QuadVertexArray = VertexArray::Create();

        float squareVertices[4 * 5] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                +0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                +0.5f, +0.5f, 0.0f, 1.0f, 1.0f,
                -0.5f, +0.5f, 0.0f, 0.0f, 1.0f
        };

        Ref<VertexBuffer> squareVertexBuffer;
        squareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

        squareVertexBuffer->SetLayout({
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float2, "a_TexCoord" }
        });

        s_Data->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Ref<IndexBuffer> squareIndexBuffer;
        squareIndexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        s_Data->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

        s_Data->StandardWhiteTexture = Ambient::Texture2D::Create(1, 1);
        uint32_t white_texture_data = 0xffffffff;
        s_Data->StandardWhiteTexture->SetData(&white_texture_data, sizeof(uint32_t));

        // TODO Generate absolute path
        s_Data->Shader = Shader::Create("../../assets/shaders/Combined.glsl");
        s_Data->Shader->Bind();
        s_Data->Shader->SetInt("u_Texture", 0);
    }

    void Renderer2D::Shutdown()
    {
        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_Data->Shader->Bind();
        s_Data->Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0 }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        s_Data->Shader->SetFloat4("u_Color", color);
        s_Data->StandardWhiteTexture->Bind();

        glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), position) *  // translation
                glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 }); // scale

        s_Data->Shader->SetMat4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);

        s_Data->StandardWhiteTexture->Unbind();
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        DrawQuad({ position.x, position.y, 0 }, size, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        s_Data->Shader->Bind();
        texture->Bind();

        glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), position) *  // translation
                glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 }); // scale

        s_Data->Shader->SetMat4("u_Transform", transform);
        s_Data->Shader->SetFloat4("u_Color", glm::vec4(1.0f));

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);

        texture->Unbind();
    }
} // namespace Ambient