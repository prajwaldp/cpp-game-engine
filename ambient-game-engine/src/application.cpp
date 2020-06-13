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
}  // namespace Ambient
