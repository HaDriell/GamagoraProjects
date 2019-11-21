//////////////////////////////////////////
//Vertex Shader
#version 450 core

//Vertex Layout
in layout (location = 0) vec3 Position;
in layout (location = 2) vec3 Color;
in layout (location = 3) vec2 UV;

//Interface
out vec3 vs_Color;
out vec2 vs_UV;

//Parameters
uniform mat4 ModelMatrix      = mat4(1);
uniform mat4 ViewMatrix       = mat4(1);
uniform mat4 ProjectionMatrix = mat4(1);


void main()
{
    vs_Color    = Color;
    vs_UV       = UV;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0f);
}

//////////////////////////////////////////
//Fragment Shader
#version 450 core

struct Material
{
    vec4        emissive;
    bool        hasEmissiveMap;
    sampler2D   emissiveMap;

    vec4        ambient;
    bool        hasAmbientMap;
    sampler2D   ambientMap;
};

//Interface
in vec3 vs_Color;
in vec2 vs_UV;

//Output
out vec4 fs_Color;

//Parameters
uniform Material material;


vec4 GetMaterialEmissiveColor()
{
    vec4 color = material.emissive;
    if (material.hasEmissiveMap)
    {
        color *= texture(material.emissiveMap, vs_UV);
    }
    return color;
}

vec4 GetMaterialAmbientColor()
{
    vec4 color = material.ambient;
    if (material.hasAmbientMap)
    {
        color *= texture(material.ambientMap, vs_UV);
    }
    return color;
}

void main()
{
    fs_Color = GetMaterialAmbientColor() + GetMaterialEmissiveColor();
}