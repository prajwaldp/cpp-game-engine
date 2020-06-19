#include "Renderer/Renderer.h"

#include <GL/glew.h>

namespace Ambient
{
// Set the Renderer API to OpenGL
// Future support for Direct3D, Vulkan, etc.?
RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;

void Renderer::Draw()
{
    unsigned int buffer;
    float positions[] = {-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f};

    // glGenBuffers(1, &buffer);
    // glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions,
    // GL_STATIC_DRAW); glDrawArrays(GL_TRIANGLES, 0, 3);
}
} // namespace Ambient
