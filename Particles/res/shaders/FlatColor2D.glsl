//////////////////////////////////////////
//Vertex Shader
#version 450 core

//Vertex Layout
in layout (location = 0) vec2 Position;
in layout (location = 3) vec3 Color;

//Interface
out vec4 vs_Color;

//Parameters
uniform mat4 ProjectionMatrix = mat4(1);


void main()
{
    gl_Position = ProjectionMatrix * vec4(Position, 0.0, 1.0f);
    vs_Color    = vec4(Color, 1.0);
}

//////////////////////////////////////////
//Fragment Shader
#version 450 core

//Interface
in vec4 vs_Color;

out vec4 Color;

void main()
{
    Color = vs_Color;
}