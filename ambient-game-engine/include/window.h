#pragma once

#include <string>

#include "GLFW/glfw3.h"
#include "event.h"

namespace Ambient {

struct WindowProps {
  std::string Title;
  unsigned int Width;
  unsigned int Height;

  WindowProps(const std::string& title = "Ambient Engine",
              unsigned int width = 1280, unsigned int height = 720)
      : Title(title), Width(width), Height(height) {}
};

class Window {
 public:
  using EventCallbackFn = std::function<void(Ambient::Event::Event&)>;

  virtual ~Window() {}
  virtual void OnUpdate() = 0;

  virtual unsigned int GetWidth() const = 0;
  virtual unsigned int GetHeight() const = 0;

  virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
  virtual void SetVSync(bool enabled) = 0;
  virtual bool IsVsync() const = 0;

  static Window* Create(const WindowProps& props = WindowProps());
};

class MacOSWindow : public Window {
 public:
  MacOSWindow(const WindowProps& props);
  virtual ~MacOSWindow();

  void OnUpdate() override;

  inline unsigned int GetWidth() const override { return m_Data.Width; }
  inline unsigned int GetHeight() const override { return m_Data.Height; }

  inline void SetEventCallback(const EventCallbackFn& callback) override {
    m_Data.EventCallback = callback;
  }

  void SetVSync(bool enabled) override;
  bool IsVsync() const override;

 private:
  virtual void Init(const WindowProps& props);
  virtual void ShutDown();

  GLFWwindow* m_Window;

  struct WindowData {
    std::string Title;
    unsigned int Width, Height;
    bool VSync;

    EventCallbackFn EventCallback;
  };

  WindowData m_Data;
};
}  // namespace Ambient
