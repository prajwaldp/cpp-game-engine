#include "Renderer/Renderer.h"

#include <GL/glew.h>

namespace Ambient
{
// Set the Renderer API to OpenGL
// Future support for Direct3D, Vulkan, etc.?
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

void Renderer::BeginScene(OrthographicCamera& camera)
{
    m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray)
{
    // DirectX requires binding before the vertex buffer is created
    // Because the layout has to correspond

    shader->Bind();
    shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

void Renderer::Flush()
{
    // TODO FLush the renderer (in a different thread)
}

} // namespace Ambient
