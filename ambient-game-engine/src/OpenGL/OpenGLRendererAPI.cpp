#include "Renderer/OpenGLRenderer.h"
#include "Types.h"

#include "GL/glew.h"

namespace Ambient
{
    void OpenGLRendererAPI::Init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetClearColor()
    {
        glClearColor(0.1, 0.1, 0.9, 1.0);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count)
    {
        // If the count is specified (i.e. is not 0), use it
        uint32_t ib_count = index_count == 0 ? vertex_array->GetIndexBuffer()->GetCount() : index_count;

        glDrawElements(GL_TRIANGLES, ib_count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
} // namespace Ambient
