#include "application.h"

namespace Ambient {
Application::Application() {
  m_Window = std::unique_ptr<Window>(Window::Create());
}

Application::~Application() {}

void Application::Run() {
  std::cout << "Ambient Game Engine is running" << std::endl;
  while (m_Running) {
    m_Window->OnUpdate();
  }
}
}  // namespace Ambient
