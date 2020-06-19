#pragma once

#include "Event.h"
#include "LayerStack.h"
#include "Log.h"
#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Window.h"

namespace Ambient
{
class Application
{
  public:
    Application();
    virtual ~Application();
    void Run();
    void OnEvent(Event::Event &);

    void PushLayer(Layer *layer);
    void PushOverlay(Layer *overlay);

    static Application *s_Instance;

  private:
    bool OnWindowClosed(Event::WindowCloseEvent &);

    std::unique_ptr<Window> m_Window;
    bool m_Running = true;

    LayerStack m_LayerStack; // stack allocated ?
};

Application *CreateApplication();
} // namespace Ambient
