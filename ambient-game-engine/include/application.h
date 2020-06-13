#pragma once

#include "event.h"
#include "layer_stack.h"
#include "window.h"

namespace Ambient {
class Application {
 public:
  Application();
  virtual ~Application();
  void Run();
  void OnEvent(Event::Event&);

  void PushLayer(Layer* layer);
  void PushOverlay(Layer* overlay);

 private:
  bool OnWindowClosed(Event::WindowCloseEvent&);

  std::unique_ptr<Window> m_Window;
  bool m_Running = true;

  LayerStack m_LayerStack;  // stack allocated ?
};

Application* CreateApplication();
}  // namespace Ambient
