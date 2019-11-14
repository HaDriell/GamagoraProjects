#include "CameraController.h"

#include "../../System.h"

CameraController::CameraController(const mat4& projectionMatrix, const vec3& position, const vec3& orientationDegrees)
: Layer("Camera Controller"), camera(MakeRef<Camera>(projectionMatrix, position, orientationDegrees)), 
dragging(false), pitch(orientationDegrees.x), yaw(orientationDegrees.y), speed(0.1), mouseSensitivity(0.5f)
{
}


void CameraController::setPitch(float pitch)
{
    this->pitch = pitch;
}

void CameraController::setSpeed(float speed)
{
    this->speed = speed;
}

void CameraController::onLoad()
{
    vec2 windowSize = vec2(getWindow()->getWidth(), getWindow()->getHeight());
    draggingPosition = windowSize / 2;
}

void CameraController::onUpdate(float deltaTime)
{

    dragging = getWindow()->inputs().isButtonPressed(MOUSE_BUTTON_1);

    vec2 mouse = getWindow()->inputs().getMousePosition();
    if (dragging)
    {
        vec2 delta = draggingPosition - mouse;
        yaw     += delta.x * mouseSensitivity;
        pitch   += delta.y * mouseSensitivity;

        camera->setRotation(pitch, yaw, 0);
    }
    draggingPosition = mouse;

    //Camera Translation movements
    mat4 verticalRotation = mat4::RotationX(-pitch);
    mat4 horizontalRotation = mat4::RotationY(-yaw);

    vec3 forward = verticalRotation * horizontalRotation * -vec3::Z;
    vec3 right   = horizontalRotation * vec3::X;
    vec3 up      = vec3::Y;
    vec3 movementDirection = vec3(0);
    if (getWindow()->inputs().isKeyPressed(KEY_W)) movementDirection += forward;
    if (getWindow()->inputs().isKeyPressed(KEY_S)) movementDirection -= forward;
    if (getWindow()->inputs().isKeyPressed(KEY_A)) movementDirection -= right;
    if (getWindow()->inputs().isKeyPressed(KEY_D)) movementDirection += right;
    if (getWindow()->inputs().isKeyPressed(KEY_SPACE)) movementDirection += up;
    if (getWindow()->inputs().isKeyPressed(KEY_C)) movementDirection -= up;
    movementDirection = movementDirection.normalize();
    
    camera->translate(movementDirection * speed);

    //Quick exit control
    if (getWindow()->inputs().isKeyPressed(KEY_ESCAPE)) getWindow()->close();
}

void CameraController::onUnload()
{
}

Ref<Camera> CameraController::getCamera() const
{
    return camera; 
}