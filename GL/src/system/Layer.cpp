 #include "Layer.h"


Layer::Layer(const std::string& name) : name(name), window(nullptr)
{
}

const std::string& Layer::getName() const
{
    return name;
}

Window* Layer::getWindow()
{
    return window;
}


void Layer::setWindow(Window* window)
{
    if (this->window) onUnload();
    this->window = window;
    if (this->window) onLoad();
}

void Layer::onLoad()
{
}

void Layer::onUnload()
{
}

void Layer::onUpdate(float deltaTime)
{
}

void Layer::onRender()
{
}