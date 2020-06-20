#include "Renderer/OpenGLContext.h"
#include "Log.h"

#include <GL/glew.h>

// Load GLFW after glew.h
#include <GLFW/glfw3.h>

namespace Ambient
{
OpenGLContext::OpenGLContext(GLFWwindow* window_handle) : m_WindowHandle(window_handle)
{
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);

    // Initialized GLEW (The OpenGL Extension Loading library I chose)
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        AM_CORE_ERROR("glewInit() failed. {}", glewGetErrorString(err));
    }

    AM_CORE_INFO("OpenGL Info => Vendor: {}, Renderer: {}, Version: {}, GLSL "
                 "Version: {}",
                 glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION),
                 glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void OpenGLContext::SwapBuffers()
{
    // Swap GLFW window buffers
    glfwSwapBuffers(m_WindowHandle);
}
} // namespace Ambient
