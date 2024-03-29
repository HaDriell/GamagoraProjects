#include <iostream>

#include <random>

#include <Common.h>
#include <File.h>
#include <System.h>
#include <Graphics.h>

#include "Profiling.h"

//Represents a single Renderable object
struct Actor
{
    //Render util
    Transform       transform;
    Ref<Mesh>       mesh;

    //Phong properties
    vec4            emissive;
    vec4            ambient;
    vec4            diffuse;
    vec4            specular;
    float           shininess;
    Ref<Texture2D>  emissiveMap;
    Ref<Texture2D>  ambientMap;
    Ref<Texture2D>  diffuseMap;
    Ref<Texture2D>  specularMap;
};

struct Light
{
    vec3            position;
    //vec3            direction;
    vec3            color;
    float           intensity;

    mat4            projection;
    Framebuffer     framebuffer;

    Light(unsigned int w, unsigned int h) : 
        projection(mat4::PerspectiveFov(90, w, h, 0.1, 1000)),
        framebuffer(Framebuffer(w, h)) {}
};

//Represents the whole Stage
struct Stage : public Layer
{
    Ref<Camera>             mainCamera;
    std::vector<Ref<Actor>> actors;
    std::vector<Ref<Light>> lights;

    void onRender() override;
};

void loadShaders();
void loadMeshes();
void loadTextures();


Ref<Light> spawnLight(const vec3& position, const vec3& color, float intensity);
Ref<Actor> spawnActor(const std::string& meshName, const vec3& position = vec3(0), const vec3& rotation = vec3(0), float scale = 1);
void drawTexture(float viewportWidth, float viewportHeight, float x, float y, float width, float height, Ref<Texture2D> texture);

int main()
{
    WindowSettings settings;
    settings.glMajorVersion = 4;
    settings.glMinorVersion = 5;
    settings.multisampling  = 8;    // Will probably be replaced by a custom anti-aliased FBO
    settings.glCoreProfile  = true;
    settings.vsync          = true;
    settings.resizeable     = false;
    settings.visible        = false; //hide during the mesh loading
    settings.debug          = true;


    //Create the Window
    Ref<Window> window = MakeRef<Window>(settings);

    //Push the Window FPS counter
    window->pushLayer(new FPSLayer());

    //Push the Camera Controller to the Window
    CameraController cameraController = CameraController(mat4::PerspectiveFov(80.0f, settings.width, settings.height, 0.1f, 10000.0f), vec3(0, 0, 25));
    cameraController.setSpeed(3);
    window->pushLayer(&cameraController);
    Stage stage;
    stage.mainCamera = cameraController.getCamera();
    window->pushLayer(&stage);

    //Load before showing window
    loadShaders();
    loadTextures();
    loadMeshes();
    window->show();

    //push Actors

    //Floor 
    stage.actors.push_back(spawnActor("cube", vec3(0, -1000, 0), vec3(0), 1900));

    //Cube
    stage.actors.push_back(spawnActor("cube", vec3(0), vec3(0), 10)); 
    stage.actors.back()->ambientMap = Assets<Texture2D>::Find("crate0_diffuse");
    stage.actors.back()->diffuseMap = Assets<Texture2D>::Find("crate0_diffuse");

    //push Lights
    stage.lights.push_back(spawnLight(vec3(500, 20, 0), vec3(1), 1e2));

    //MainLoop
    std::default_random_engine random;
    std::uniform_real_distribution<float> Float(0, 1);
    bool lightSpawned = false;
    Timer time;
    while (!window->shouldClose())
    {
        Render::CheckError();
        window->update();

        if (window->inputs().isKeyPressed(KEY_R))
        {
            stage.lights.front()->position = stage.mainCamera->getPosition();
        }

        if (window->inputs().isKeyPressed(KEY_F))
        {
            if (!lightSpawned)
            {
                stage.lights.push_back(spawnLight(stage.mainCamera->getPosition(), vec3(Float(random), Float(random), Float(random)), 1e3));
            }
            lightSpawned = true;
        } else {
            lightSpawned = false;
        }

        window->render();
    }
    window.reset(); // deletes the Window
}

Ref<Light> spawnLight(const vec3& position, const vec3& color, float intensity)
{
    Ref<Light> light = MakeRef<Light>(1024, 1024);
    light->position     = position;
    light->color        = color;
    light->intensity    = intensity;
    light->framebuffer.getColorBuffer()->defineEmptyImage(TextureInternalFormat::R32F, light->framebuffer.getWidth(), light->framebuffer.getHeight());

    light->framebuffer.bind();
    if (!light->framebuffer.isComplete())
        light->framebuffer.debug();
    light->framebuffer.unbind();

    return light;
}

Ref<Actor> spawnActor(const std::string& meshName, const vec3& position, const vec3& rotation, float scale)
{
    Ref<Actor> actor = MakeRef<Actor>();

    actor->transform.setPosition(position);
    actor->transform.setRotation(rotation);
    actor->transform.setScale(scale, scale, scale);
    
    actor->ambient      = vec4(0.1, 0.1, 0.1, 1);
    actor->diffuse      = vec4(0.9, 0.9, 0.9, 1);
    actor->emissive     = vec4(0);
    actor->specular     = vec4(0.3, 0.3, 0.3, 1);
    actor->shininess    = 300;

    actor->mesh = Assets<Mesh>::Find(meshName);
    if (!actor->mesh)   LogError("Mesh '{0}' not found !", meshName);

    return actor;
}

void loadTextures()
{
    Ref<Texture2D> texture;
    texture = MakeRef<Texture2D>(); texture->loadImage("res/textures/metal.png");                   Assets<Texture2D>::Add("metal", texture);

    texture = MakeRef<Texture2D>(); texture->loadImage("res/textures/crate0/crate0_bump.png");      Assets<Texture2D>::Add("crate0_bump", texture);
    texture = MakeRef<Texture2D>(); texture->loadImage("res/textures/crate0/crate0_diffuse.png");   Assets<Texture2D>::Add("crate0_diffuse", texture);
    texture = MakeRef<Texture2D>(); texture->loadImage("res/textures/crate0/crate0_normal.png");    Assets<Texture2D>::Add("crate0_normal", texture);

    texture = MakeRef<Texture2D>(); texture->loadImage("res/textures/crate1/crate1_bump.png");      Assets<Texture2D>::Add("crate1_bump", texture);
    texture = MakeRef<Texture2D>(); texture->loadImage("res/textures/crate1/crate1_diffuse.png");   Assets<Texture2D>::Add("crate1_diffuse", texture);
    texture = MakeRef<Texture2D>(); texture->loadImage("res/textures/crate1/crate1_normal.png");    Assets<Texture2D>::Add("crate1_normal", texture);

    texture = MakeRef<Texture2D>(); texture->loadImage("res/textures/crate2/crate2_bump.png");      Assets<Texture2D>::Add("crate2_bump", texture);
    texture = MakeRef<Texture2D>(); texture->loadImage("res/textures/crate2/crate2_diffuse.png");   Assets<Texture2D>::Add("crate2_diffuse", texture);
    texture = MakeRef<Texture2D>(); texture->loadImage("res/textures/crate2/crate2_normal.png");    Assets<Texture2D>::Add("crate2_normal", texture);
}

void loadShaders()
{
    Ref<Shader> shader;
    shader = MakeRef<Shader>(); shader->compile("res/shaders/phong.glsl");                  Assets<Shader>::Add("Phong", shader);   if (!shader->isValid()) shader->debug();
    shader = MakeRef<Shader>(); shader->compile("res/shaders/phong-ambient-pass.glsl");     Assets<Shader>::Add("Phong-Ambient", shader); if (!shader->isValid()) shader->debug();
    shader = MakeRef<Shader>(); shader->compile("res/shaders/phong-lighting-pass.glsl");    Assets<Shader>::Add("Phong-Lighting", shader); if (!shader->isValid()) shader->debug();

    shader = MakeRef<Shader>(); shader->compile("res/shaders/depth.glsl");      Assets<Shader>::Add("Depth", shader); if (!shader->isValid()) shader->debug();
    shader = MakeRef<Shader>(); shader->compile("res/shaders/texture.glsl");    Assets<Shader>::Add("Texture", shader); if (!shader->isValid()) shader->debug();
}

void loadMeshes()
{
    Ref<Mesh> mesh;
    mesh = MakeRef<Mesh>(); mesh->loadOBJ("res/meshes/bunny.obj");          Assets<Mesh>::Add("bunny", mesh);
    mesh = MakeRef<Mesh>(); mesh->loadOBJ("res/meshes/cube.obj");           Assets<Mesh>::Add("cube", mesh);
    mesh = MakeRef<Mesh>(); mesh->loadOBJ("res/meshes/otter.obj");          Assets<Mesh>::Add("otter", mesh);
    // mesh = MakeRef<Mesh>(); mesh->loadOBJ("res/meshes/tree.obj");           Assets<Mesh>::Add("tree", mesh);
    // mesh = MakeRef<Mesh>(); mesh->loadOBJ("res/meshes/queen_of_sea.obj");   Assets<Mesh>::Add("queen_of_sea", mesh);
}

void Stage::onRender()
{
    if (actors.empty()) return;
    if (!mainCamera) {
        LogWarning("No Main Camera !");
        return;
    }
    
    //Shadow mapping pass
    {
        RenderPipeline pipeline;
        pipeline.blending = true;
        pipeline.blendingMode = BlendingMode::Add;
        pipeline.depthTesting = true;
        pipeline.faceCulling = true;
        Render::ConfigurePipeline(pipeline);

        Ref<Shader> shader = Assets<Shader>::Find("Depth");
        shader->bind();

        for (Ref<Light> light : lights)
        {
            //Fixed View straight down
            mat4 view = mat4::Translation(-light->position) * mat4::RotationX(-90);
            //Setup Light's "camera"
            shader->setUniform("ViewMatrix", view);
            shader->setUniform("ProjectionMatrix", light->projection);
            //Draw each Actor inside the Light Framebuffer
            light->framebuffer.bind();
            Render::Clear();
            Render::SetViewport(0, 0, light->framebuffer.getWidth(), light->framebuffer.getHeight());
            for (Ref<Actor> actor : actors)
            {
                shader->setUniform("ModelMatrix",    actor->transform.getMatrix());
                Render::DrawIndexed(*actor->mesh->getVertexArray(), *actor->mesh->getIndexBuffer());
            }
            light->framebuffer.unbind();
        }
    }
    
    //Phong pass
    {
        Render::SetViewport(0, 0, getWindow()->getWidth(), getWindow()->getHeight());
        //Ambient Pass
        {
            Ref<Shader> ambientShader = Assets<Shader>::Find("Phong-Ambient");
            ambientShader->bind();

            RenderPipeline ambientPipeline;
            ambientPipeline.blending = true;
            ambientPipeline.blendingMode = BlendingMode::Add;
            ambientPipeline.depthTesting = true;
            ambientPipeline.faceCulling = true;
            Render::ConfigurePipeline(ambientPipeline);

            //Camera
            ambientShader->setUniform("ViewMatrix",       mainCamera->getViewMatrix());
            ambientShader->setUniform("ProjectionMatrix", mainCamera->getProjectionMatrix());
            for (Ref<Actor> actor : actors)
            {
                //ModelMatrix
                ambientShader->setUniform("ModelMatrix", actor->transform.getMatrix());
                //Emissive
                ambientShader->setUniform("material.emissive", actor->emissive);
                ambientShader->setUniform("material.hasEmissiveMap", (bool) actor->emissiveMap);
                if (actor->emissiveMap) {   actor->emissiveMap->bind(1);    ambientShader->setUniform("material.emissiveMap", 1); }
                //Ambient
                ambientShader->setUniform("material.ambient", actor->ambient);
                ambientShader->setUniform("material.hasAmbientMap", (bool) actor->ambientMap);
                if (actor->ambientMap) {    actor->ambientMap->bind(2);     ambientShader->setUniform("material.ambientMap", 2); }

                //Draw Call
                Render::DrawIndexed(*actor->mesh->getVertexArray(), *actor->mesh->getIndexBuffer());
            }
        }

        //Lighting Passes (one per light)
        {
            Ref<Shader> lightingShader = Assets<Shader>::Find("Phong-Lighting");
            lightingShader->bind();

            RenderPipeline lightingPipeline;

            lightingPipeline.faceCulling    = true;

            lightingPipeline.depthWriting   = false;
            lightingPipeline.depthTesting   = true;
            lightingPipeline.depthTestMode  = DepthTestMode::Equal;

            lightingPipeline.blending       = true;
            lightingPipeline.blendingMode   = BlendingMode::Add;
            lightingPipeline.srcFactor      = BlendingFactor::One;
            lightingPipeline.dstFactor      = BlendingFactor::One; 
            Render::ConfigurePipeline(lightingPipeline);


            //Camera
            lightingShader->setUniform("ViewMatrix",       mainCamera->getViewMatrix());
            lightingShader->setUniform("ProjectionMatrix", mainCamera->getProjectionMatrix());
            for (Ref<Actor> actor : actors)
            {
                //ModelMatrix
                lightingShader->setUniform("ModelMatrix", actor->transform.getMatrix());
                //Diffuse
                lightingShader->setUniform("material.diffuse", actor->diffuse);
                lightingShader->setUniform("material.hasDiffuseMap", (bool) actor->diffuseMap);
                if (actor->diffuseMap) {   actor->diffuseMap->bind(1);    lightingShader->setUniform("material.diffuseMap", 1); }
                //Specular
                lightingShader->setUniform("material.hasSpecularMap", (bool) actor->specularMap);
                if (actor->specularMap) {   actor->specularMap->bind(2);    lightingShader->setUniform("material.specularMap", 2); }
                lightingShader->setUniform("material.specular", actor->specular);
                lightingShader->setUniform("material.shininess", actor->shininess);

                for (Ref<Light> light : lights)
                {
                    lightingShader->setUniform("light.intensity", light->intensity);
                    lightingShader->setUniform("light.position",  light->position);
                    lightingShader->setUniform("light.color",     light->color);
                    //Fixed View straight down
                    mat4 view = mat4::Translation(-light->position) * mat4::RotationX(-90);
                    //Shadow Mapping
                    lightingShader->setUniform("light.projection", light->projection);
                    lightingShader->setUniform("light.view",       view);
                    light->framebuffer.getColorBuffer()->bind(3);
                    lightingShader->setUniform("light.shadowMap", 3);
                    //Draw Call
                    Render::DrawIndexed(*actor->mesh->getVertexArray(), *actor->mesh->getIndexBuffer());
                }
            }
        }
    }

    
    //Phong pass in FBO
    Framebuffer phongFBO = Framebuffer(getWindow()->getWidth(), getWindow()->getHeight());
    /*
    {
        //Prepare FBO
        phongFBO.bind();
        Render::SetViewport(0, 0, phongFBO.getWidth(), phongFBO.getHeight());
        Render::Clear();


        //Ambient Pass
        {
            Ref<Shader> ambientShader = Assets<Shader>::Find("Phong-Ambient");
            ambientShader->bind();

            RenderPipeline ambientPipeline;
            ambientPipeline.blending = true;
            ambientPipeline.blendingMode = BlendingMode::Add;
            ambientPipeline.depthTesting = true;
            ambientPipeline.faceCulling = true;
            Render::ConfigurePipeline(ambientPipeline);

            //Camera
            ambientShader->setUniform("ViewMatrix",       mainCamera->getViewMatrix());
            ambientShader->setUniform("ProjectionMatrix", mainCamera->getProjectionMatrix());
            for (Ref<Actor> actor : actors)
            {
                //ModelMatrix
                ambientShader->setUniform("ModelMatrix", actor->transform.getMatrix());
                //Emissive
                ambientShader->setUniform("material.emissive", actor->emissive);
                ambientShader->setUniform("material.hasEmissiveMap", (bool) actor->emissiveMap);
                if (actor->emissiveMap) {   actor->emissiveMap->bind(1);    ambientShader->setUniform("material.emissiveMap", 1); }
                //Ambient
                ambientShader->setUniform("material.ambient", actor->ambient);
                ambientShader->setUniform("material.hasAmbientMap", (bool) actor->ambientMap);
                if (actor->ambientMap) {    actor->ambientMap->bind(2);     ambientShader->setUniform("material.ambientMap", 2); }

                //Draw Call
                Render::DrawIndexed(*actor->mesh->getVertexArray(), *actor->mesh->getIndexBuffer());
            }
        }

        //Lighting Passes (one per light)
        {
            Ref<Shader> lightingShader = Assets<Shader>::Find("Phong-Lighting");
            lightingShader->bind();

            RenderPipeline lightingPipeline;

            lightingPipeline.faceCulling    = true;

            lightingPipeline.depthWriting   = false;
            lightingPipeline.depthTesting   = true;
            lightingPipeline.depthTestMode  = DepthTestMode::Equal;

            lightingPipeline.blending       = true;
            lightingPipeline.blendingMode   = BlendingMode::Add;
            lightingPipeline.srcFactor      = BlendingFactor::One;
            lightingPipeline.dstFactor      = BlendingFactor::One; 
            Render::ConfigurePipeline(lightingPipeline);


            //Camera
            lightingShader->setUniform("ViewMatrix",       mainCamera->getViewMatrix());
            lightingShader->setUniform("ProjectionMatrix", mainCamera->getProjectionMatrix());
            for (Ref<Actor> actor : actors)
            {
                //ModelMatrix
                lightingShader->setUniform("ModelMatrix", actor->transform.getMatrix());
                //Diffuse
                lightingShader->setUniform("material.diffuse", actor->diffuse);
                lightingShader->setUniform("material.hasDiffuseMap", (bool) actor->diffuseMap);
                if (actor->diffuseMap) {   actor->diffuseMap->bind(1);    lightingShader->setUniform("material.diffuseMap", 1); }
                //Specular
                lightingShader->setUniform("material.hasSpecularMap", (bool) actor->specularMap);
                if (actor->specularMap) {   actor->specularMap->bind(2);    lightingShader->setUniform("material.specularMap", 2); }
                lightingShader->setUniform("material.specular", actor->specular);
                lightingShader->setUniform("material.shininess", actor->shininess);

                for (Ref<Light> light : lights)
                {
                    lightingShader->setUniform("light.intensity", light->intensity);
                    lightingShader->setUniform("light.position",  light->position);
                    lightingShader->setUniform("light.color",     light->color);
                    //Fixed View straight down
                    mat4 view = mat4::Translation(-light->position) * mat4::RotationX(-90);
                    //Shadow Mapping
                    lightingShader->setUniform("light.projection", light->projection);
                    lightingShader->setUniform("light.view",       view);
                    light->framebuffer.getColorBuffer()->bind(3);
                    lightingShader->setUniform("light.shadowMap", 3);
                    //Draw Call
                    Render::DrawIndexed(*actor->mesh->getVertexArray(), *actor->mesh->getIndexBuffer());
                }
            }
        }

        phongFBO.unbind();
    }
    //*/

    float width  = getWindow()->getWidth();
    float height = getWindow()->getHeight();
    Render::SetViewport(0, 0, width, height);

    //Left side is Ortho Color & Depth
    // drawTexture(width, height, 0, 0, width, height, phongFBO.getColorBuffer());
    for (Ref<Light> light : lights)
        drawTexture(width, height, 0, 0, 400, 225, light->framebuffer.getColorBuffer());
    drawTexture(width, height, 0, 230, 400, 225, phongFBO.getColorBuffer());
}

void drawTexture(float screenWidth, float screenHeight, float x, float y, float width, float height, Ref<Texture2D> texture)
{
    Ref<VertexArray>    vertexArray  = MakeRef<VertexArray>();
    Ref<VertexBuffer>   vertexBuffer = MakeRef<VertexBuffer>();
    Ref<IndexBuffer>    indexBuffer  = MakeRef<IndexBuffer>();
    
    float l = x;
    float r = x + width;
    float t = y;
    float b = y + height;

    //Upload Quad
    std::vector<MeshVertex> vertices;
    MeshVertex vertex;
    vertex.position = vec3(l, t, 0);
    vertex.uv       = vec2(0, 1);
    vertices.push_back(vertex);
    
    vertex.position = vec3(r, t, 0);
    vertex.uv       = vec2(1, 1);
    vertices.push_back(vertex);
    
    vertex.position = vec3(r, b, 0);
    vertex.uv       = vec2(1, 0);
    vertices.push_back(vertex);
    
    vertex.position = vec3(l, b, 0);
    vertex.uv       = vec2(0, 0);
    vertices.push_back(vertex);
    vertexBuffer->defineData(vertices);

    //Setup Vertex Layout
    vertexBuffer->defineLayout(MeshVertexLayout);
    vertexArray->addVertexBuffer(vertexBuffer);

    //Upload Indices
    std::vector<unsigned int> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);
    indexBuffer->defineData(indices);


    RenderPipeline pipeline;
    pipeline.blending = false;
    pipeline.depthTesting = false;
    pipeline.faceCulling = false;
    Render::ConfigurePipeline(pipeline);

    Ref<Shader> shader = Assets<Shader>::Find("Texture");
    shader->bind();
    shader->setUniform("ProjectionMatrix", mat4::Orthographic(0, screenWidth, 0, screenHeight, 1, -1));

    //Texture to draw
    texture->bind(1);
    shader->setUniform("u_Texture", 1);
    Render::DrawIndexed(*vertexArray, *indexBuffer);
}