#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Types.h"

#include <glm/glm.hpp>
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

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

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

    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        s_RendererAPI->DrawIndexed(vertexArray);
    }

  private:
    static RendererAPI* s_RendererAPI;
};

class Renderer
{

  public:
    static void BeginScene(OrthographicCamera& camera);
    static void EndScene();

    static void Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray,
                       const glm::mat4 transformMatrix = glm::mat4(1.0f));
    static void Flush();

    // Inline functions

    inline static RendererAPI::API GetAPI()
    {
        return RendererAPI::GetAPI();
    }

  private:
    struct SceneData
    {
        glm::mat4 ViewProjectionMatrix;
    };

    static SceneData* m_SceneData;
};

} // namespace Ambient
