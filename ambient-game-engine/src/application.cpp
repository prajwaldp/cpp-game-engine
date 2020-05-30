#include "application.h"

#include "log.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Ambient {
Application::Application() {
  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
}

Application::~Application() {}

void Application::Run() {
  std::cout << "Ambient Game Engine is running" << std::endl;
  while (m_Running) {
    m_Window->OnUpdate();
  }
}

void Application::OnEvent(Event::Event& e) {
  Event::EventDispatcher dispatcher(e);
  dispatcher.Dispatch<Event::WindowCloseEvent>(
      BIND_EVENT_FN(Application::OnWindowClosed));

  AM_CORE_TRACE("{0}", e.ToString());
}

bool Application::OnWindowClosed(Event::WindowCloseEvent& e) {
  m_Running = false;
  return true;
}
}  // namespace Ambient
