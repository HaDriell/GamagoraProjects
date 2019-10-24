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

        //2D Renderer (high level abstraction)
        Renderer2D renderer(settings.width, settings.height);

        Timer age;
        Render::Init();
        Render::ClearColor(vec4(0, 0, 0, 1)); // black
        while (!window->shouldClose())
        {
            //Poll & Swap Buffers
            window->update();
            Render::Debug();

            //Begin scene draw
            Render::Clear();

            //Low level Rendering Example
            shader.bind();
            transform t;
            t.set_scaling(3, 3, 3);
            t.set_rotation(0, 60 * age.elapsed(), 0);
            shader.setUniform("ModelMatrix", t.get_matrix());
            shader.setUniform("light_position", vec3(0.0f, 1.0f, 0.0f));
            shader.setUniform("light_intensity", 1.0f);
            shader.setUniform("light_color", vec3(1.0f));
            Render::DrawIndexed(*mesh.getVertexArray(), *mesh.getIndexBuffer());

            //High level Rendering Example
            renderer.push(mat4::RotationZ(60 * age.elapsed()));
            renderer.push(mat4::Translation(100, 50, 0));
            renderer.fillRect(0, 0, 300, 100);
            renderer.pop();
            renderer.pop();
        }
    }
    std::cout << "Press Enter to Exit" << std::endl;
    std::cin.get();
}