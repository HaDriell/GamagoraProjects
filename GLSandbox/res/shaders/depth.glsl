//////////////////////////////////////////
//Vertex Shader
#version 450 core

//Vertex Layout
in layout (location = 0) vec3 Position;

//Parameters
uniform mat4 ModelMatrix      = mat4(1);
uniform mat4 ViewMatrix       = mat4(1);
uniform mat4 ProjectionMatrix = mat4(1);


void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0f);
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

//Output
out vec4 fs_Color;

void main()
{
    fs_Color = vec4(gl_FragCoord.z); //grayscale
}