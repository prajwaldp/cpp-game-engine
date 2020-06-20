#include "Input.h"

#include "Application.h"

#include <GLFW/glfw3.h>

namespace Ambient
{
bool MacOSInput::IsKeyPressedImpl(int keycode)
{
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, keycode);

    return state == GLFW_PRESS || state == GLFW_REPEAT;
}
} // namespace Ambient
