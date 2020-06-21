#pragma once

#include "Renderer/Buffer.h"
#include "Types.h"

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

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) = 0;

        virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) = 0;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;

        virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

        static VertexArray* Create();
    };

    class OpenGLVertexArray : public VertexArray
    {
        uint32_t m_RendererID;
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;

    public:
        OpenGLVertexArray();

        ~OpenGLVertexArray();

        virtual void Bind() const override;

        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;

        virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;

        inline virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override
        {
            return m_VertexBuffers;
        };

        inline virtual const Ref<IndexBuffer>& GetIndexBuffer() const override
        {
            return m_IndexBuffer;
        };
    };
} // namespace Ambient
