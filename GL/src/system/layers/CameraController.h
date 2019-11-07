#pragma once

#include "../../Common.h"
#include "../Layer.h"

#include "../../Graphics.h"

class CameraController : public Layer
{
private:
    Camera  camera;
    float   speed;
    float   strafe;
    float   pitch;
    float   yaw;

    //Tracking system
    bool    dragging;
    vec2    lastMousePosition;


public:
    CameraController(const Camera& camera);

    virtual void onLoad() override;
    virtual void onUnload() override;
    virtual void onRender(float deltaTime) override;

    const Camera& getCamera() const; 
};