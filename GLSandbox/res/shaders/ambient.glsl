//////////////////////////////////////////
//Vertex Shader
#version 450 core

//Vertex Layout
in layout (location = 0) vec3 Position;
in layout (location = 2) vec3 Color;

//Interface
out vec3 vs_Color;

//Parameters
uniform mat4 ModelMatrix      = mat4(1);
uniform mat4 ViewMatrix       = mat4(1);
uniform mat4 ProjectionMatrix = mat4(1);


void main()
{
    vs_Color    = Color;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0f);
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

//Interface
in vec3 vs_Color;

//Output
out vec4 fs_Color;

void main()
{
    fs_Color = vec4(vs_Color, 1);
}