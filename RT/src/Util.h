#pragma once

#include <glm/glm.hpp>
#include <string>

const float PI = 3.14159265359f;

float clamp(float min, float max, float value);
float lerp(float start, float end, float percent);
float deg2rad(float degrees);

std::string to_string(int value, int digits = 2);
std::string trim(const std::string& string);