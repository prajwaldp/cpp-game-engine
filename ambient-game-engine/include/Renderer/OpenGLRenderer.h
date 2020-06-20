#include "Renderer/Renderer.h"
#include "Types.h"

namespace Ambient
{
class OpenGLRendererAPI : public RendererAPI
{
    virtual void Init() override;
    virtual void SetClearColor() override;
    virtual void Clear() override;

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
};
} // namespace Ambient
