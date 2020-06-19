#include "Application.h"
#include "Log.h"
#include "Renderer/Renderer.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Ambient
{
Application *Application::s_Instance = nullptr;

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

Application::Application()
{
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

    /**
     * Draw a triangle
     * => Create a VertexArray
     * => Create a VertexBuffer and IndexBuffer
     * => Add the VertexBuffer and IndexBuffer to the VertexArray
     **/

    // clang-format off
    float triangleVertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };
    // clang-format on

    m_VertexArray.reset(VertexArray::Create());

    std::shared_ptr<VertexBuffer> triangleVertexBuffer;
    triangleVertexBuffer.reset(VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));

    BufferLayout layout = {{ShaderDataType::Float3, "a_Position"}, {ShaderDataType::Float4, "a_Color"}};
    triangleVertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(triangleVertexBuffer);

    uint32_t indices[3] = {0, 1, 2};
    std::shared_ptr<IndexBuffer> triangleIndexBuffer;
    triangleIndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    m_VertexArray->SetIndexBuffer(triangleIndexBuffer);

    /**
     * Draw a Square
     **/

    // clang-format off
    float squareVertices[3 * 4] = {
        -0.75f, -0.75f, 0.0f,
         0.75f, -0.75f, 0.0f,
         0.75f,  0.75f, 0.0f,
        -0.75f,  0.75f, 0.0f
    };
    // clang-format on

    m_SquareVertexArray.reset(VertexArray::Create());

    std::shared_ptr<VertexBuffer> squareVertexBuffer;
    squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

    BufferLayout squareLayout = {{ShaderDataType::Float3, "a_Position"}};
    squareVertexBuffer->SetLayout(squareLayout);
    m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
    std::shared_ptr<IndexBuffer> squareIndexBuffer;
    squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

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

    std::string vertexShaderSrc = R"(
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

    std::string fragmentShaderSrc = R"(
       #version 410 core

       layout(location = 0) out vec4 color;

       in vec3 v_Position;
       in vec4 v_Color;

       void main() {
           color = v_Color;
       }
   )";

    m_Shader.reset(new Shader(vertexShaderSrc, fragmentShaderSrc));
    // instead of m_Shader = std::make_unique<Shader>()

    std::string squareVertexShaderSrc = R"(
       #version 410 core

       layout(location = 0) in vec3 a_Position;

       out vec3 v_Position;

       void main() {
           v_Position = a_Position;
           gl_Position = vec4(a_Position, 1.0);
       }
   )";

    std::string squareFragmentShaderSrc = R"(
       #version 410 core

       layout(location = 0) out vec4 color;

       in vec3 v_Position;

       void main() {
           color = vec4(0.2, 0.1, 0.98, 1.0);
       }
   )";

    m_SquareShader.reset(new Shader(squareVertexShaderSrc, squareFragmentShaderSrc));
}

Application::~Application()
{
}

void Application::Run()
{
    while (m_Running)
    {
        RenderCommand::SetClearColor();
        RenderCommand::Clear();

        Renderer::BeginScene();

        // DirectX requires binding before the vertex buffer is created
        // Because the layout has to correspond

        m_SquareShader->Bind();
        Renderer::Submit(m_SquareVertexArray);

        m_Shader->Bind();
        Renderer::Submit(m_VertexArray);

        Renderer::EndScene();

        Renderer::Flush(); // Usually done on a separate thread

        for (auto layer : m_LayerStack)
        {
            layer->OnUpdate();
        }

        m_Window->OnUpdate();
    }
}

void Application::PushLayer(Layer *layer)
{
    m_LayerStack.Push(layer);
}

void Application::PushOverlay(Layer *overlay)
{
    m_LayerStack.PushOverlay(overlay);
}

void Application::OnEvent(Event::Event &e)
{
    Event::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Event::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClosed));

    AM_CORE_TRACE("{0}", e.ToString());

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(e);
        if (e.Handled)
        {
            break;
        }
    }
}

bool Application::OnWindowClosed(Event::WindowCloseEvent &e)
{
    m_Running = false;
    return true;
}
} // namespace Ambient
