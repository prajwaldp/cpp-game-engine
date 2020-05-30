#pragma once

#include "event.h"
#include "window.h"

namespace Ambient {
class Application {
 public:
  Application();
  virtual ~Application();
  void Run();
  void OnEvent(Event::Event&);

 private:
  bool OnWindowClosed(Event::WindowCloseEvent&);

  std::unique_ptr<Window> m_Window;
  bool m_Running = true;
};

Application* CreateApplication();
}  // namespace Ambient
