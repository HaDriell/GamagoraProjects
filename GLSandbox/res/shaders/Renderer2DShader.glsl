//////////////////////////////////////////
//Vertex Shader
#version 450 core

//Vertex Layout
in vec2     Position;
in vec3     Color;
in vec2     UV;
in float    TextureID;

//Interface
out vec2    vs_Position;
out vec3    vs_Color;
out vec2    vs_UV;
out float   vs_TextureID;

//Parameters

void main()
{
    gl_Position = vec4(Position, 0.0, 1.0);
    //Setup Fragment interface
    vs_Position     = Position;
    vs_Color        = Color;
    vs_UV           = UV;
    vs_TextureID    = TextureID;
}


//////////////////////////////////////////
//Fragment Shader
#version 450 core

//Interface
in vec2     vs_Position;
in vec3     vs_Color;
in vec2     vs_UV;
in float    vs_TextureID;

//Fragment Layout
out vec4 fs_Color;

//Parameters
uniform sampler2D u_Texture[32];

void main()
{
    fs_Color = vec4(vs_Color, 1);

    if (vs_TextureID > 0.0)
    {
        int TID = int(vs_TextureID);
        fs_Color *= texture(u_Texture[TID], vs_UV);
    }
}