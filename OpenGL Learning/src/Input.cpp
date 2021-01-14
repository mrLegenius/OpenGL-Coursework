#include "Input.h"
#include "GLFW/glfw3.h"
extern GLFWwindow* window;
Input* Input::m_Instance;
Input& Input::GetInstance()
{
    if (m_Instance == nullptr)
        m_Instance = new Input;

    return *m_Instance;
}

bool Input::IsKeyPressed(int keycode)
{
    bool isDown = IsKeyDown(keycode);
    bool stateChanged = (keysPressState[keycode] != isDown);
    keysPressState[keycode] = isDown;

    return stateChanged && keysPressState[keycode] == true;
}
bool Input::IsKeyReleased(int keycode)
{
    bool isUp = IsKeyUp(keycode);
    bool stateChanged = (keysPressState[keycode] != isUp);
    keysPressState[keycode] = isUp;

    return stateChanged && keysPressState[keycode] == false;
}
bool Input::IsKeyDown(int keycode)
{
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool Input::IsKeyUp(int keycode)
{
    return !IsKeyDown(keycode);
}
bool Input::IsMouseButtonPressed(int button)
{
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

float Input::GetMouseX()
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return (float)x;
}

float Input::GetMouseY()
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return (float)y;
}

float Input::GetMouseDeltaX()
{
    float xpos = GetMouseX();
    if (lastMouseX == 0)
    {
        lastMouseX = xpos;
    }
    float delta = xpos - lastMouseX;
    lastMouseX = xpos;
    return delta;
}

float Input::GetMouseDeltaY()
{
    float ypos = GetMouseY();
    if (lastMouseY == 0)
    {
        lastMouseY = ypos;
    }
    float delta = lastMouseY - ypos;
    lastMouseY = ypos;
    return delta;
}
