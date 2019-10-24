#pragma once


//Forward declaration
class Renderer2D;

class Renderable2D
{
public:
    virtual void render(Renderer2D& renderer) const = 0;
};