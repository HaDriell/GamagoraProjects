#include "CameraController.h"

#include "../../System.h"


CameraController::CameraController(const Camera& camera) : camera(camera), dragging(false), pitch(camera.getRotation().x), yaw(camera.getRotation().y)
{
}


void CameraController::onLoad()
{
    getWindow()->events().add<KeyPressedEvent>([&](const KeyPressedEvent& event)
    {
        //Forward
        if (event.keycode == KEY_W) speed++;
        if (event.keycode == KEY_S) speed--;
        //Side
        if (event.keycode == KEY_A) strafe--;
        if (event.keycode == KEY_D) strafe++;
    });

    getWindow()->events().add<KeyReleasedEvent>([&](const KeyReleasedEvent& event)
    {
        //Forward
        if (event.keycode == KEY_W) speed = 0;
        if (event.keycode == KEY_S) speed = 0;
        //Side
        if (event.keycode == KEY_A) strafe = 0;
        if (event.keycode == KEY_D) strafe = 0;
    });

    //Tracking System
    getWindow()->events().add<MouseButtonPressedEvent>([&](const MouseButtonPressedEvent& event) { dragging = true; });
    getWindow()->events().add<MouseButtonReleasedEvent>([&](const MouseButtonReleasedEvent& event) { dragging = false; });
    getWindow()->events().add<MouseMovedEvent>([&](const MouseMovedEvent& event)
    {
        vec2 newMousePosition = vec2(event.x, event.y);
        if (dragging)
        {
            vec2 dv = newMousePosition - lastMousePosition;
            dv      *= 0.3f;
            
            yaw -= dv.x;
            while (yaw > 360) yaw -= 360;
            while (yaw <-360) yaw += 360;

            pitch   -= dv.y;
            //Clamp
            if (pitch > 89) pitch = 89;
            if (pitch < -89) pitch = -89;

            camera.setRotation(pitch, yaw, 0);
        }
        lastMousePosition = newMousePosition;
    });
}


void CameraController::onRender(float deltaTime)
{
    float pRad = deg2rad * pitch;
    float pcos = std::cos(pRad);
    float psin = std::sin(pRad);

    float yRad = deg2rad * yaw;
    float ycos = std::cos(yRad);
    float ysin = std::sin(yRad);

    mat4 rotation = mat4::RotationXYZ(camera.getRotation()).inverse();

    vec3 forward = (rotation * vec3::Z) * speed;
    vec3 left    = (rotation * vec3::X) * -strafe; // Y axis is inverted

    LogDebug("Forward {0} {1} {2}", forward.x, forward.y, forward.z);

    camera.translate((forward + left) * deltaTime);
}


void CameraController::onUnload()
{
}

const Camera& CameraController::getCamera() const
{
    return camera; 
}