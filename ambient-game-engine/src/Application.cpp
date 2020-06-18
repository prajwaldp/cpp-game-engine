#include "Application.h"

#include <GLFW/glfw3.h>

#include "Log.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Ambient {
Application* Application::s_Instance = nullptr;

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
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

Application::Application() {
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };

    m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

    {
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" }
        };

        m_VertexBuffer->SetLayout(layout);
    }

    uint32_t index = 0;
    const auto& layout = m_VertexBuffer->GetLayout();
    for (const auto& element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,
                              element.GetComponentCount(),
                              ShaderDataTypeToOpenGLBaseType(element.Type),
                              element.IsNormalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(),
                              (const void*)element.Offset);

        index++;
    }

    uint32_t indices[3] = {0, 1, 2};
    m_IndexBuffer.reset(
        IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

    /*
    Shaders
    =======

    -> Vertex shader
        -> Runs on the GPU for every vertex (point in 3D space)
        -> Normalize vertices to the [-1, 1] space
        -> Runs 3 times for the traingle example

    -> Fragment shader
        -> Also called Pixel shader in other frameworks
        -> Runs per pixel
        -> Fills in the triangle
    */

    std::string vertex_shader_src = R"(
       #version 410 core

       layout(location = 0) in vec3 a_Position;
       layout(location = 1) in vec4 a_Color;

       out vec3 v_Position;
       out vec4 v_Color;

       void main() {
           v_Position = a_Position;
           v_Color = a_Color;
           gl_Position = vec4(a_Position, 1.0);
       }
   )";

    std::string fragment_shader_src = R"(
       #version 410 core

       layout(location = 0) out vec4 color;

       in vec3 v_Position;
       in vec4 v_Color;

       void main() {
           color = v_Color;
       }
   )";

    m_Shader.reset(new Shader(
        vertex_shader_src, fragment_shader_src));  // instead of m_Shader =
                                                   // std::make_unique<Shader>()
}

Application::~Application() {}

void Application::Run() {
    while (m_Running) {
        glClearColor(0.2f, 0.2f, 0.2f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // DirectX requires binding before the vertex buffer is created
        // Because the layout has to correspond
        m_Shader->Bind();

        glBindVertexArray(m_VertexArray);
        glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT,
                       nullptr);

        for (auto layer : m_LayerStack) {
            layer->OnUpdate();
        }

        m_Window->OnUpdate();
    }
}

void Application::PushLayer(Layer* layer) {
    m_LayerStack.Push(layer);
}

void Application::PushOverlay(Layer* overlay) {
    m_LayerStack.PushOverlay(overlay);
}

void Application::OnEvent(Event::Event& e) {
    Event::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Event::WindowCloseEvent>(
        BIND_EVENT_FN(Application::OnWindowClosed));

    AM_CORE_TRACE("{0}", e.ToString());

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(e);
        if (e.Handled) {
            break;
        }
    }
}

bool Application::OnWindowClosed(Event::WindowCloseEvent& e) {
    m_Running = false;
    return true;
}
}  // namespace Ambient
