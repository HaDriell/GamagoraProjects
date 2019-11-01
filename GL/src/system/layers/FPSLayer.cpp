#include "FPSLayer.h"

FPSLayer::FPSLayer(float fpsLogCooldown, unsigned int frameTimeCapacity) : Layer("FPSLayer"), 
fpsLogCooldown(fpsLogCooldown), fpsLogAge(0.0f), frameTimeCapacity(frameTimeCapacity), frameTimeIndex(0)
{
    frameTime = new float[frameTimeCapacity];
    for (int i = 0; i < frameTimeCapacity; i++)
    {
        frameTime[i] = 0.0f; // setup to 0 by default
    }
}

FPSLayer::~FPSLayer()
{
    delete[] frameTime;
}

void FPSLayer::onRender(float deltaTime)
{
    frameTime[frameTimeIndex++] = deltaTime;
    if (frameTimeIndex >= frameTimeCapacity)
        frameTimeIndex = 0;

    fpsLogAge += deltaTime;
    if (fpsLogAge > fpsLogCooldown)
    {
        fpsLogAge = 0;

        //Compute Average
        float averageFrameTime = 0.0f;
        for (unsigned int i = 0; i < frameTimeCapacity; i++)
        {
            averageFrameTime += frameTime[i];
        }
        averageFrameTime /= (float) frameTimeCapacity;
        float averageFrameFrequency = 1.0f / averageFrameTime;

        LogDebug("Average frame time : {0}. Average FPS : {1}", averageFrameTime, averageFrameFrequency);
    }
}