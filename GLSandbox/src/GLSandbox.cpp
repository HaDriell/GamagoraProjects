#include <iostream>

#include <random>

#include <Common.h>
#include <File.h>
#include <System.h>
#include <Graphics.h>

struct Renderer2DLayer : public Layer
{
    Scope<Renderer2D> renderer = nullptr;

    unsigned int hSubdivisions = 400;
    unsigned int vSubdivisions = 225;

    std::default_random_engine random;

    Renderer2DLayer() : Layer("Renderer2D Layer") {}

    
    void onLoad()
    {
        renderer = MakeScope<Renderer2D>();
    }

    void onUnload()
    {
        renderer.release();
    }

    void onRender(float deltaTime)
    {
        float width = getWindow()->getWidth();
        float height = getWindow()->getHeight();

        std::uniform_real_distribution<float> Float(0.0f, 1.0f);

        renderer->begin(mat4::Orthographic(0, width, 0, height, 1, -1));

        float hSize = width / (float) hSubdivisions;
        float vSize = height / (float) vSubdivisions;
        for (int i = 0; i < hSubdivisions; i++)
        {
            float x = i * hSize;
            for (int j = 0; j < vSubdivisions; j++)
            {
                float y = j * vSize;

                //Draw quad
                vec3 color;
                color.x = Float(random);
                color.y = Float(random);
                color.z = Float(random);
                renderer->fillRect(x, y, hSize, vSize, color);
            }
        }
        renderer->end();
    }

};

struct MeshLayer : public Layer
{
    RenderPipeline pipeline;
    Ref<Shader> shader;
    Ref<Mesh> mesh;

    std::string shaderFilename;
    std::string meshFilename;
    mat4 ModelMatrix;
    float rotationSpeed = 60;
    float rotation;

    MeshLayer(const std::string& shaderFilename, const std::string& meshFilename, const mat4& modelMatrix = mat4()) 
    : shaderFilename(shaderFilename), meshFilename(meshFilename), ModelMatrix(modelMatrix)
    {
    }

    void onLoad()
    {
        shader = Assets<Shader>::Find(shaderFilename);
        if (!shader)
        {
            shader = MakeRef<Shader>();
            shader->compile(shaderFilename);
            if (!shader->isValid())
                shader->debug();
            Assets<Shader>::Add(shaderFilename, shader);
        }

        mesh = Assets<Mesh>::Find(meshFilename);
        if (!mesh)
        {
            mesh = MakeRef<Mesh>();
            mesh->loadOBJ(meshFilename);
            Assets<Mesh>::Add(meshFilename, mesh);
        }

        pipeline.depthTesting = true;
        pipeline.blending = true;
        pipeline.blendingMode = BlendingMode::Add;
        pipeline.srcFactor = BlendingFactor::SrcAlpha;
        pipeline.dstFactor = BlendingFactor::OneMinusSrcAlpha;
    }

    void onRender(float deltaTime)
    {
        mat4 projection;
        vec3 translation;

        projection = mat4::PerspectiveFov(90, 16, 9, 0.1f, 1000.0f); // width & height aren't correct anyway
        translation = vec3(0, 0, -2.5);

        rotation += deltaTime;
        shader->bind();
        shader->setUniform("u_ModelMatrix", mat4::RotationY(rotationSpeed * rotation) * ModelMatrix);
        shader->setUniform("u_ViewMatrix", mat4::Translation(translation));
        shader->setUniform("u_ProjectionMatrix", projection);
        Render::ConfigurePipeline(pipeline);
        Render::DrawIndexed(*mesh->getVertexArray(), *mesh->getIndexBuffer());  
    }
};

int main()
{
    WindowSettings settings;
    settings.glMajorVersion = 4;
    settings.glMinorVersion = 5;
    settings.glCoreProfile  = true;
    settings.resizeable     = false;


    //Create the Window
    Ref<Window> window = std::make_shared<Window>(settings);
    // window->setVSync(false);
    window->pushLayer(new FPSLayer(2));
    // window->pushLayer(new Renderer2DLayer());
    // window->pushLayer(new MeshLayer("res/shaders/example.glsl", "res/meshes/cube.obj"));
    // window->pushLayer(new MeshLayer("res/shaders/example.glsl", "res/meshes/queen_of_sea.obj"));

    //Push the Camera Controller to the Window
    CameraController cameraController = CameraController(Camera(mat4::PerspectiveFov(90.0f, settings.width, settings.height, 0.1f, 1000.0f), vec3(0, 0, - 5)));
    window->pushLayer(&cameraController);


    MTLLoader mtlLoader;
    mtlLoader.load("res/materials/tree.mtl");
    MTLMaterial material = mtlLoader.getMaterials()[0];

    MTLTexture  diffuseMap = material.diffuseMap;
    Texture2D diffuseTex;
    diffuseTex.loadImage(diffuseMap.path);

    Shader shader;
    shader.compile("res/shaders/phong.glsl");
    if (!shader.isValid())
        shader.debug();

    Mesh mesh;
    mesh.loadOBJ("res/meshes/tree.obj");

    RenderPipeline pipeline;
    pipeline.blending = true;
    pipeline.depthTesting = true;

    Timer t;
    //MainLoop
    while (!window->shouldClose())
    {
        Camera camera = cameraController.getCamera();


        Render::Debug();
        window->update();
        Render::Clear();

        //PHONG TEST !
        shader.bind();

        //Material setup
        shader.setUniform("material.ambient", material.ambientColor);
        shader.setUniform("material.diffuse", material.diffuseColor);
        shader.setUniform("material.specular", material.specularColor);

        //Light setup (Positioned on Camera)
        shader.setUniform("light.position", vec3(0, 5, 0));
        shader.setUniform("light.color", vec3(1));
        shader.setUniform("light.intensity", 10.0f);

        //Camera setup
        shader.setUniform("camera.position", camera.getPosition());

        for (int i = -5; i <= 5; i++)
        {
            //Transforms
            shader.setUniform("ProjectionMatrix", camera.getProjectionMatrix());
            shader.setUniform("ViewMatrix", camera.getViewMatrix());
            shader.setUniform("ModelMatrix", mat4::RotationY(30.0 * t.elapsed()) * mat4::Translation(i, 0, 0));
            //Draw call
            Render::ConfigurePipeline(pipeline);
            Render::DrawIndexed(*mesh.getVertexArray(), *mesh.getIndexBuffer());
        }

        window->render();
    }
    window.reset(); // deletes the Window

    // std::cout << "Press Enter to Exit" << std::endl;
    // std::cin.get();
}