#include "Log.h"
#include "Renderer/VertexArray.h"

#include <GL/glew.h>

namespace Ambient
{
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::None:
        return 0;
        break;
    case ShaderDataType::Float:
        return GL_FLOAT;
        break;
    case ShaderDataType::Float2:
        return GL_FLOAT;
        break;
    case ShaderDataType::Float3:
        return GL_FLOAT;
        break;
    case ShaderDataType::Float4:
        return GL_FLOAT;
        break;
    case ShaderDataType::Mat3:
        return GL_FLOAT;
        break;
    case ShaderDataType::Mat4:
        return GL_FLOAT;
        break;
    case ShaderDataType::Int:
        return GL_INT;
        break;
    case ShaderDataType::Int2:
        return GL_INT;
        break;
    case ShaderDataType::Int3:
        return GL_INT;
        break;
    case ShaderDataType::Int4:
        return GL_INT;
        break;
    case ShaderDataType::Bool:
        return GL_BOOL;
        break;
    }

    AM_CORE_ERROR("Unknown ShaderDataType");
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
    glGenVertexArrays(1, &m_RendererID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void OpenGLVertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void OpenGLVertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertex_buffer)
{
    glBindVertexArray(m_RendererID);
    vertex_buffer->Bind();

    if (vertex_buffer->GetLayout().GetElements().size() == 0)
    {
        AM_CORE_ERROR("Trying to add a vertex buffer with no layout to the vertex array");
    }

    uint32_t index = 0;
    const auto &layout = vertex_buffer->GetLayout();
    for (const auto &element : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
                              element.IsNormalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                              (const void *)element.Offset);

        index++;
    }

    m_VertexBuffers.push_back(vertex_buffer);
}

void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &index_buffer)
{
    glBindVertexArray(m_RendererID);
    index_buffer->Bind();
    m_IndexBuffer = index_buffer;
}

} // namespace Ambient
