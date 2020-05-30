#pragma once

#include "event.h"
#include "window.h"

namespace Ambient {
class Application {
 public:
  Application();
  virtual ~Application();
  void Run();

 private:
  std::unique_ptr<Window> m_Window;
  bool m_Running = true;
};

Application* CreateApplication();
}  // namespace Ambient
