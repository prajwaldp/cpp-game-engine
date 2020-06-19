#pragma once

#include "Renderer/Buffer.h"

#include <memory>
#include <vector>

namespace Ambient
{
class VertexArray
{
  public:
    virtual ~VertexArray()
    {
    }

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertex_buffer) = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &index_buffer) = 0;

    virtual const std::vector<std::shared_ptr<VertexBuffer>> &GetVertexBuffers() const = 0;
    virtual const std::shared_ptr<IndexBuffer> &GetIndexBuffer() const = 0;

    static VertexArray *Create();
};

class OpenGLVertexArray : public VertexArray
{
    uint32_t m_RendererID;
    std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;

  public:
    OpenGLVertexArray();
    ~OpenGLVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertex_buffer) override;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &index_buffer) override;

    inline virtual const std::vector<std::shared_ptr<VertexBuffer>> &GetVertexBuffers() const override
    {
        return m_VertexBuffers;
    };

    inline virtual const std::shared_ptr<IndexBuffer> &GetIndexBuffer() const override
    {
        return m_IndexBuffer;
    };
};
} // namespace Ambient
