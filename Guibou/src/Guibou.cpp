#include <iostream>
#include <random>

#include "maths.h"

#include <Common.h>
#include <Graphics.h>
#include <System.h>

#include "Timer.h"

struct Vertex {
    vec3 position;
    vec3 color;
};

const BufferLayout VertexLayout = {
    { VertexAttributeType::Float3, "position" },
    { VertexAttributeType::Float3, "color"    },
};


int main()
{
    WindowSettings settings;
    settings.glMajorVersion = 4;
    settings.glMinorVersion = 5;
    Window window(settings);

    std::shared_ptr<Shader> shader      = std::make_shared<Shader>();
    std::shared_ptr<VertexArray> vao    = std::make_shared<VertexArray>();
    std::shared_ptr<VertexBuffer> vbo   = std::make_shared<VertexBuffer>();
    std::shared_ptr<IndexBuffer> ibo    = std::make_shared<IndexBuffer>();



    //Compile Shader
    {
        ShaderSources sources;
        loadGLSLFile("res/basic.glsl", sources);
        shader->compile(sources);
        if (!shader->isValid())
            shader->debug();
    }

    //Upload Data
    {
        std::default_random_engine random;
        std::uniform_real_distribution<float> distPosition(-1.0f, 1.0f);
        std::uniform_real_distribution<float> distSpeed(-1.0f, 1.0f);
        std::uniform_real_distribution<float> distColor(0.0f, 1.0f);

        unsigned int count = 1000;
        std::vector<Vertex> vertices;
        vertices.reserve(count * 4);
        for (unsigned int i = 0; i < count; i++)
        {
            vec3 origin = vec3(distPosition(random), distPosition(random), 0);
            float size = 0.01f;
            Vertex vertex;

            vertex.position = origin + vec3(0, 0, 0);
            vertex.color    = vec3(distColor(random), distColor(random), distColor(random));
            vertices.push_back(vertex);

            vertex.position = origin + vec3(size, 0, 0);
            vertex.color    = vec3(distColor(random), distColor(random), distColor(random));
            vertices.push_back(vertex);

            vertex.position = origin + vec3(size, size, 0);
            vertex.color    = vec3(distColor(random), distColor(random), distColor(random));
            vertices.push_back(vertex);

            vertex.position = origin + vec3(0, size, 0);
            vertex.color    = vec3(distColor(random), distColor(random), distColor(random));
            vertices.push_back(vertex);
        }

        std::vector<unsigned int> indices;
        indices.reserve(count * 6);
        for (unsigned int i = 0; i < count; i++)
        {
            unsigned int offset = (i * 6);
            //Triangle 1
            indices.push_back(offset + 0);
            indices.push_back(offset + 1);
            indices.push_back(offset + 2);
            //Triangle 2
            indices.push_back(offset + 2);
            indices.push_back(offset + 3);
            indices.push_back(offset + 0);
        }
        ibo->defineData(indices);
        
        vbo->defineData(vertices);
        vbo->defineLayout(VertexLayout);
        vao->addVertexBuffer(vbo);
    }


    Timer age;
    // glViewport(0, 0, window.getWidth(), window.getHeight());
    Render::Init();
    Render::ClearColor(vec4(0, 0, 0, 1));
    while(!window.shouldClose())
    {
        window.update();
        Render::Clear();

        shader->bind();
        Render::DrawIndexed(*vao, *ibo);
    }
}