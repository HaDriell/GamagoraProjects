#include "FPSLayer.h"

#include "../Window.h"

FPSLayer::FPSLayer(const std::string& title, float fpsLogCooldown, unsigned int frameTimeCapacity) : Layer("FPSLayer"), 
title(title), fpsLogCooldown(fpsLogCooldown), fpsLogAge(0.0f), frameTimeCapacity(frameTimeCapacity), frameTimeIndex(0)
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

void FPSLayer::onUpdate(float deltaTime)
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
        getWindow()->setTitle(title + " Average Frame time : " + std::to_string(averageFrameTime) + " FPS : " + std::to_string(averageFrameFrequency));
    }
}