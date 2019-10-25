#include <iostream>
#include <vector>

#include <random>

#include "Timer.h"
#include "Events.h"
#include "Graphics.h"

int main()
{
    std::default_random_engine random;
    std::uniform_real_distribution<float> Float(0, 1);

    //Scoped main, because Window has to be deleted
    {
        WindowSettings settings;
        settings.width = 800;
        settings.height = 800;
        std::shared_ptr<Window> window = std::make_shared<Window>(settings);
        window->setVSync(false);

        //High level rendering example
        Renderer2D renderer(settings.width, settings.height);

        //Low level rendering example
        Shader  shader;
        Mesh    mesh;
        Texture texture;

        //Initialize Shader & Mesh
        {
            //Compile Shader from file
            ShaderSources sources;
            // if (!loadGLSLFile("res/example.glsl", sources)) return 1;
            if (!loadGLSLFile("res/phong.glsl", sources)) return 1;
            shader.compile(sources);
            if (!shader.isValid())  
                shader.debug();

            //Load Mesh from file
            // loadOBJFile("res/cube.obj", mesh);
            loadOBJFile("res/bunny.obj", mesh);

            //Load Texture from file
            TextureSettings textureSettings;
            Image image;
            loadImageFile("res/metal.png", image);
            texture.defineImage(image);
            texture.defineSettings(textureSettings);
        }


        Timer age;
        Timer frameTimer;
        Render::Init();
        Render::ClearColor(vec4(0, 0, 0, 1));
        while (!window->shouldClose())
        {
            //Window management & screen clean-up
            window->update();
            Render::Debug();
            Render::Clear();

            //Low Level render loop
            {
                shader.bind();
                shader.setUniform("ModelMatrix", mat4::Scale(3, 3, 3) * mat4::RotationY(60 * age.elapsed()));
                shader.setUniform("light_position", vec3(0.0f, 1.0f, 0.0f));
                shader.setUniform("light_intensity", 1.0f);
                shader.setUniform("light_color", vec3(1.0f));
                Render::DrawIndexed(*mesh.getVertexArray(), *mesh.getIndexBuffer());
            }

            //High level Rendering Example
            {
                renderer.push(mat4::RotationZ(60 * age.elapsed()));
                renderer.push(mat4::Translation(100, 50, 0));
                renderer.fillRect(0, 0, 300, 100);
                renderer.drawImage(0, 0, 100, 300, texture);
                renderer.pop();
                renderer.pop();
            }

            //Make the Renderer kneel
            {
                random.seed(1);
                int count = 10;
                float quadWidth = window->getWidth() / (float) count;
                float quadHeight = window->getHeight() / (float) count;

                for (int dx = 0; dx < count; dx++)
                {
                    for (int dy = 0; dy < count; dy++)
                    {
                        vec3 color = vec3(Float(random), Float(random), Float(random));
                        renderer.fillRect(dx * quadWidth, dy * quadHeight, quadWidth, quadHeight, color);
                    }
                }

                renderer.pop();
                renderer.pop();
            }

            //Update timer
            int fps = (int) (1.f / frameTimer.elapsed());
            frameTimer.reset();
            window->setTitle("GL Window " + std::to_string(fps) + " FPS");
        }
    }
    std::cout << "Press Enter to Exit" << std::endl;
    std::cin.get();
}