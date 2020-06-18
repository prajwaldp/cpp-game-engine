#include <GL/glew.h>

#include "Log.h"
#include "Renderer/Buffer.h"

namespace Ambient {

/*****************************
 * Vertex Buffer Definitions *
 * ***************************/

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
    // glCreateBuffers is only available in OpenGL 4.5 onwards
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    AM_CORE_TRACE(
        "Reached the end of OpenGLVertexBuffer::OpenGLVertexBuffer()");
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void OpenGLVertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/****************************
 * Index Buffer Definitions *
 * **************************/

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : m_Count(count) {
    // glCreateBuffers is only available in OpenGL 4.5 onwards
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
                 GL_STATIC_DRAW);

    AM_CORE_TRACE("Reached the end of OpenGLIndexBuffer::OpenGLIndexBuffer()");
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}  // namespace Ambient
