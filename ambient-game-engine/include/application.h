#pragma once

namespace Ambient {
class Application {
 public:
  Application();
  virtual ~Application();
  void Run();
};

Application* CreateApplication();
}  // namespace Ambient
