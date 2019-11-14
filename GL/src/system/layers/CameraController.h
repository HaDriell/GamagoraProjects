#pragma once

#include "../../Common.h"
#include "../Layer.h"

#include "../../Graphics.h"

class CameraController : public Layer
{
private:
    Ref<Camera> camera;
    float mouseSensitivity;
    float speed;
    float pitch;
    float yaw;

    //Mouse Dragging system
    bool    dragging;
    vec2    draggingPosition;


public:
    CameraController(const mat4& projectionMatrix, const vec3& position = vec3(), const vec3& orientationDegrees = vec3());

    void setPitch(float pitch);
    void setSpeed(float speed);

    virtual void onLoad() override;
    virtual void onUnload() override;
    virtual void onUpdate(float deltaTime) override;

    Ref<Camera> getCamera() const;
};