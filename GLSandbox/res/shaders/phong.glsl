//////////////////////////////////////////
//Vertex Shader
#version 450 core


//Vertex Layout
in vec3 Position;
in vec3 Normal;
in vec3 Color;
in vec2 UV;

//Interface
out vec3 vs_Position;
out vec3 vs_Normal;
out vec3 vs_Color;
out vec2 vs_UV;

//Parameters
uniform mat4 ModelMatrix      = mat4(1);
uniform mat4 ViewMatrix       = mat4(1);
uniform mat4 ProjectionMatrix = mat4(1);


void main()
{
    mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    vs_Position = (MVP * vec4(Position, 1.0f)).xyz;
    vs_Normal   = (MVP * vec4(Normal, 0.0f)).xyz;
    vs_Color    = Color;
    vs_UV       = UV;

    gl_Position = MVP * vec4(vertex_position, 1.0f);
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

//Interface
in vec3 vs_Position;
in vec3 vs_Normal;
in vec3 vs_Color;
in vec2 vs_UV;


//Output
out vec4 fs_Color;

//Parameters
uniform vec3    light_position;
uniform float   light_intensity;
uniform vec3    light_color;

void main()
{
    fs_Color = vec4(0);

    //Diffuse Lighting
    vec3 htl = normalize(light_position - vs_position);
    float angle = clamp(dot(htl, vs_normal), 0, 1);
    vec3 diffuseColor = vs_color * (light_color * angle * light_intensity);
    fs_Color += vec4(diffuseColor, 1);
}