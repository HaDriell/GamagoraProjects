#include <iostream>
#include <vector>

#include "Timer.h"
#include "Events.h"
#include "Graphics.h"

int main()
{
    //Scoped main, because Window has to be deleted
    {
        WindowSettings settings;
        settings.width = 800;
        settings.height = 800;
        std::shared_ptr<Window> window = std::make_shared<Window>(settings);
        //Compile Shader

        ShaderSources sources;
        // if (!loadGLSLFile("res/example.glsl", sources)) return 1;
        if (!loadGLSLFile("res/phong.glsl", sources)) return 1;

        Shader shader = Shader();
        shader.compile(sources);
        // shader.debug();

        //Load Mesh
        Mesh mesh = Mesh();
        // loadOBJFile("res/cube.obj", mesh);
        loadOBJFile("res/bunny.obj", mesh);

        Timer age;
        Render::Init();
        Render::ClearColor(vec4(0, 0, 0, 1)); // black
        while (!window->shouldClose())
        {
            //Poll & Swap Buffers
            window->update();
            Render::Debug();
            float deltaTime = age.elapsed();

            //Begin scene draw
            Render::Clear();

            shader.bind();
            transform t;
            // t.set_translation(std::sin(dt) * 0.3f, 0, 0);
            t.set_scaling(3, 3, 3);
            t.set_rotation(0, 60 * deltaTime, 0);
            shader.setUniform("ModelMatrix", t.get_matrix());

            //Render objects
            Render::DrawIndexed(mesh.getVertexArray(), mesh.getIndexBuffer());
        }
    }
    std::cout << "Press Enter to Exit" << std::endl;
    std::cin.get();
}