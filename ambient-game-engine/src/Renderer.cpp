#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Types.h"

#include <GL/glew.h>

namespace Ambient
{
    // Set the Renderer API to OpenGL
    // Future support for Direct3D, Vulkan, etc.?
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    void Renderer::Init()
    {
        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const Ref<Shader> shader,
            const Ref<VertexArray>& vertexArray,
            const glm::mat4 transformMatrix)
    {
        // DirectX requires binding before the vertex buffer is created
        // Because the layout has to correspond

        shader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection",
                m_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transformMatrix);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray, 0);
    }

    void Renderer::Flush()
    {
        // TODO FLush the renderer (in a different thread)
    }

} // namespace Ambient
