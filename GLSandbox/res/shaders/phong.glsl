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
    vs_Position = (ModelMatrix * vec4(Position, 1.0f)).xyz;
    vs_Normal   = normalize( (ModelMatrix * vec4(Normal, 0.0f)) ).xyz ;
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
    vec3        ambient;
    vec3        diffuse;
    vec3        specular;
    float       shininess;
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

//Phong Lighting
void main()
{
    vec3 lightDirection = normalize(light.position - vs_Position);
    vec3 viewDirection = normalize(camera.position - vs_Position);
    vec3 reflectDirection = reflect(-lightDirection, vs_Normal);
    float lightDistance = distance(light.position, vs_Position);

    //Light Illumination
    vec3 illumination = light.color * light.intensity / (lightDistance * lightDistance);

    //Ambient Component
    vec3 ambient = material.ambient;

    //Diffuse Component
    float diffuseFactor = max(0.0, dot(vs_Normal, lightDirection));
    vec3 diffuse = diffuseFactor * material.diffuse;

    //Specular Component
    float specularFactor = pow(max(0.0, dot(reflectDirection, viewDirection)), material.shininess);
    vec3 specular = specularFactor * material.specular * 0.0;

    vec3 combination = (ambient + diffuse + specular) * illumination;
    fs_Color = vec4(combination, 1.0);
}