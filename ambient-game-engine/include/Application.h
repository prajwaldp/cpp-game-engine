#pragma once

#include "Event.h"
#include "LayerStack.h"
#include "Log.h"
#include "Timestep.h"
#include "Window.h"

namespace Ambient
{
class Application
{
  public:
    Application();
    virtual ~Application();
    void Run();
    void OnEvent(Event::Event&);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    static Application* s_Instance;

    inline Window& GetWindow()
    {
        return *m_Window;
    }

    inline static Application& Get()
    {
        return *s_Instance;
    }

  private:
    bool OnWindowClosed(Event::WindowCloseEvent&);

    std::unique_ptr<Window> m_Window;
    bool m_Running = true;

    LayerStack m_LayerStack; // stack allocated ?
    float m_LastFrameTime;
};

Application* CreateApplication();
} // namespace Ambient
