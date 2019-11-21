//////////////////////////////////////////
//Vertex Shader
#version 450 core

//Vertex Layout
in layout (location = 0) vec3 Position;
in layout (location = 1) vec3 Normal;
in layout (location = 2) vec3 Color;
in layout (location = 3) vec2 UV;

//Interface

out vec3 vs_Position;
out vec3 vs_Normal;
out vec2 vs_UV;

//Parameters
uniform mat4 ModelMatrix      = mat4(1);
uniform mat4 ViewMatrix       = mat4(1);
uniform mat4 ProjectionMatrix = mat4(1);


void main()
{
    vs_Position = vec3(ModelMatrix * vec4(Position, 1.0));
    vs_Normal   = normalize(vec3(ModelMatrix * vec4(Normal, 0.0)));
    vs_UV       = UV;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0f);
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

struct PointLight
{
    //Phyiscal data
    vec3        position;
    vec3        color;
    float       intensity;
    //Shadow mapping
    mat4        projection;
    mat4        view;
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
uniform Material        material;
uniform Camera          camera;
uniform PointLight[16]  pointLight;
uniform sampler2D[16]   shadowMap;
uniform int             pointLightCount;

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

void main()
{
    fs_Color = GetMaterialAmbientColor() + GetMaterialEmissiveColor();

    //Compute PointLights
    for (int i = 0; i < pointLightCount; i++)
    {
        PointLight light = pointLight[i];
        vec3 lightDirection = normalize(light.position - vs_Position);
        vec3 reflectDirection = reflect(-lightDirection, vs_Normal);
        vec3 viewDirection = normalize(camera.position - vs_Position);
        float lightDistance = distance(light.position, vs_Position);

        // Sample Shadow map
        vec4 sm_Position = light.projection * light.view * vec4(vs_Position, 1.0);
        vec2 sm_UV = (sm_Position.xy / sm_Position.w) * 0.5 + 0.5;
        // We're in light BEFORE shadowDistance (RED32F)
        float inLight = 0.0;
        // Force inShadow if we're out of the texture coords
        if (sm_UV.x < 0.0 || sm_UV.x > 1.0 || sm_UV.y < 0.0 || sm_UV.y > 1.0) {
            inLight = 0.0;
        } else {
            inLight = 1.0 - step(texture(shadowMap[i], sm_UV).r + 0.1, lightDistance);
        }

        float diffuseFactor = max(0.0, dot(vs_Normal, lightDirection));
        float specularFactor = pow(max(0.0, dot(viewDirection, reflectDirection)), material.shininess);
        float intensity = inLight * light.intensity / (lightDistance * lightDistance);

        vec4 diffuse  = GetMaterialDiffuseColor() * diffuseFactor;
        vec4 specular = GetMaterialSpecularColor() * specularFactor;

        fs_Color += (diffuse + specular) * vec4(light.color, 1.0) * intensity;
    }
}