#include <iostream>
#include <vector>

#include "Timer.h"
#include "Events.h"
#include "Graphics.h"

int main()
{
    //Scoped main, because Window has to be deleted
    {
        std::shared_ptr<Window> window = std::make_shared<Window>();
        //Compile Shader        
        Shader shader = Shader();
        shader.compileFile("res/example.glsl");

        //Load Mesh
        Mesh mesh = Mesh();
        loadOBJFile("res/cube.obj", mesh);
        loadOBJFile("res/bunny.obj", mesh);

        Timer age;
        Render::Init();
        Render::ClearColor(vec4(0, 0, 0, 1)); // black
        while (!window->shouldClose())
        {
            //Poll & Swap Buffers
            window->update();
            Render::Debug();
            float dt = age.elapsed();

            //Begin scene draw
            Render::Clear();

            //Setup Shader 
            shader.bind();
            transform t;
            t.set_translation(std::sin(dt) * 0.3f, 0, 0);
            t.set_rotation(dt * 15, dt * 5, dt * 10);
            shader.setUniform("model", t.get_matrix());

            //Render objects
            Render::DrawIndexed(mesh.getVertexArray(), mesh.getIndexBuffer());

            //Setup another Shader...
                //Render other objects...

            //End scene draw
        }
    }
}