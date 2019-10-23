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
        shader.compileFile("res/helloTriangle.glsl");

        //Create Geometry
        std::vector<float> positions;
        positions.push_back(-1); positions.push_back(-1); positions.push_back(-0.3f);
        positions.push_back(+1); positions.push_back(-1); positions.push_back(-0.3f);
        positions.push_back(+0); positions.push_back(+1); positions.push_back(-0.3f);

        std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
        vertexBuffer->defineData(positions);
        vertexBuffer->defineLayout({{ VertexAttributeType::Float3, "position" }});

        //Create Color
        std::vector<float> colors;
        colors.push_back(1); colors.push_back(0); colors.push_back(0); colors.push_back(1);
        colors.push_back(0); colors.push_back(1); colors.push_back(0); colors.push_back(1);
        colors.push_back(0); colors.push_back(0); colors.push_back(1); colors.push_back(1);

        std::shared_ptr<VertexBuffer> colorBuffer = std::make_shared<VertexBuffer>();
        colorBuffer->defineData(colors);
        colorBuffer->defineLayout({{ VertexAttributeType::Float4, "color" }});
        
        //Create VertexArray
        std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>(); 
        vertexArray->addVertexBuffer(vertexBuffer);
        vertexArray->addVertexBuffer(colorBuffer);

        //Create Indices
        std::vector<unsigned int> indices;
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>();
        indexBuffer->defineData(indices);

        Mesh mesh = Mesh();
        mesh.loadFromOBJ("res/cube.obj");

        Timer age;
        age.reset();

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
            Render::DrawIndexed(vertexArray, indexBuffer);
            Render::DrawIndexed(mesh.getVertexArray(), mesh.getIndexBuffer());

            //Setup Shader...
            //Render objects...

            //End scene draw
        }
    }
}