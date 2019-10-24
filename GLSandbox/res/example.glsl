//////////////////////////////////////////
//Vertex Shader
#version 330 core

struct Vertex {
    vec4 position;
    vec4 normal;
    vec4 color;
    vec2 uv;
};

//Constants
uniform mat4 model;

//Vertex Layout
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 uv;

//Interface
out Vertex vertex;

void main()
{
    vertex.position = model * vec4(position, 1.0);
    vertex.normal   = model * vec4(normal, 0.0);
    vertex.color    = color;
    vertex.uv       = uv;

    gl_Position = vertex.position;
}


//////////////////////////////////////////
//Fragment Shader
#version 330 core

struct Vertex {
    vec4 position;
    vec4 normal;
    vec4 color;
    vec2 uv;
};

out vec4 PixelColor;


in Vertex vertex;

void main()
{
    PixelColor = vertex.color;
}