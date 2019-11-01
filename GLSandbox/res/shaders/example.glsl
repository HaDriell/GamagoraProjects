//////////////////////////////////////////
//Vertex Shader
#version 450 core

//Vertex Layout
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec3 Color;
layout (location = 3) in vec2 UV;

//Interface
out vec3 vs_Normal;
out vec3 vs_Color;

//Parameters
uniform mat4 u_ModelMatrix;

void main()
{
    gl_Position = u_ModelMatrix * vec4(Position, 1.0);
    vs_Color = Color;
    vs_Normal = normalize((u_ModelMatrix * vec4(Normal, 0.0)).xyz);
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

//Interface
in vec3 vs_Normal;
in vec3 vs_Color;

//Output
out vec4 Color;

void main()
{
    Color = vec4(vs_Normal * vs_Color, 1);
}