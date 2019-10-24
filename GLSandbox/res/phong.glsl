//////////////////////////////////////////
//Vertex Shader
#version 450 core


//Vertex Layout
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec3 vertex_color;
layout (location = 3) in vec2 vertex_uv;

//Interface
out vec3 vs_position;
out vec3 vs_normal;
out vec3 vs_color;
out vec2 vs_uv;

//Parameters
uniform mat4 ModelMatrix      = mat4(1);
uniform mat4 ViewMatrix       = mat4(1);
uniform mat4 ProjectionMatrix = mat4(1);


void main()
{
    vs_position = (ModelMatrix * vec4(vertex_position, 1.0f)).xyz;
    vs_normal   = (ModelMatrix * vec4(vertex_normal, 0.0f)).xyz;
    vs_color    = vertex_color;
    vs_uv       = vertex_uv;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.0f);
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

//Interface
in vec3 vs_position;
in vec3 vs_normal;
in vec3 vs_color;
in vec2 vs_uv;


//Output
out vec4 fs_color;

//Parameters
uniform vec3    light_position;
uniform float   light_intensity;
uniform vec3    light_color;

void main()
{
    fs_color = vec4(0);

    //Diffuse Lighting
    vec3 htl = normalize(light_position - vs_position);
    float angle = clamp(dot(htl, vs_normal), 0, 1);
    vec3 diffuseColor = vs_color * (light_color * angle * light_intensity);
    fs_color += vec4(diffuseColor, 1);
}