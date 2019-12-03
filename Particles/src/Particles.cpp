#include <System.h>
#include <Graphics.h>
#include <Random.h>
#include <omp.h>

#define PROFILING
#include <Profiling.h>

template<typename T>
struct MemoryBuffer {
    Ref<VertexBuffer>   vbo;

private:
    std::vector<T>      data;
    bool                dirty = false;

public:
    MemoryBuffer(unsigned int capacity)
    {
        data.resize(capacity); // forced to resize at initialization.
    }

    T get(unsigned int index)
    {
        return data[index];
    }

    void set(unsigned int index, const T& value)
    {
        data[index] = value;
        dirty = true;
    }

    void sync()
    {
        if (dirty)
        {
            dirty = false;
            vbo->defineData(data);
        }
    }
};

struct SimpleParticleSystem : Layer
{
public:
    //Local Entropy
    Random                  random;
    //Render objects
    Ref<Shader>             shader;
    Ref<VertexArray>        vao;
    //Global Capacity indicator
    unsigned int            particleCount;
    //Member Buffers
    MemoryBuffer<vec2>      position;
    MemoryBuffer<vec2>      speed;
    MemoryBuffer<float>     mass;
    MemoryBuffer<vec3>      color;
    MemoryBuffer<float>     lifespan;

    SimpleParticleSystem(unsigned int particleCount) : 
        Layer("SimpleParticleSystem"), 
        particleCount(particleCount),
        position(particleCount),
        speed(particleCount),
        mass(particleCount),
        color(particleCount),
        lifespan(particleCount)
    { }

    void randomizeParticle(unsigned int index)
    {
        if (index >= particleCount)
        {
            LogError("Index {0} doesn't refer to an existing Particle !", index);
            return;
        }
        //Randomize every member
        position.set(index, vec2(random.Float(0, getWindow()->getWidth()), random.Float(0, getWindow()->getHeight())));
        speed.set(index,    vec2(random.Float(-2, 2), random.Float(-2, 2)));
        mass.set(index,     random.Float(3, 60));
        color.set(index,    vec3(random.Float(0, 1), random.Float(0, 1), random.Float(0, 1)));
        lifespan.set(index, random.Float(1, 10));
    }

    void onLoad() override
    {
        shader = Assets<Shader>::Find("FlatColor2D");
        vao = MakeRef<VertexArray>();

        //Initialize VBOs (Instanciate, Layout, Bind to VAO)
        position.vbo    = MakeRef<VertexBuffer>();
        position.vbo->defineLayout({{ VertexAttributeType::Float2, "Position" }});
        vao->addVertexBuffer(position.vbo);

        speed.vbo       = MakeRef<VertexBuffer>();
        speed.vbo->defineLayout({{ VertexAttributeType::Float2, "Speed" }});
        vao->addVertexBuffer(speed.vbo);

        mass.vbo        = MakeRef<VertexBuffer>();
        mass.vbo->defineLayout({{ VertexAttributeType::Float, "Mass" }});
        vao->addVertexBuffer(mass.vbo);

        color.vbo       = MakeRef<VertexBuffer>();
        color.vbo->defineLayout({{ VertexAttributeType::Float3, "Color" }});
        vao->addVertexBuffer(color.vbo);

        lifespan.vbo    = MakeRef<VertexBuffer>();
        lifespan.vbo->defineLayout({{ VertexAttributeType::Float, "Lifespan" }});
        vao->addVertexBuffer(lifespan.vbo);
    }

    void onUnload() override
    {
        shader.reset();
        vao.reset();

        position.vbo.reset();
        speed.vbo.reset();
        mass.vbo.reset();
        color.vbo.reset();
        lifespan.vbo.reset();
    }

    void onUpdate(float deltaTime) override
    {
        PROFILE_SCOPE("Update");
        #pragma omp parallel for
        for (unsigned int i = 0; i < particleCount; i++)
        {
            vec2 a = mass.get(i) * vec2(0, 9.81);
            vec2 s = speed.get(i);
            vec2 p = position.get(i);

            //Bounce on Window Borders
            if (p.x < 0 || p.x >= getWindow()->getWidth()) s.x = -s.x;
            if (p.y < 0 || p.y >= getWindow()->getHeight()) s.y = -s.y;            

            //Update data
            s += a * deltaTime;
            p += s * deltaTime;

            //Upload Updated data
            speed.set(i, s);
            position.set(i, p);
        }
    }

    void onRender() override
    {
        PROFILE_SCOPE("Render");
        {
            PROFILE_SCOPE("BufferUpload");
            //Upload dirty MemoryBuffers
            position.sync();
            speed.sync();
            mass.sync();
            color.sync();
            lifespan.sync();
        }

        //Draw Call
        shader->bind();
        shader->setUniform("ProjectionMatrix", mat4::Orthographic(0, getWindow()->getWidth(), 0, getWindow()->getHeight(), 1, -1));

        Render::PointSize(2.f);
        Render::DrawPoints(*vao, 0, particleCount);
    }
};


int main()
{
    WindowSettings settings;
    settings.glMajorVersion = 4;
    settings.glMinorVersion = 5;
    settings.multisampling  = 8;
    settings.glCoreProfile  = true;
    settings.vsync          = false;
    settings.resizeable     = false;
    settings.visible        = false;
    settings.debug          = true;

    Ref<Window> window = MakeRef<Window>(settings);

    ////////////////////////////////////////////////////////////////////////////////
    // Initialization Section Begin ////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    
    Ref<Shader> shader = MakeRef<Shader>();
    shader->compile("res/shaders/FlatColor2D.glsl"); if (!shader->isValid()) shader->debug(); Assets<Shader>::Add("FlatColor2D", shader);

    //SPS
    const int particleCount = 1e7;
    SimpleParticleSystem sps = SimpleParticleSystem(particleCount);
    window->pushLayer(&sps);
    for (unsigned int i = 0; i < particleCount; i++)
    {
        sps.randomizeParticle(i);
    }
    
    //Kikimeter
    window->pushLayer(new FPSLayer());
    ////////////////////////////////////////////////////////////////////////////////
    // Initialization Section End //////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    window->show();
    while (!window->shouldClose())
    {
        PROFILE_SCOPE("Frame");
        window->update();
        window->render();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Clean Up Section Begin //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // Clean Up Section End ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////


    window.reset();
}