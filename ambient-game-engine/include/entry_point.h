#pragma once

// This function has to be defined in the client
extern Ambient::Application* Ambient::CreateApplication();

int main(int argc, char** argv) {
  Ambient::Application* app = Ambient::CreateApplication();
  app->Run();
  delete app;
  return 0;
}
