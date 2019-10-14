#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "../Events.h"

struct WindowSettings
{
    std::string title  = "GL Window";
    int glMajorVersion = 3; 
    int glMinorVersion = 2;
    int width          = 800;
    int height         = 450;
    bool decorated     = true;
};

class Window
{
public:
    //Reference counter to track when GLFW should init & terminate
    static int windowCount;

private:
    std::string title;
    unsigned int width;
    unsigned int height;
    GLFWwindow* handle;
    EventSystem events;
    
public:
    Window(const WindowSettings& settings = WindowSettings());
    ~Window();
    void update();

    bool shouldClose() const;

    std::string getTitle() const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void show();
    void hide();
    void close();
    bool isVisible() const;

    //Window Events
    friend void OnWindowMoved(GLFWwindow* window, int x, int y);
    friend void OnWindowResized(GLFWwindow* window, int width, int height);
    friend void OnWindowFocus(GLFWwindow* window, int focus);
    friend void OnWindowClosed(GLFWwindow* window);
    //Key Events
    friend void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    friend void OnChar(GLFWwindow* window, unsigned int codepoint);
    //Mouse Events
    friend void OnMouseMoved(GLFWwindow* window, double x, double y);
    friend void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
    friend void OnMouseScrolled(GLFWwindow* window, double xOffset, double yOffset);
};