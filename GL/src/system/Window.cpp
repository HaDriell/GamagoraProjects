#include "Window.h"

#include <iostream>
#include <cstdlib>
#include <atomic>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Log.h>

void APIENTRY opengl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void glfw_error_callback(int error, const char* description);

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









std::string toString_OpenGLSource(GLenum source)
{
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:               return "API";
        case GL_DEBUG_SOURCE_APPLICATION:       return "Application";
        case GL_DEBUG_SOURCE_OTHER:             return "Other";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   return "Shader Compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY:       return "Third Party";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     return "Window System";
    }
    return "Unknown";
}

std::string toString_OpenGLType(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated";
        case GL_DEBUG_TYPE_ERROR:               return "Error";
        case GL_DEBUG_TYPE_MARKER:              return "Marker";
        case GL_DEBUG_TYPE_OTHER:               return "Other";
        case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
        case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group";
        case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
        case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behavior";
    }
    return "Unknown";
}

std::string toString_OpenGLSeverity(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_SEVERITY_NOTIFICATION:    return "Notification";
        case GL_DEBUG_SEVERITY_LOW:             return "Low";
        case GL_DEBUG_SEVERITY_MEDIUM:          return "Medium";
        case GL_DEBUG_SEVERITY_HIGH:            return "High";
    }
    return "Unknown";
}

void opengl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    LogDebug("[{0}~{1}] ({2}) [{3}] : {4}",
        toString_OpenGLSource(source),
        toString_OpenGLType(type),
        id,
        toString_OpenGLSeverity(severity),
        std::string_view(message, length)
    );
}

void glfw_error_callback(int error, const char* description)
{
    std::string message(description);
    LogError("GLFW Error({0}): {1}", error, message);
}









//Window atomic counter
std::atomic_int __Windows = 0;

Window::Window(const WindowSettings& settings)
{
    int count = __Windows++;

    //Initialization required. First Window to be created
    if (count == 0)
    {
        if (!glfwInit())
        {
            LogError("Failed to initialize GLFW3 !");
        }
        else 
        {
            LogTrace("Binding Logger to the GLFW3 error callback");
            glfwSetErrorCallback(glfw_error_callback);
        }
        LogInfo("Initialized GLFW");
    }

    this->width = settings.width;
    this->height = settings.height;
    this->title = settings.title;

    glfwDefaultWindowHints();
    //OpenGL Context setup
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.glMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.glMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, settings.glCoreProfile ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);

    glfwWindowHint(GLFW_DECORATED, settings.decorated ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, settings.resizeable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); //enforce invisible creation and to delegate late initialization

    //Create the Window & Load OpenGL function pointers
    handle = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);

    if (!handle)
    {
        LogError("Failed to open Window.");
    }

    //Load the OpenGL Context using glad and glfw
    glfwMakeContextCurrent(handle);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSetWindowUserPointer(handle, this);

    //Setup the GL Debug callback
    glDebugMessageCallback(opengl_message_callback, nullptr);

    //Show the OpenGL Context Info
    LogInfo("OpenGL Context Info\n"
        "\tVendor   : {0}\n"
        "\tRenderer : {1}\n"
        "\tVersion  : {2}\n",
        glGetString(GL_VENDOR),
        glGetString(GL_RENDERER),
        glGetString(GL_VERSION)
    );

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
    setVSync(settings.vsync);
    show();
}

Window::~Window()
{
    for (auto layer = layers.rbegin(); layer != layers.rend(); layer++)
    {
        (*layer)->setWindow(nullptr); // trigger on unload
    }
    layers.clear();

    glfwDestroyWindow(handle);

    int count = __Windows--;

    //Last Window closing ! we can safely terminate the Window
    if (count == 1)
    {
        glfwTerminate();
        LogInfo("Terminated GLFW");
    }
}

void Window::update()
{
    //Update GLFW3 Window
    glfwPollEvents();
    glfwSwapBuffers(handle);
}

void Window::render()
{
    //Refresh Update timer
    float deltaTime = renderTimer.elapsed();
    renderTimer.reset();

    //Render Layers
    for (auto layer = layers.begin(); layer != layers.end(); layer++)
    {
        (*layer)->onRender(deltaTime); // trigger on unload
    }
}

void Window::pushLayer(Layer* layer)
{
    if (layer)
    {
        LogTrace("Pushing Layer '{0}'", layer->getName());
        layers.push_back(layer);
        layer->setWindow(this);
    }
}

void Window::popLayer()
{
    Layer* back = layers.back();
    layers.pop_back();
    back->setWindow(nullptr);
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
