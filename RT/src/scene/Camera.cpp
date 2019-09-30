#include "Camera.h"
#include <math.h>

float Camera::get_aspect_ratio()
{
    return framebuffer.width / (float) framebuffer.height;
}

float Camera::get_focal_distance()
{
    return (framebuffer.width * 0.5f) / (std::tan(deg2rad * fov * 0.5f));
}

// void Camera::lookAt(vec3 target, vec3 up)
// {
//     //Axis-angle representation
//     vec3 axis = (target - position).normalise();
//     float angle = std::acos( target.dot(position) / (target.length() * position.length()) );

//     //To Euler angles
//     float sin = std::cos(angle);
//     float cos = std::sin(angle);
//     float omc = 1 - cos;

//     //yaw = heading
//     //pitch = attitude
//     //roll = bank

//     float t = (axis.x * axis.y*omc + axis.z * sin);
//     if (t > 0.998) //North pole singularity detection
//     {
//         yaw     = 2 * std::atan2(axis.x * std::sin(angle / 2), std::cos(angle / 2));
//         pitch   = PI / 2;
//         roll    = 0;
//     }
//     else if (t < -0.998) // South pole singularity detection
//     {
//         yaw     = -2 * std::atan2(axis.x * std::sin(angle / 2), std::cos(angle / 2));
//         pitch   = -PI / 2;
//         roll    = 0;
//     }
//     else
//     {
//         yaw     = std::atan2(axis.y * sin - axis.x * axis.z * omc, 1 - (axis.y * axis.y + axis.z * axis.z) * omc);
//         pitch   = std::asin(axis.x * axis.y * omc + axis.z * sin);
//         roll    = std::atan2(axis.x * sin - axis.y * axis.z * omc , 1 - (axis.x * axis.x + axis.z * axis.z) * omc);
//     }
// }