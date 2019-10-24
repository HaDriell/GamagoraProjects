//////////////////////////////////////////
//Vertex Shader
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 uv;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(position, 1.0);
    gl_Normal;
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

out vec4 color;


in struct Vertex {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 uv;
} vertex;

void main()
{
    color = vertex.color;
    color = normalize(color);
}