//////////////////////////////////////////
//Vertex Shader
#version 450 core

in vec3 position;
in vec3 color;

out vec3 vs_color;

void main()
{
    gl_Position = vec4(position, 1.0f);
    vs_color = color;
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

in vec3 vs_color;

out vec4 color;

void main()
{
    color = vec4(vs_color, 1);
}