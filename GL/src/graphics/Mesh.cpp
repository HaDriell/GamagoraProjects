#include "Mesh.h"

#include <glad/glad.h>
#include <algorithm>
#include <fstream>
#include <sstream>

//Useful Parsing functions
bool __read_unsigned_int(std::istringstream& stream, unsigned int& value);
bool __read_vec3(std::istringstream& stream, vec3& v);
bool __read_vec2(std::istringstream& stream, vec2& v);
bool __read_vertex(std::istringstream& stream, unsigned int& v, unsigned int& vn, unsigned int& vt);

Mesh::Mesh()
{
    //Create all OpenGL buffers associated with that mesh
    vertexArray = std::make_shared<VertexArray>();
    positions   = std::make_shared<VertexBuffer>(); 
    normals     = std::make_shared<VertexBuffer>(); 
    colors      = std::make_shared<VertexBuffer>();
    uvs         = std::make_shared<VertexBuffer>();
    indices = std::make_shared<IndexBuffer>();

    //Setup the layout
    positions->defineLayout({{ VertexAttributeType::Float3, "position" }});
    normals->defineLayout({{ VertexAttributeType::Float3, "normal" }});
    colors->defineLayout({{ VertexAttributeType::Float3, "color" }});
    uvs->defineLayout({{ VertexAttributeType::Float2, "uv" }});

    //Configure the bindings
    vertexArray->addVertexBuffer(positions);
    vertexArray->addVertexBuffer(normals);
    vertexArray->addVertexBuffer(colors);
    vertexArray->addVertexBuffer(uvs);
}

Mesh::~Mesh()
{
}

void Mesh::setIndices(const std::vector<unsigned int>& indices)
{
    this->indices->defineData(indices);
}

void Mesh::setIndices(const std::vector<unsigned short>& indices)
{
    this->indices->defineData(indices);
}

void Mesh::setPositions(const std::vector<vec3>& positions)
{
    this->positions->defineData(positions);
}

void Mesh::setNormals(const std::vector<vec3>& normals)
{
    this->normals->defineData(normals);
}

void Mesh::setColors(const std::vector<vec3>& colors)
{
    this->colors->defineData(colors);
}

void Mesh::setUVs(const std::vector<vec2>& uvs)
{
    this->uvs->defineData(uvs);
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
    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec3> colors;
    std::vector<vec2> uvs;

    unsigned int vertexIndex = 0;
    for (Vertex vertex : obj_indices)
    {
        indices.push_back(vertexIndex++);
        positions.push_back(obj_positions[vertex.v - 1]);
        colors.push_back(vec3(1));
        if (vertex.vn) 
        {
            normals.push_back(obj_normals[vertex.vn - 1]);
        }
        if (vertex.vt)
        {
            uvs.push_back(obj_uvs[vertex.vt - 1]);
        }
    }

    //Upload data to the according buffers
    setPositions(positions);
    setNormals(normals);
    setColors(colors);
    setUVs(uvs);
    setIndices(indices);
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
