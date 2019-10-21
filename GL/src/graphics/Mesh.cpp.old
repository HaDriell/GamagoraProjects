#include "Mesh.h"

#include <glad/glad.h>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "Render.h"


//Useful Parsing functions
bool __read_unsigned_int(std::istringstream& stream, unsigned int& value);
bool __read_vec3(std::istringstream& stream, vec3& v);
bool __read_vec2(std::istringstream& stream, vec2& v);
bool __read_vertex(std::istringstream& stream, unsigned int& v, unsigned int& vn, unsigned int& vt);
std::vector<std::string> split(const std::string& str, char c = ' ');

Mesh::Mesh() : vertexArray(0), indexBuffer(0) {}

Mesh::~Mesh()
{
    destroy();
}

void Mesh::destroy()
{
    //Delete VAO
    if (vertexArray)
    {
        glDeleteVertexArrays(1, &vertexArray);
        vertexArray = 0;
    }

    //Delete VBOs
    if (!vertexBuffers.empty())
    {
        for (unsigned int vertexBuffer : vertexBuffers)
        {
            glDeleteBuffers(1, &vertexBuffer);
        }
        vertexBuffers.clear();
    }

    //Delete IBO
    if (indexBuffer)
    {
        glDeleteBuffers(1, &indexBuffer);
        indexBuffer = 0;
    }

    elementCount = 0;
}


void Mesh::draw(const Shader& shader)
{
    shader.bind();
    drawTriangles(vertexArray, indexBuffer, elementCount);
}

void Mesh::loadFromMemory(std::vector<unsigned int>& indices, const std::vector<vec3>& vertices, const std::vector<vec3>& normals, const std::vector<vec2>& uvs)
{
    //Check for optional stuff coherence
    if (!normals.empty() && vertices.size() > normals.size())
    {
        std::cout << "Failed to load Mesh from memory : only " << normals.size() << " normals for " << vertices.size() << " vertices." << std::endl;
        return;
    }
    if (!uvs.empty() && vertices.size() > uvs.size())
    {
        std::cout << "Failed to load Mesh from memory : only " << uvs.size() << " UVMappings for " << vertices.size() << " vertices." << std::endl;
        return;
    }
    
    //Check for indices coherence
    unsigned int maxIndexValue = *std::max_element(indices.begin(), indices.end());
    if (maxIndexValue >= vertices.size())
    {
        std::cout << "Failed to load Mesh from memory : An index targets the vertex " << maxIndexValue << " but there is only " << vertices.size() << " of them." << std::endl;
        return;
    }

    //Destroy any previously existing buffer in VRAM
    destroy();

    //Upload indices
    {
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        elementCount = indices.size();
    }

    //Create the vertex array
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    //Always (I could make a function but I'm lazy)
    {
        //Load Vertices
        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        //Register to VAO
        glEnableVertexAttribArray(vertexBuffers.size());
        glVertexAttribPointer(vertexBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, 0);
        vertexBuffers.push_back(vbo);
    }

    if (!normals.empty())
    {
        //Load Normals
        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), normals.data(), GL_STATIC_DRAW);

        //Register to VAO
        glEnableVertexAttribArray(vertexBuffers.size());
        glVertexAttribPointer(vertexBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, 0);
        vertexBuffers.push_back(vbo);
    }

    if (!uvs.empty())
    {
        //Load UVMappings
        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * 2 * sizeof(float), uvs.data(), GL_STATIC_DRAW); // warning : vec2, not vec3

        //Register to VAO
        glEnableVertexAttribArray(vertexBuffers.size());
        glVertexAttribPointer(vertexBuffers.size(), 2, GL_FLOAT, GL_FALSE, 0, 0);
        vertexBuffers.push_back(vbo);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void Mesh::loadFromOBJ(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cout << "Failed to load Mesh from file : " << path << " not found !" << std::endl;
        return;
    }

    //Face Vertex
    struct Vertex
    {
        unsigned int v = 0;
        unsigned int vn = 0;
        unsigned int vt = 0;
    };

    std::vector<vec3> obj_positions;
    std::vector<vec3> obj_normals;
    std::vector<vec2> obj_uvs;
    std::vector<Vertex> obj_indices;

    std::string line;


    while (std::getline(file, line))
    {
        std::istringstream stream(line);
        std::string header;

        //Read header
        if (!std::getline(stream, header, ' '))
            continue;

        //Ignore comment
        if (header == "#")
            continue;
        
        //Reading a vertex
        if (header == "v")
        {
            vec3 data;
            if (__read_vec3(stream, data))
                obj_positions.push_back(data);
        }

        //Reading a vertex normal
        if (header == "vn")
        {
            vec3 data;
            if (__read_vec3(stream, data))
                obj_normals.push_back(data);
        }

        //Reading a vertex UVMapping
        if (header == "vt")
        {
            vec2 data;
            if (__read_vec2(stream, data))
                obj_uvs.push_back(data);
        }

        if (header == "f")
        {
            Vertex vertex0, vertex1, vertex2;
            if (__read_vertex(stream, vertex0.v, vertex0.vn, vertex0.vt)
            &&  __read_vertex(stream, vertex1.v, vertex1.vn, vertex1.vt)
            &&  __read_vertex(stream, vertex2.v, vertex2.vn, vertex2.vt))
            {
                obj_indices.push_back(vertex0);
                obj_indices.push_back(vertex1);
                obj_indices.push_back(vertex2);
            } else {
                std::cout << "Error loading face data" << std::endl;
            }
        }
    }

    std::vector<unsigned int> indices;
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec2> uvs;

    unsigned int vertexIndex = 0;
    for (Vertex vertex : obj_indices)
    {
        indices.push_back(vertexIndex++);
        vertices.push_back(obj_positions[vertex.v - 1]);
        if (vertex.vn) 
        {
            normals.push_back(obj_normals[vertex.vn - 1]);
        }
        if (vertex.vt)
        {
            uvs.push_back(obj_uvs[vertex.vt - 1]);
        }
    }
    loadFromMemory(indices, vertices, normals, uvs);
}


//Useful Parsing functions
bool __read_unsigned_int(std::istringstream& stream, unsigned int& value)
{
    stream >> std::ws; // consume whitespaces

    if (!(stream >> value))
        return false;
    return true;
}

bool __read_vec3(std::istringstream& stream, vec3& v)
{
    stream >> std::ws; // consume whitespaces

    if (!(stream >> v.x >> v.y >> v.z))
        return false;
    return true;
}

bool __read_vec2(std::istringstream& stream, vec2& v)
{
    stream >> std::ws; // consume whitespaces

    if (!(stream >> v.x >> v.y))
        return false;
    return true;
}

bool __read_vertex(std::istringstream& stream, unsigned int& v, unsigned int& vn, unsigned int& vt)
{
    stream >> std::ws; // consume whitespaces

    std::string data;
    std::getline(stream, data, ' ');
    if (stream.fail())
        return false;

    //Vertex data only
    if (data.find("/") == std::string::npos)
    {
        v = std::stoi(data);
        vn = 0;
        vt = 0;
        return true;
    }

    //Vertex + Normal data only
    if (data.find("//") != std::string::npos)
    {
        std::istringstream dstream(data);
        dstream >> v;
        dstream.ignore(2); // skip '//'
        dstream >> vn;
        vt = 0;
        return !dstream.fail();
    }

    //Vertex + UV data only
    if (data.find("/") == data.rfind("/"))
    {
        std::istringstream dstream(data);
        dstream >> v;
        dstream.ignore(1); // skip '/'
        dstream >> vt;
        return !dstream.fail();
    }
    else //Vertex + UV + Normal
    {
        std::istringstream dstream(data);
        dstream >> v;
        dstream.ignore(1); // skip '/'
        dstream >> vt;
        dstream.ignore(1); // skip '/'
        dstream >> vn;
        return !dstream.fail();
    }
    return true;
}

std::vector<std::string> split(const std::string& input, char c)
{
    std::vector<std::string> result;

    std::istringstream stream(input);
    std::string str;

    do {
        std::getline(stream, str, c);
        result.push_back(str);
    } while (!stream.fail());

    return result;
}