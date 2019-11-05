//////////////////////////////////////////
//Vertex Shader
#version 450 core

//Vertex Layout
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec3 Color;
layout (location = 3) in vec2 UV;

//Interface
out vec3 vs_Position;
out vec3 vs_Normal;
out vec3 vs_Color;

//Parameters
uniform mat4 u_ModelMatrix      = mat4(1);
uniform mat4 u_ViewMatrix       = mat4(1);
uniform mat4 u_ProjectionMatrix = mat4(1);

void main()
{
    gl_Position =  u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(Position, 1.0);
    vs_Position = (u_ModelMatrix * vec4(Position, 1.0)).xyz;
    vs_Normal   = normalize((u_ModelMatrix * vec4(Normal, 0.0)).xyz);
    vs_Color    = Color;
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

//Interface
in vec3 vs_Position;
in vec3 vs_Normal;
in vec3 vs_Color;

//Output
out vec4 Color;

void main()
{
    vec3 light_position = vec3(0, 0, 1);
    
    vec3 to_light = normalize(light_position - vs_Position);
    float angle = dot(to_light, vs_Normal);

    vec3 light_color = vec3(1, 1, 1);
    vec3 diffuse_color = angle * light_color;


    Color = vec4(vs_Normal * vs_Color, 1);
    Color = vec4(diffuse_color, 1);
}