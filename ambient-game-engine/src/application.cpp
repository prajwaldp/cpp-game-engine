#include "application.h"

#include <iostream>

namespace Ambient {
Application::Application() {}

Application::~Application() {}

void Application::Run() {
  std::cout << "Ambient Game Engine is running" << std::endl;
  while (true) {
    ;
  }
}
}  // namespace Ambient
