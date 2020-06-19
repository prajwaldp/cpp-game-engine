#pragma once

#include "Renderer/VertexArray.h"
#include <memory>

namespace Ambient
{
class RendererAPI
{
  public:
    enum class API
    {
        None = 0,
        OpenGL = 1
    };

    virtual void SetClearColor() = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray) = 0;

    inline static API GetAPI()
    {
        return s_API;
    }

  private:
    static API s_API;
};

/**
 * A static wrapper for OpenGL commands
 **/
class RenderCommand
{
  public:
    inline static void SetClearColor()
    {
        s_RendererAPI->SetClearColor();
    }

    inline static void Clear()
    {
        s_RendererAPI->Clear();
    }

    inline static void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray)
    {
        s_RendererAPI->DrawIndexed(vertexArray);
    }

  private:
    static RendererAPI *s_RendererAPI;
};

class Renderer
{

  public:
    static void BeginScene();
    static void EndScene();

    static void Submit(const std::shared_ptr<VertexArray> &vertexArray);
    static void Flush();

    // Inline functions

    inline static RendererAPI::API GetAPI()
    {
        return RendererAPI::GetAPI();
    }
};

} // namespace Ambient
