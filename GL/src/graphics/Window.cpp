#include "Window.h"
#include <iostream>
#include <cstdlib>

#include "../Events.h"

void error_callback(int error, const char* description);

//Start without initializing anything
int Window::windowCount = 0;

void OnWindowMoved(GLFWwindow* window, int x, int y)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    WindowMovedEvent event = WindowMovedEvent(x, y);
    w->eventSystem.dispatch(event);
}

void OnWindowResized(GLFWwindow* window, int width, int height)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    w->width = width;
    w->height = height;

    WindowResizedEvent event = WindowResizedEvent(width, height);
    w->eventSystem.dispatch(event);
}

void OnWindowFocus(GLFWwindow* window, int focus)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    if (focus)
    {
        WindowGainedFocusEvent event = WindowGainedFocusEvent();
        w->eventSystem.dispatch(event);
    }
    else
    {
        WindowLostFocusEvent event = WindowLostFocusEvent();
        w->eventSystem.dispatch(event);
    }
}

void OnWindowClosed(GLFWwindow* window)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);

    WindowClosedEvent event = WindowClosedEvent();
    w->eventSystem.dispatch(event);
}

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);

    switch (action)
    {
        case GLFW_REPEAT:
        case GLFW_PRESS:
        {
            KeyPressedEvent event = KeyPressedEvent(key);
            w->eventSystem.dispatch(event);
        }
        break;

        case GLFW_RELEASE:
        {
            KeyReleasedEvent event = KeyReleasedEvent(key);
            w->eventSystem.dispatch(event);
        }
        break;
    }
}

void OnChar(GLFWwindow* window, unsigned int codepoint)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);

    KeyTypedEvent event = KeyTypedEvent(codepoint);
    w->eventSystem.dispatch(event);
}

void OnMouseMoved(GLFWwindow* window, double x, double y)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);

    MouseMovedEvent event = MouseMovedEvent(x, y);
    w->eventSystem.dispatch(event);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);

    switch (action)
    {
        case GLFW_REPEAT:
        case GLFW_PRESS:
        {
            MouseButtonPressedEvent event = MouseButtonPressedEvent(button);
            w->eventSystem.dispatch(event);
        }
        break;

        case GLFW_RELEASE:
        {
            MouseButtonReleasedEvent event = MouseButtonReleasedEvent(button);
            w->eventSystem.dispatch(event);
        }
        break;
    }
}

void OnMouseScrolled(GLFWwindow* window, double xOffset, double yOffset)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);

    MouseScrolledEvent event = MouseScrolledEvent(xOffset, yOffset);
    w->eventSystem.dispatch(event);
}



Window::Window(const WindowSettings& settings)
{
    if (windowCount == 0)
    {
        if (!glfwInit())
        {
            std::cout << "Failed to initialize GLFW !" << std::endl;
        } else {
            glfwSetErrorCallback(error_callback);
        }
    }
    windowCount++;

    this->width = settings.width;
    this->height = settings.height;
    this->title = settings.title;

    glfwDefaultWindowHints();

    //OpenGL Context setup
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.glMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.glMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_DECORATED, settings.decorated ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); //enforce invisible creation and to delegate late initialization

    //Create the Window & Load OpenGL function pointers
    handle = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);

    if (!handle)
    {
        std::cout << "Failed to open a GLFW Window" << std::endl;
    }

    //Load the OpenGL Context using glad and glfw
    glfwMakeContextCurrent(handle);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSetWindowUserPointer(handle, this);

    //Position the window
    int mxOffset;
    int myOffset;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
    glfwGetMonitorPos(monitor, &mxOffset, &myOffset);
    glfwSetWindowPos(handle, 
        mxOffset + (vidMode->width  - settings.width)  / 2,
        myOffset + (vidMode->height - settings.height) / 2);

    //Window Events
    glfwSetWindowSizeCallback(handle,  OnWindowResized);
    glfwSetWindowPosCallback(handle,   OnWindowMoved);
    glfwSetWindowCloseCallback(handle, OnWindowClosed);
    glfwSetWindowFocusCallback(handle, OnWindowFocus);
    //Key Events
    glfwSetKeyCallback(handle,         OnKey);
    glfwSetCharCallback(handle,        OnChar);
    //Mouse Events
    glfwSetMouseButtonCallback(handle, OnMouseButton);
    glfwSetCursorPosCallback(handle,   OnMouseMoved);
    glfwSetScrollCallback(handle,      OnMouseScrolled);

    //Initialization finished, show the Window
    show();
}

Window::~Window()
{
    glfwDestroyWindow(handle);


    windowCount--;
    if (windowCount == 0)
    {
        glfwTerminate();
    }
}

void Window::update()
{
    glfwPollEvents(); // will consume events and call the callbacks
    glfwSwapBuffers(handle);
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(handle);
}

std::string Window::getTitle() const
{
    return title;
}

unsigned int Window::getWidth() const
{
    return width;
}

unsigned int Window::getHeight() const
{
    return height;
}

EventSystem& Window::events()
{
    return eventSystem;
}

void Window::setVSync(bool enabled)
{
    glfwSwapInterval(enabled);
}


void Window::setTitle(const std::string& title)
{
    glfwSetWindowTitle(handle, title.c_str());
}

void Window::show()
{
    glfwShowWindow(handle);
}

void Window::hide()
{
    glfwHideWindow(handle);
}

void Window::close()
{
    glfwSetWindowShouldClose(handle, GLFW_TRUE);
}

bool Window::isVisible() const
{
    return glfwGetWindowAttrib(handle, GLFW_VISIBLE) == GLFW_TRUE;
}


void error_callback(int error, const char* description)
{
    std::string errorString(description);
    std::cout << "GLFW Error(" << error << ") : " << errorString << std::endl;
}