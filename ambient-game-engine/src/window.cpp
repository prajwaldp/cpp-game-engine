#include "window.h"

#include "GLFW/glfw3.h"
#include "event.h"
#include "log.h"

namespace Ambient {
static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description) {
  AM_CORE_ERROR("GLFW Error: {0}, {1}", error, description);
}

Window* Window::Create(const WindowProps& props) {
  return new MacOSWindow(props);
}

MacOSWindow::MacOSWindow(const WindowProps& props) { Init(props); }

MacOSWindow::~MacOSWindow() { ShutDown(); }

void MacOSWindow::Init(const WindowProps& props) {
  m_Data.Title = props.Title;
  m_Data.Width = props.Width;
  m_Data.Height = props.Height;

  AM_CORE_INFO("Creating Window [{0}] <{1}, {2}>", props.Title, props.Width,
               props.Height);

  if (!s_GLFWInitialized) {
    int success = glfwInit();
    // Check success
    glfwSetErrorCallback(GLFWErrorCallback);
    s_GLFWInitialized = true;
  }

  m_Window = glfwCreateWindow(props.Width, props.Height, m_Data.Title.c_str(),
                              nullptr, nullptr);
  glfwMakeContextCurrent(m_Window);
  glfwSetWindowUserPointer(m_Window, &m_Data);
  SetVSync(true);

  // Set GLFW Callbacks
  glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    Event::WindowCloseEvent event;
    data.EventCallback(event);
  });

  glfwSetWindowSizeCallback(
      m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        Event::WindowResizeEvent event(width, height);
        data.EventCallback(event);
      });

  glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode,
                                  int action, int mods) {
    WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        Event::KeyPressedEvent event(key, 0);
        window_data.EventCallback(event);
        break;
      }

      case GLFW_RELEASE: {
        AM_CORE_WARN("GLFW Key Release not implemented");
        break;
      }

      case GLFW_REPEAT: {
        Event::KeyPressedEvent event(key, 1);
        window_data.EventCallback(event);
        break;
      }
    }
  });

  glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button,
                                          int action, int mods) {
    WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        Event::MouseButtonPressedEvent event(button);
        window_data.EventCallback(event);
        break;
      }

      case GLFW_RELEASE: {
        Event::MouseButtonReleasedEvent event(button);
        window_data.EventCallback(event);
        break;
      }
    }
  });

  glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x_offset,
                                     double y_offset) {
    WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);

    Event::MouseScrolledEvent event(x_offset, y_offset);
    window_data.EventCallback(event);
  });

  glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x_pos,
                                        double y_pos) {
    WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);

    Event::MouseMovedEvent event(x_pos, y_pos);
    window_data.EventCallback(event);
  });
}

void MacOSWindow::ShutDown() { glfwDestroyWindow(m_Window); }

void MacOSWindow::OnUpdate() {
  glfwPollEvents();
  glfwSwapBuffers(m_Window);
}

void MacOSWindow::SetVSync(bool enabled) {
  if (enabled) {
    glfwSwapInterval(1);
  } else {
    glfwSwapInterval(0);
  }

  m_Data.VSync = enabled;
}

bool MacOSWindow::IsVsync() const { return m_Data.VSync; }
}  // namespace Ambient
