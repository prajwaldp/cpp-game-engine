#pragma once

namespace Ambient {
class Renderer {
  public:
    // Draw the contents of the buffer (vertex buffer) on the screen
    // This is done using the shader
    // OpenGL operates like a state machine (the vertex buffer and shader can be
    // chosen for each call)
    void Draw();
};
} // namespace Ambient
