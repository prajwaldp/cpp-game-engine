#pragma once

#include "event.h"

namespace Ambient {
class Application {
 public:
  Application();
  virtual ~Application();
  void Run();
};

Application* CreateApplication();
}  // namespace Ambient
