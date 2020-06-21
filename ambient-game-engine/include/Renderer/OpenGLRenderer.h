#include "Renderer/Renderer.h"
#include "Types.h"

namespace Ambient
{
    class OpenGLRendererAPI : public RendererAPI
    {
        void Init() override;

        void SetClearColor() override;

        void Clear() override;

        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
    };
} // namespace Ambient
