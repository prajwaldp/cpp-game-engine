#pragma once

#include <string>
#include <vector>

#include "Log.h"

namespace Ambient {

enum class ShaderDataType {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::None:
            return 0;
            break;
        case ShaderDataType::Float:
            return 4;
            break;
        case ShaderDataType::Float2:
            return 4 * 2;
            break;
        case ShaderDataType::Float3:
            return 4 * 3;
            break;
        case ShaderDataType::Float4:
            return 4 * 4;
            break;
        case ShaderDataType::Mat3:
            return 3 * 3;
            break;
        case ShaderDataType::Mat4:
            return 4 * 4;
            break;
        case ShaderDataType::Int:
            return 4;
            break;
        case ShaderDataType::Int2:
            return 4 * 2;
            break;
        case ShaderDataType::Int3:
            return 4 * 3;
            break;
        case ShaderDataType::Int4:
            return 4 * 4;
            break;
        case ShaderDataType::Bool:
            return 1;
            break;
    }

    AM_CORE_ERROR("Unknown ShaderDataType");
    return 0;
}

struct BufferElement {
    ShaderDataType Type;
    std::string Name;
    uint32_t Offset;
    uint32_t Size;
    bool IsNormalized;

    BufferElement(ShaderDataType type,
                  const std::string& name,
                  bool normalized = false)
        : Name(name),
          Type(type),
          Size(ShaderDataTypeSize(type)),
          Offset(0),
          IsNormalized(normalized) {}

    uint32_t GetComponentCount() const {
        switch (Type) {
            case ShaderDataType::None:
                return 0;
                break;
            case ShaderDataType::Float:
                return 1;
                break;
            case ShaderDataType::Float2:
                return 2;
                break;
            case ShaderDataType::Float3:
                return 3;
                break;
            case ShaderDataType::Float4:
                return 4;
                break;
            case ShaderDataType::Mat3:
                return 3 * 3;
                break;
            case ShaderDataType::Mat4:
                return 4 * 4;
                break;
            case ShaderDataType::Int:
                return 1;
                break;
            case ShaderDataType::Int2:
                return 2;
                break;
            case ShaderDataType::Int3:
                return 3;
                break;
            case ShaderDataType::Int4:
                return 4;
                break;
            case ShaderDataType::Bool:
                return 1;
                break;
        }

        AM_CORE_ERROR("Unknown ShaderDataType");
        return 0;
    }
};

class BufferLayout {
    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;

    void CalculateOffsetAndStride() {
        m_Stride = 0;
        uint32_t offset = 0;

        for (auto& element : m_Elements) {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }

   public:
    BufferLayout() {}

    BufferLayout(const std::initializer_list<BufferElement>& elements)
        : m_Elements(elements) {
        CalculateOffsetAndStride();
    }

    inline uint32_t GetStride() const { return m_Stride; }

    inline const std::vector<BufferElement>& GetElements() const {
        return m_Elements;
    }

    std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

    std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
};

class VertexBuffer {
   public:
    virtual ~VertexBuffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual const BufferLayout& GetLayout() const = 0;
    virtual void SetLayout(const BufferLayout& layout) = 0;

    static VertexBuffer* Create(float* vertices, uint32_t size);
};

class IndexBuffer {
   public:
    virtual ~IndexBuffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetCount() const = 0;

    static IndexBuffer* Create(uint32_t* indices, uint32_t count);
};

/*
 VertexBuffer and IndexBuffer for the OpenGL graphics API
*/

class OpenGLVertexBuffer : public VertexBuffer {
    uint32_t m_RendererID;
    BufferLayout m_Layout;

   public:
    OpenGLVertexBuffer(float* vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    inline virtual const BufferLayout& GetLayout() const override {
        return m_Layout;
    }

    inline virtual void SetLayout(const BufferLayout& layout) override {
        m_Layout = layout;
    }
};

class OpenGLIndexBuffer : public IndexBuffer {
    uint32_t m_RendererID;
    uint32_t m_Count;

   public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
    virtual ~OpenGLIndexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual uint32_t GetCount() const override { return m_Count; }
};

}  // namespace Ambient
