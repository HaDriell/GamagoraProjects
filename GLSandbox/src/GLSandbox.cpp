#include <iostream>
#include <vector>

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
        positions.push_back(-1); positions.push_back(-1); positions.push_back(0);
        positions.push_back(+1); positions.push_back(-1); positions.push_back(0);
        positions.push_back(+0); positions.push_back(+1); positions.push_back(0);

        std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(positions.data(), positions.size());
        vertexBuffer->setLayout({
            { VertexAttributeType::Float3, "position" }
        });

        //Create Color
        std::vector<float> colors;
        colors.push_back(1); colors.push_back(1); colors.push_back(1); colors.push_back(1);
        colors.push_back(1); colors.push_back(1); colors.push_back(1); colors.push_back(1);
        colors.push_back(1); colors.push_back(1); colors.push_back(1); colors.push_back(1);

        std::shared_ptr<VertexBuffer> colorBuffer = std::make_shared<VertexBuffer>(colors.data(), colors.size());
        colorBuffer->setLayout({
            { VertexAttributeType::Float4, "color" }
        });
        
        //Create VertexArray
        std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>(); 
        vertexArray->addVertexBuffer(vertexBuffer);
        vertexArray->addVertexBuffer(colorBuffer);


        //Create Indices
        std::vector<unsigned int> indices;
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());


        // Render::Init();
        Render::ClearColor(vec4(0, 0, 0, 1)); // black
        while (!window->shouldClose())
        {
            //Poll & Swap Buffers
            window->update();

            //Render
            Render::Clear();

            //Done every loop
            shader.bind();
            Render::DrawIndexed(vertexArray, indexBuffer);
            Render::Debug();
        }
    }
    // std::cout << "Program Terminated. Press Enter to terminate" << std::endl;
    // std::cin.get();
}