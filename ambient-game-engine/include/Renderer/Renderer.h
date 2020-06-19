#pragma once

namespace Ambient
{
enum class RendererAPI
{
    None = 0,
    OpenGL = 1
};

class Renderer
{
  public:
    // TODO - Make private and add a getter
    static RendererAPI s_RendererAPI;
    inline static RendererAPI GetAPI()
    {
        return s_RendererAPI;
    }

    // Draw the contents of the buffer (vertex buffer) on the screen
    // This is done using the shader
    // OpenGL operates like a state machine (the vertex buffer and shader can be
    // chosen for each call)
    void Draw();
};
} // namespace Ambient
