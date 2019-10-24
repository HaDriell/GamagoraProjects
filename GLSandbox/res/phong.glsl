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


void main()
{
    // //Simulate a single point light
    // vec3 light_position = vec3(0.0, 1.0, 0.0);
    // float light_intensity = 1;

    // //Compute angle between light & surface
    // vec3 htl = normalize(light_position - vs_position);
    // float diffuse = clamp(dot(htl, vs_normal), 0, 1);

    // vec3 diffuseColor = vs_color * diffuse;

    fs_color = vec4(vs_normal, 1.0);
}