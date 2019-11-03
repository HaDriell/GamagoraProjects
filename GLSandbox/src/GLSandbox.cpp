#include <iostream>

#include <random>

#include <Common.h>
#include <System.h>
#include <Graphics.h>

struct Renderer2DLayer : public Layer
{
    Renderer2D renderer;
    Ref<Shader> shader;

    unsigned int hSubdivisions = 400;
    unsigned int vSubdivisions = 225;

    std::default_random_engine random;

    Renderer2DLayer() : Layer("Renderer2D Layer") {}

    void onLoad()
    {
        shader = std::make_shared<Shader>();
        if (!shader->compile("res/shaders/Renderer2DShader.glsl"))
            shader->debug();
    }

    void onRender(float deltaTime)
    {
        float width = getWindow()->getWidth();
        float height = getWindow()->getHeight();

        std::uniform_real_distribution<float> Float(0.0f, 1.0f);

        renderer.begin(shader, mat4::Orthographic(0, width, 0, height, 1, -1));

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
                renderer.fillRect(x, y, hSize, vSize, color);
            }
        }
        renderer.end();
    }

    void onUnload() {}
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

    MeshLayer(const std::string& shaderFilename, const std::string& meshFilename, const mat4& modelMatrix) 
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
        pipeline.srcBlending = BlendingFactor::SrcAlpha;
        pipeline.dstBlending = BlendingFactor::OneMinusSrcAlpha;
    }

    void onRender(float deltaTime)
    {
        rotation += deltaTime;
        shader->bind();
        shader->setUniform("u_ModelMatrix", mat4::RotationY(rotationSpeed * rotation) * ModelMatrix);
        Render::ConfigurePipeline(pipeline);
        Render::DrawIndexed(*mesh->getVertexArray(), *mesh->getIndexBuffer());  
    }
};

int main()
{
    WindowSettings settings;
    settings.glMajorVersion = 4;
    settings.glMinorVersion = 5;
    settings.resizeable     = false;

    //Create the Window
    Ref<Window> window = std::make_shared<Window>(settings);
    // window->setVSync(false);
    window->pushLayer(new FPSLayer(2));
    // window->pushLayer(new Renderer2DLayer());
    window->pushLayer(new MeshLayer("res/shaders/example.glsl", "res/meshes/otter.obj", mat4::Translation(-0.5f, 0, 0)));
    window->pushLayer(new MeshLayer("res/shaders/example.glsl", "res/meshes/otter.obj", mat4::Translation(+0.5f, 0, 0)));

    //MainLoop
    while (!window->shouldClose())
    {
        Render::Debug();
        window->update();
        Render::Clear();
        window->render();
    }
    window.reset(); // deletes the Window

    // std::cout << "Press Enter to Exit" << std::endl;
    // std::cin.get();
}