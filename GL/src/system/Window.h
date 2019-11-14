#pragma once

#include "../Common.h"

#include "Input.h"
#include "EventSystem.h"
#include "Layer.h"


struct WindowSettings
{
    std::string title   = "Window";
    int glMajorVersion  = 4; 
    int glMinorVersion  = 5;
    bool glCoreProfile  = false;
    int width           = 1366;
    int height          = 768;
    int multisampling   = 0;
    bool visible        = true;
    bool decorated      = true;
    bool resizeable     = true;
    bool vsync          = true;
    bool debug          = false;
};


//opaque window pointer
struct GLFWwindow;

class Window
{
private:
    GLFWwindow* handle;
    std::string title;
    unsigned int width;
    unsigned int height;

    //Internal systems
    Input       input;
    EventSystem eventSystem;
    std::vector<Layer*> layers;

    Timer updateTimer;
    
public:
    Window(const WindowSettings& settings = WindowSettings());
    ~Window();

    void update();
    void render();
    void pushLayer(Layer* layer);
    void popLayer();


    bool shouldClose() const;
    std::string getTitle() const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;


    const Input& inputs() const;
    EventSystem& events();
    void setVSync(bool enabled);
    void setTitle(const std::string& title);
    void show();
    void hide();
    void close();
    bool isVisible() const;

public:
    //////////////////////////
    //Static GLFW3 callbacks//
    //////////////////////////

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