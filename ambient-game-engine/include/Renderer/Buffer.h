#pragma once

namespace Ambient {
class VertexBuffer {
   public:
    virtual ~VertexBuffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    // VertexBuffer& if I want a referencce counting system
    static VertexBuffer* Create(float* vertices, uint32_t size);
};

class IndexBuffer {
   public:
    virtual ~IndexBuffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetCount() const = 0;

    // IndexBuffer& if I want a referencce counting system
    static IndexBuffer* Create(uint32_t* indices, uint32_t count);
};

/*
 VertexBuffer and IndexBuffer for the OpenGL graphics API
*/

class OpenGLVertexBuffer : public VertexBuffer {
    uint32_t m_RendererID;

   public:
    OpenGLVertexBuffer(float* vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;
};

class OpenGLIndexBuffer : public IndexBuffer {
    uint32_t m_RendererID;
    uint32_t m_Count;

   public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
    virtual ~OpenGLIndexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;

    virtual uint32_t GetCount() const { return m_Count; }
};

}  // namespace Ambient
