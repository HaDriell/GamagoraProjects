//////////////////////////////////////////
//Vertex Shader
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec4 uv;

uniform mat4 model;

out vec4 vertexColor;

void main()
{
    gl_Position = model * vec4(position, 1.0);
    vertexColor = color;
}


//////////////////////////////////////////
//Fragment Shader
#version 330 core

out vec4 color;

in vec4 vertexColor;

void main()
{
    color = vertexColor;
}