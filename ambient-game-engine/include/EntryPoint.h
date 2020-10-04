#pragma once

#include "Log.h"

// This function has to be defined in the client
extern Ambient::Application* Ambient::CreateApplication();

int main(int argc, char** argv)
{
    Ambient::Logger::Init();
    AM_CORE_INFO("Initialized Logger");

    Ambient::Application* app = Ambient::CreateApplication();
    app->Run();

    delete app;
    return 0;
}
