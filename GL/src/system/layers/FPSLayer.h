#pragma once

#include "../../Common.h"
#include "../Layer.h"

class FPSLayer : public Layer
{
private:
    float fpsLogCooldown;
    float fpsLogAge;

    //Memory structure
    unsigned int frameTimeCapacity;
    unsigned int frameTimeIndex;
    float *      frameTime;

public:
    FPSLayer(float fpsLogCooldown = 5.0f, unsigned int frameTimeCapacity = 60);
    ~FPSLayer();
    virtual void onUpdate(float deltaTime) override;
};