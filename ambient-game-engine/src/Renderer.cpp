#include "Renderer/Renderer.h"

#include <GL/glew.h>

namespace Ambient {
void Renderer::Draw() {
    unsigned int buffer;
    float positions[] = {-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f};

    // glGenBuffers(1, &buffer);
    // glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions,
    // GL_STATIC_DRAW); glDrawArrays(GL_TRIANGLES, 0, 3);
}
} // namespace Ambient
