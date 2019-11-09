#include "PhongMaterial.h"

#include "../Render.h"

////////////////////////////////////////////////////////////////////////////////
// Phong Material Shader ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

const RenderPipeline PhongPipeline = RenderPipeline(
        true, 
        true, 
        BlendingMode::Add,
        BlendingFactor::SrcAlpha, 
        BlendingFactor::OneMinusSrcAlpha, 
        true);

const std::string PhongShader = "PhongShader";

//TODO : consider using {fmt} to synchronize defines with the actual GLSL sources
#define MAX_POINT_LIGHTS        32
#define MAX_SPOT_LIGHTS         32
#define MAX_DIRECTIONAL_LIGHTS  32

const std::string PhongVertexShaderSource = R"(
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
        vs_Position = (ModelMatrix * vec4(Position, 1.0f)).xyz;
        vs_Normal   = (ModelMatrix * vec4(Normal,   0.0f)).xyz;
        vs_Color    = Color;
        vs_UV       = UV;

        gl_Position = MVP * vec4(Position, 1.0f);
    }
)";

const std::string PhongFragmentShaderSource = R"(
    #version 450 core

    struct PointLight
    {
        vec3 position;
        vec3 color;
        float intensity;
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
        float intensity = light.intensity / (distance(light.position, position) * distance(light.position, position));

        vec4 diffuse  = GetMaterialDiffuseColor() * diffuseFactor;
        vec4 specular = GetMaterialSpecularColor() * specularFactor;

        return (diffuse + specular) * vec4(light.color, 1.0);
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
)";

Ref<Shader> PhongMaterial::LoadShader()
{
    Ref<Shader> shader = Assets<Shader>::Find(PhongShader);
    if (!shader)
    {
        shader = MakeRef<Shader>();
        ShaderSources sources;
        sources[ShaderType::Vertex]     = PhongVertexShaderSource;
        sources[ShaderType::Fragment]   = PhongFragmentShaderSource;
        shader->compile(sources);
        if (!shader->isValid())
            shader->debug();
        Assets<Shader>::Add(PhongShader, shader);
    }
    return shader;
}

////////////////////////////////////////////////////////////////////////////////
// Phong Material Definition ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


PhongMaterial::PhongMaterial(const vec4& emissive, const vec4& ambient, const vec4& diffuse, const vec4& specular, float shininess) 
: Material(LoadShader()), emissive(emissive), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
{
}

void PhongMaterial::setupModelMatrix(const mat4& modelMatrix)
{
    shader->setUniform("ModelMatrix", modelMatrix);
}

void PhongMaterial::setupCamera(const Camera& camera)
{
    shader->setUniform("ViewMatrix", camera.getViewMatrix());
    shader->setUniform("ProjectionMatrix", camera.getProjectionMatrix());
    shader->setUniform("camera.position", camera.getPosition());
}

void PhongMaterial::setupLights(const LightSetup& setup)
{
    int pointLightCount = std::min(MAX_POINT_LIGHTS, (int) setup.pointLights.size());
    for (int i = 0; i < pointLightCount; i++)
    {
        shader->setUniform("pointLight[" + std::to_string(i) + "].position",    setup.pointLights[i].position);
        shader->setUniform("pointLight[" + std::to_string(i) + "].color",       setup.pointLights[i].color);
        shader->setUniform("pointLight[" + std::to_string(i) + "].intensity",   setup.pointLights[i].intensity);
    }
    shader->setUniform("pointLightCount", pointLightCount);
}

void PhongMaterial::setupProperties() 
{
    shader->setUniform("material.emissive", emissive);
    shader->setUniform("material.ambient", ambient);
    shader->setUniform("material.diffuse", diffuse);
    shader->setUniform("material.specular", specular);
    shader->setUniform("material.shininess", shininess);

    if (emissiveMap)
    {
        shader->setUniform("material.hasEmissiveMap", true);
        shader->setUniform("material.emissiveMap", 1);
        emissiveMap->bind(1);
    } else {
        shader->setUniform("material.hasEmissiveMap", false);
    }

    if (ambientMap)
    {
        shader->setUniform("material.hasAmbientMap", true);
        shader->setUniform("material.ambientMap", 2);
        ambientMap->bind(2);
    } else {
        shader->setUniform("material.hasAmbientMap", false);
    }

    if (diffuseMap)
    {
        shader->setUniform("material.hasDiffuseMap", true);
        shader->setUniform("material.diffuseMap", 3);
        diffuseMap->bind(3);
    } else {
        shader->setUniform("material.hasDiffuseMap", false);
    }

    if (specularMap)
    {
        shader->setUniform("material.hasSpecularMap", true);
        shader->setUniform("material.specularMap", 4);
        specularMap->bind(4);
    } else {
        shader->setUniform("material.hasSpecularMap", false);
    }

    Render::ConfigurePipeline(PhongPipeline);
}

void PhongMaterial::setEmissive(const vec4& color)
{
    this->emissive = color;
}

void PhongMaterial::setAmbient(const vec4& color)
{
    this->ambient = color;
}

void PhongMaterial::setDiffuse(const vec4& color)
{
    this->diffuse = color;
}

void PhongMaterial::setSpecular(const vec4& color)
{
    this->specular = color;
}

void PhongMaterial::setShininess(float shininess)
{
    this->shininess = shininess;
}

void PhongMaterial::setEmissiveMap(Ref<Texture2D> texture) 
{
    this->emissiveMap = texture; 
}

void PhongMaterial::setAmbientMap(Ref<Texture2D> texture) 
{
    this->ambientMap = texture; 
}

void PhongMaterial::setSpecularMap(Ref<Texture2D> texture) 
{
    this->specularMap = texture; 
}

void PhongMaterial::setDiffuseMap(Ref<Texture2D> texture)
{
    this->diffuseMap = texture;
}