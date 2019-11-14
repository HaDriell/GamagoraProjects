#pragma once

#include "../../Common.h"
#include "../Layer.h"

class FPSLayer : public Layer
{
private:
    std::string title;
    float fpsLogCooldown;
    float fpsLogAge;

    //Memory structure
    unsigned int frameTimeCapacity;
    unsigned int frameTimeIndex;
    float *      frameTime;

public:
    FPSLayer(const std::string& title = "Window ", float fpsLogCooldown = 0.5f, unsigned int frameTimeCapacity = 60);
    ~FPSLayer();
    virtual void onUpdate(float deltaTime) override;
};