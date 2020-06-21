#pragma once

#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Ambient
{
    class OpenGLContext : public GraphicsContext
    {
    private:
        GLFWwindow* m_WindowHandle;

    public:
        OpenGLContext(GLFWwindow* window_handle);

        virtual void Init() override;

        virtual void SwapBuffers() override;
    };
} // namespace Ambient
