#include "Application.h"

#include <GLFW/glfw3.h>

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Ambient {
Application* Application::s_Instance = nullptr;

Application::Application() {
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

    // Newer OpenGL is usually Create and not Gen and Bind
    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    float vertices[3 * 3] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                             0.0f,  0.0f,  0.5f, 0.0f};

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glGenBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

    unsigned int indices[3] = {0, 1, 2};
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

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

       out vec3 v_Position;

       layout(location = 0) in vec3 a_Position;

       void main() {
           v_Position = a_Position;
           gl_Position = vec4(a_Position, 1.0);
       }
   )";

    std::string fragment_shader_src = R"(
       #version 410 core

       layout(location = 0) out vec4 color;

       in vec3 v_Position;

       void main() {
           color = vec4(v_Position * 0.5 + 0.5, 1.0);
       }
   )";

    m_Shader.reset(new Shader(
        vertex_shader_src, fragment_shader_src)); // instead of m_Shader =
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
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        for (auto layer : m_LayerStack) {
            layer->OnUpdate();
        }

        m_Window->OnUpdate();
    }
}

void Application::PushLayer(Layer* layer) { m_LayerStack.Push(layer); }

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
} // namespace Ambient
