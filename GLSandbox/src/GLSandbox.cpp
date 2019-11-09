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
    settings.multisampling  = 8;    // Will probably be replaced by a custom anti-aliased FBO
    settings.glCoreProfile  = true;
    settings.resizeable     = false;
    settings.visible        = false; //hide during the mesh loading


    //Create the Window
    Ref<Window> window = MakeRef<Window>(settings);
    window->pushLayer(new FPSLayer(1));

    //Push the Camera Controller to the Window
    CameraController cameraController = CameraController(mat4::PerspectiveFov(90.0f, settings.width, settings.height, 0.1f, 1000.0f), vec3(0, 0, 5));
    cameraController.setSpeed(3);
    window->pushLayer(&cameraController);
    Scene scene = Scene(cameraController.getCamera());
    window->pushLayer(&scene);

    //Load Textures2D here
    {
        LogDebug("File : {0}", VFS::Exists("res/textures/metal.png"));
        LogDebug("File : {0}", VFS::Exists("res/textures/crate0/crate0_diffuse.png"));
        Ref<Texture2D> loader;
        loader = MakeRef<Texture2D>();  loader->loadImage("res/textures/metal.png");    Assets<Texture2D>::Add("metal", loader);

        loader = MakeRef<Texture2D>();  loader->loadImage("res/textures/crate0/crate0_bump.png");      Assets<Texture2D>::Add("crate0_bump", loader);
        loader = MakeRef<Texture2D>();  loader->loadImage("res/textures/crate0/crate0_diffuse.png");   Assets<Texture2D>::Add("crate0_diffuse", loader);
        loader = MakeRef<Texture2D>();  loader->loadImage("res/textures/crate0/crate0_normal.png");    Assets<Texture2D>::Add("crate0_normal", loader);

        loader = MakeRef<Texture2D>();  loader->loadImage("res/textures/crate1/crate1_bump.png");      Assets<Texture2D>::Add("crate1_bump", loader);
        loader = MakeRef<Texture2D>();  loader->loadImage("res/textures/crate1/crate1_diffuse.png");   Assets<Texture2D>::Add("crate1_diffuse", loader);
        loader = MakeRef<Texture2D>();  loader->loadImage("res/textures/crate1/crate1_normal.png");    Assets<Texture2D>::Add("crate1_normal", loader);

        loader = MakeRef<Texture2D>();  loader->loadImage("res/textures/crate2/crate2_bump.png");      Assets<Texture2D>::Add("crate2_bump", loader);
        loader = MakeRef<Texture2D>();  loader->loadImage("res/textures/crate2/crate2_diffuse.png");   Assets<Texture2D>::Add("crate2_diffuse", loader);
        loader = MakeRef<Texture2D>();  loader->loadImage("res/textures/crate2/crate2_normal.png");    Assets<Texture2D>::Add("crate2_normal", loader);
    }

    //Load Meshes here
    {
        Ref<Mesh> loader;
        loader = MakeRef<Mesh>();   loader->loadOBJ("res/meshes/bunny.obj");        Assets<Mesh>::Add("bunny", loader);
        loader = MakeRef<Mesh>();   loader->loadOBJ("res/meshes/cube.obj");         Assets<Mesh>::Add("cube", loader);
        loader = MakeRef<Mesh>();   loader->loadOBJ("res/meshes/queen_of_sea.obj"); Assets<Mesh>::Add("queen_of_sea", loader);
        loader = MakeRef<Mesh>();   loader->loadOBJ("res/meshes/otter.obj");        Assets<Mesh>::Add("otter", loader);
        loader = MakeRef<Mesh>();   loader->loadOBJ("res/meshes/tree.obj");         Assets<Mesh>::Add("tree", loader);
    }
    window->show();

    //Generate a bunch of nodes
    {
        std::default_random_engine random(0);
        std::uniform_int_distribution<int>      IMesh(0, 4);
        std::uniform_real_distribution<float>   FPos(-300, +300);
        std::uniform_real_distribution<float>   FColor(0.01, 0.9);
        std::uniform_real_distribution<float>   FShininess(0, 256);
        std::uniform_real_distribution<float>   FScale(10, 20);
        for (int i = 0; i < 25; i++)
        {
            Ref<Node> node = MakeRef<Node>();
            //Setup Mesh
            int meshType = IMesh(random);
            switch (meshType)
            {
                case 0: node->setMesh(Assets<Mesh>::Find("bunny")); break;
                case 1: node->setMesh(Assets<Mesh>::Find("cube")); break;
                case 2: node->setMesh(Assets<Mesh>::Find("queen_of_sea")); break;
                case 3: node->setMesh(Assets<Mesh>::Find("otter")); break;
                case 4: node->setMesh(Assets<Mesh>::Find("tree")); break;
            }

            vec4 ambient = vec4(FColor(random), FColor(random), FColor(random), 1);
            vec4 diffuse = vec4(FColor(random), FColor(random), FColor(random), 1);
            vec4 specular = vec4(FColor(random), FColor(random), FColor(random), 1);
            float shininess = FShininess(random);

            Ref<PhongMaterial> material = MakeRef<PhongMaterial>(vec4(0), ambient, diffuse, specular, shininess);
            if (meshType == 1) // cube type
            {
                std::uniform_int_distribution<int> ICrate(0, 2);
                switch (ICrate(random))
                {
                case 0: material->setDiffuseMap(Assets<Texture2D>::Find("crate0_diffuse")); break;
                case 1: material->setDiffuseMap(Assets<Texture2D>::Find("crate1_diffuse")); break;
                case 2: material->setDiffuseMap(Assets<Texture2D>::Find("crate2_diffuse")); break;
                }
            }
            node->setMaterial(material);

            //Random Transform
            vec3 position = vec3(FPos(random), FPos(random), FPos(random));
            float scale   = FScale(random);

            Transform transform;
            transform.setPosition(position);
            transform.setScale(vec3(scale));
            node->setTransform(transform);


            //Push to the Scene
            scene.addNode(node);
        }

        {
            LightSetup randomSetup;
            for (int i = 0; i < 10; i++)
            {
                vec3 position = vec3(FPos(random), FPos(random), FPos(random));
                vec3 color    = vec3(FColor(random), FColor(random), FColor(random));
                randomSetup.pointLights.push_back(PointLight(position, color, 1));
            }
            scene.setLights(randomSetup);
        }
    }

    Timer t;
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