#include "Renderer/Renderer.h"

namespace Ambient
{
class OpenGLRendererAPI : public RendererAPI
{
    virtual void SetClearColor() override;
    virtual void Clear() override;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray) override;
};
} // namespace Ambient
