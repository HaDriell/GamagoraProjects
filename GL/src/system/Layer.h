#pragma once

#include "../Common.h"

#include "WindowEvent.h"
#include "MouseEvent.h"
#include "KeyEvent.h"

class Window;

class Layer
{
private:
    Window* window;

protected:
    std::string name;

public:
    Layer(const std::string& name = "Layer");
    const std::string& getName() const;
    const Window* getWindow() const;

    virtual void setWindow(Window* window) final;

    /////////////////////////
    //Application callbacks//
    /////////////////////////

    virtual void onLoad();
    virtual void onUnload();
    virtual void onRender(float deltaTime);
};