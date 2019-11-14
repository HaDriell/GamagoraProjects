//////////////////////////////////////////
//Vertex Shader
#version 450 core

//Vertex Layout
in layout (location = 0) vec3 Position;
in layout (location = 1) vec3 Normal;
in layout (location = 2) vec3 Color;
in layout (location = 3) vec2 UV;

//Interface

out vec4 vs_PositionFromLightPerspective;
out vec3 vs_Position;
out vec3 vs_Normal;
out vec2 vs_UV;

//Parameters
uniform mat4 LightMatrix      = mat4(1);
uniform mat4 ModelMatrix      = mat4(1);
uniform mat4 ViewMatrix       = mat4(1);
uniform mat4 ProjectionMatrix = mat4(1);


void main()
{
    vs_Position = vec3(ModelMatrix * vec4(Position, 1.0));
    vs_Normal   = vec3(ModelMatrix * vec4(Normal,   0.0));
    vs_UV       = UV;

    vs_PositionFromLightPerspective = LightMatrix * vec4(Position, 1.0);
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0f);
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

struct PointLight
{
    vec3        position;
    vec3        color;
    float       intensity;
    vec2        shadowTexelSize;
    sampler2D   shadowMap;
};

struct Camera
{
    vec3 position;
};

struct Material
{
    vec4        emissive;
    bool        hasEmissiveMap;
    sampler2D   emissiveMap;

    vec4        ambient;
    bool        hasAmbientMap;
    sampler2D   ambientMap;

    vec4        diffuse;
    bool        hasDiffuseMap;
    sampler2D   diffuseMap;

    vec4        specular;
    bool        hasSpecularMap;
    sampler2D   specularMap;

    float       shininess;
};


//Interface
in vec4 vs_PositionFromLightPerspective;
in vec3 vs_Position;
in vec3 vs_Normal;
in vec2 vs_UV;

//Output
out vec4 fs_Color;

//Parameters
uniform Material    material;
uniform Camera      camera;
uniform PointLight  pointLight[32];
uniform int         pointLightCount;

float SampleShadowMap(sampler2D shadowMap, vec4 initialShadowMapCoords)
{
    vec3 shadowMapCoords = (initialShadowMapCoords.xyz / initialShadowMapCoords.w) * vec3(0.5) + vec3(0.5);
    return step(initialShadowMapCoords.w, texture(shadowMap, shadowMapCoords.xy).r);
}

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

vec4 GetMaterialDiffuseColor()
{
    vec4 color = material.diffuse;
    if (material.hasDiffuseMap)
    {
        color *= texture(material.diffuseMap, vs_UV);
    }
    return color;
}

vec4 GetMaterialSpecularColor()
{
    vec4 color = material.specular;
    if (material.hasSpecularMap)
    {
        color *= texture(material.specularMap, vs_UV);
    }
    return color;
}

vec4 ComputePointLight(PointLight light, vec3 position, vec3 normal)
{
    vec3 lightDirection = normalize(light.position - position);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 viewDirection = normalize(camera.position - position);

    float diffuseFactor = max(0.0, dot(normal, lightDirection));
    float specularFactor = pow(max(0.0, dot(viewDirection, reflectDirection)), material.shininess);
    float intensity = light.intensity / (distance(light.position, position) 
                    * distance(light.position, position))
                    * SampleShadowMap(light.shadowMap, vs_PositionFromLightPerspective);

    vec4 diffuse  = GetMaterialDiffuseColor() * diffuseFactor;
    vec4 specular = GetMaterialSpecularColor() * specularFactor;

    return (diffuse + specular) * vec4(light.color, 1.0) * intensity;
}

void main()
{
    fs_Color = GetMaterialAmbientColor() + GetMaterialEmissiveColor();

    //Compute PointLights
    for (int i = 0; i < pointLightCount; i++)
    {
        fs_Color += ComputePointLight(pointLight[i], vs_Position, normalize(vs_Normal));
    }
}