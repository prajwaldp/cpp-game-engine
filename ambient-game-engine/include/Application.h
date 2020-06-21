#pragma once

#include "Event.h"
#include "LayerStack.h"
#include "Log.h"
#include "Timestep.h"
#include "Window.h"

namespace Ambient
{
    class Application
    {
    public:
        Application();

        virtual ~Application();

        void Run();

        void OnEvent(Event&);

        void PushLayer(Layer* layer);

        void PushOverlay(Layer* overlay);

        static Application* s_Instance;

        inline Window& GetWindow()
        {
            return *m_Window;
        }

        inline static Application& Get()
        {
            return *s_Instance;
        }

    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        bool m_Minimized = false;

        LayerStack m_LayerStack; // stack allocated ?
        float m_LastFrameTime;

        bool OnWindowClosed(WindowCloseEvent&);

        bool OnWindowResized(WindowResizeEvent&);
    };

    Application* CreateApplication();
} // namespace Ambient
