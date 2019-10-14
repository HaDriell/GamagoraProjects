#include "Window.h"
#include <iostream>
#include <cstdlib>

#include "../Events.h"


//Start without initializing anything
int Window::windowCount = 0;

void OnWindowMoved(GLFWwindow* window, int x, int y)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    WindowMovedEvent event = WindowMovedEvent(x, y);
    w->events.dispatch(event);
}

void OnWindowResized(GLFWwindow* window, int width, int height)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    w->width = width;
    w->height = height;

    WindowResizedEvent event = WindowResizedEvent(width, height);
    w->events.dispatch(event);
}

void OnWindowFocus(GLFWwindow* window, int focus)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);
    if (focus)
    {
        WindowGainedFocusEvent event = WindowGainedFocusEvent();
        w->events.dispatch(event);
    }
    else
    {
        WindowLostFocusEvent event = WindowLostFocusEvent();
        w->events.dispatch(event);
    }
}

void OnWindowClosed(GLFWwindow* window)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);

    WindowClosedEvent event = WindowClosedEvent();
    w->events.dispatch(event);
}

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);

    //TODO : process the key and fire events according to the context
    // w->events.dispatch(event);
}

void OnChar(GLFWwindow* window, unsigned int codepoint)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);

    KeyTypedEvent event = KeyTypedEvent(codepoint);
    w->events.dispatch(event);
}
void OnMouseMoved(GLFWwindow* window, double x, double y)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);

}

void OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);

}

void OnMouseScrolled(GLFWwindow* window, double xOffset, double yOffset)
{
    Window* w = (Window*) glfwGetWindowUserPointer(window);

}



Window::Window(const WindowSettings& settings)
{
    if (windowCount == 0)
    {
        if (!glfwInit())
        {
            std::cout << "Failed to initialize GLFW !" << std::endl;
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