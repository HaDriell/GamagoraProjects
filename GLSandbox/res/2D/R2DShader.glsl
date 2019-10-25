//////////////////////////////////////////
//Vertex Shader
#version 450 core


//Vertex Layout
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_uv;

//Interface
out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_uv;

//Parameters
uniform mat4 u_ProjectionMatrix = mat4(1);
uniform mat4 u_ModelMatrix      = mat4(1);


void main()
{
    vs_position = vertex_position;
    vs_color    = vertex_color;
    vs_uv       = vertex_uv;

    gl_Position = u_ProjectionMatrix * u_ModelMatrix * vec4(vertex_position, 1.0f);
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

//Interface
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_uv;


//Output
out vec4 fs_color;

//Parameters
uniform sampler2D u_Texture;

void main()
{
    vec4 texture_color = texture(u_Texture, vs_uv);
    fs_color = vec4(vs_color, 1) * texture_color;
}