#include "Renderer/Renderer.h"

#include <GL/glew.h>

namespace Ambient
{
// Set the Renderer API to OpenGL
// Future support for Direct3D, Vulkan, etc.?
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

/**
 * Takes all of our scene parameters
 * E.g. Camera parameters, lighting and environment parameters etc.
 **/
void Renderer::BeginScene()
{
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray)
{
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

void Renderer::Flush()
{
    // TODO FLush the renderer (in a different thread)
}

} // namespace Ambient
