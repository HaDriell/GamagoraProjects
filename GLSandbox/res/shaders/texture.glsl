//////////////////////////////////////////
//Vertex Shader
#version 450 core

//Vertex Layout
in layout (location = 0) vec3 Position;
in layout (location = 3) vec2 UV;

//Interface
out vec2 vs_UV;

//Parameters
uniform mat4 ModelMatrix      = mat4(1);
uniform mat4 ViewMatrix       = mat4(1);
uniform mat4 ProjectionMatrix = mat4(1);

void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0f);
    vs_UV = UV;
}

//////////////////////////////////////////
//Fragment Shader
#version 450 core

//Inteface
in vec2 vs_UV;

//Parameter
uniform sampler2D u_Texture;

//Output
out vec4 fs_Color;

void main()
{
    fs_Color = texture(u_Texture, vs_UV);
}