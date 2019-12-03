//////////////////////////////////////////
//Compute Shader
#version 450 core

layout(std430) struct Particle
{
    vec2    position;
    vec2    speed;
    float   mass;
    vec3    color;
    float   lifespan;
};

//Work group configurations
layout(local_size_x = 32) in;

layout(std430, binding = 0) buffer parts
{
    Particle particles[];
};

void main() {
    
}