#include "Application.h"
#include "Renderer/Renderer.h"

#include "GLFW/glfw3.h" // For glfwGetTime

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Ambient
{
Application* Application::s_Instance = nullptr;

Application::Application()
{
    if (s_Instance != nullptr)
    {
        AM_CORE_ERROR("This is not the first instance of Application");
    }

    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

    Renderer::Init();
}

Application::~Application()
{
}

void Application::Run()
{
    while (m_Running)
    {
        float time = (float)glfwGetTime();
        Timestep timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;

        if (!m_Minimized)
        {
            for (auto layer : m_LayerStack)
            {
                layer->OnUpdate(timestep);
            }
        }

        m_Window->OnUpdate();
    }
}

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.Push(layer);
}

void Application::PushOverlay(Layer* overlay)
{
    m_LayerStack.PushOverlay(overlay);
}

void Application::OnEvent(Event::Event& e)
{
    Event::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Event::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClosed));
    dispatcher.Dispatch<Event::WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResized));

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

bool Application::OnWindowClosed(Event::WindowCloseEvent& e)
{
    m_Running = false;
    return true;
}

bool Application::OnWindowResized(Event::WindowResizeEvent& e)
{
    if (e.GetWidth() == 0 || e.GetHeight() == 0)
    {
        m_Minimized = true;
    }
    m_Minimized = false;
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

    return false;
}
} // namespace Ambient
