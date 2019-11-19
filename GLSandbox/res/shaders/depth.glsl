//////////////////////////////////////////
//Vertex Shader
#version 450 core

//Vertex Layout
in layout (location = 0) vec3 Position;

//Interface
out vec4 vs_Position;

//Parameters
uniform mat4 ModelMatrix      = mat4(1);
uniform mat4 ViewMatrix       = mat4(1);
uniform mat4 ProjectionMatrix = mat4(1);


void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0f);
    vs_Position = ViewMatrix * ModelMatrix * vec4(Position, 1.0f);
}

//////////////////////////////////////////
//Fragment Shader
#version 450 core

//Interface
in vec4 vs_Position;

//Output
out float fs_Depth;

void main()
{ 
    fs_Depth = length(vs_Position.xyz);
}