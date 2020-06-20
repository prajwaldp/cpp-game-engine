#include "Renderer/OpenGLRenderer.h"

#include "GL/glew.h"

namespace Ambient
{
void OpenGLRendererAPI::SetClearColor()
{
    glClearColor(0.1, 0.1, 0.9, 1.0);
}

void OpenGLRendererAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
{
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}
} // namespace Ambient
