#pragma once

const float PI = 3.14159265358f;

const float deg2rad = PI / 180.f;
const float rad2deg = 180.f / PI;

float clamp(float min, float max, float value);
float lerp(float start, float end, float percent);