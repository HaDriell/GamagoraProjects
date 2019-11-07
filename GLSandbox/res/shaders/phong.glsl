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
    vs_Normal   = normalize( (MVP * vec4(Normal, 0.0f)).xyz );
    vs_Color    = Color;
    vs_UV       = UV;

    gl_Position = MVP * vec4(Position, 1.0f);
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

//Custom data types
struct Light
{
    vec3    position;
    vec3    color;
    float   intensity;
};

struct Camera
{
    vec3 position;
    vec3 direction;
};

struct Material
{
    //Colors
    vec3        ambientColor;
    vec3        diffuseColor;
    vec3        specularColor;
    float       shininess;
    //Textures
    // sampler2D   ambientMap;
    // sampler2D   diffuseMap;
    // sampler2D   specularMap;
};

//Interface
in vec3 vs_Position;
in vec3 vs_Normal;
in vec3 vs_Color;
in vec2 vs_UV;


//Output
out vec4 fs_Color;

//Parameters
uniform Light       light;
uniform Camera      camera;
uniform Material    material;

//Ambient is constant, doesn't care about Light
vec3 AmbientColor()
{
    return material.ambientColor;
}

//Diffuse modulates the color depending on the angle between the light and the surface
vec3 DiffuseColor()
{
    vec3 to_light = normalize(light.position - vs_Position);
    float angle = max(dot(vs_Normal, to_light), 0.0);
    return angle * material.diffuseColor;
}

//Specular modulates the color depending on the angle between the Camera and the surface
vec3 SpecularColor()
{
    vec3 to_light = normalize(light.position - vs_Position);
    vec3 to_camera = normalize(camera.position - vs_Position);
    vec3 from_surface = reflect(-to_light, vs_Normal);
    float specularity = pow(max(dot(to_camera, from_surface), 0.0), material.shininess);
    return specularity * material.specularColor;
}

vec3 LightColor()
{
    return light.color * light.intensity;
}

void main()
{
    fs_Color = vec4( (AmbientColor() + DiffuseColor() + SpecularColor()) * LightColor(), 1.0);
}