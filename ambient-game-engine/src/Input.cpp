#include "Input.h"

#include "Application.h"

#include <GLFW/glfw3.h>

namespace Ambient
{
    Input* Input::s_Instance = new MacOSInput();

    bool MacOSInput::IsKeyPressedImpl(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, keycode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }
} // namespace Ambient
