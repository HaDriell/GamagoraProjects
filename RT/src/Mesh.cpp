#include "Mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include "Util.h"

unsigned int read_vertex_info(std::istringstream& stream, unsigned int& v, unsigned int& vt, unsigned int& vn)
{
    unsigned int a, b, c;
    unsigned int count = 0;

    if (stream.peek() != ' ' && stream >> a)
    {
        count++;
        while (stream.peek() == '/') stream.ignore();
    }

    if (stream.peek() != ' ' && stream >> b)
    {
        count++;
        while (stream.peek() == '/') stream.ignore();
    }
    
    if (stream.peek() != ' ' && stream >> c)
    {
        count++;
        while (stream.peek() == '/') stream.ignore();
    }
    switch (count)
    {
        case 1:
            v = a;
        break;

        case 2:
            v = a;
            vn = b;
        break;

        case 3:
            v = a;
            vt = b;
            vn = c;
        break;
    
        default:
        break;
    }

    return count;
}

void Mesh::load_off(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return;

    enum Section {
        MAGICK_NUMBER = 0,
        HEADER,
        VERTEX_DATA,
        FACE_DATA,
        END
    };
    
    Section section = MAGICK_NUMBER;
    unsigned int vertexCount;
    unsigned int faceCount;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> colors;
    std::vector<unsigned int> indices;

    
    //Parse file
    std::string line;
    while (section != END && std::getline(file, line))
    {
        if (line[line.length() - 1] == '\r')
            line = line.substr(0, line.length() - 1);

        //Ignore empty lines
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream stream(line);

        switch (section)
        {
        case Section::MAGICK_NUMBER:
            //This is a header file type check.
            if (line[0] != 'O' 
            ||  line[1] != 'F'
            ||  line[2] != 'F')
            {
                std::cout << "Aborting load : not an OFF file" << std::endl;
                return;
            }

            section = Section::HEADER;            
        break;

        case Section::HEADER:

            if (!(stream >> vertexCount >> faceCount))
            {
                std::cout << "Aborting load : unable to read vertex and face count" << std::endl;
                return;
            }

            positions.reserve(vertexCount);
            colors.reserve(faceCount);
            section = Section::VERTEX_DATA;
        break;

        case Section::VERTEX_DATA:
            glm::vec3 v;

            if (!(stream >> v.x >> v.y >> v.z))
            {
                std::cout << "Aborting load : unable to read vertex data" << std::endl;
                return;
            }
            
            positions.push_back(v);

            //We're done loading vertex positions
            if (positions.size() == vertexCount)
                section = Section::FACE_DATA;
        break;

        case Section::FACE_DATA:
            unsigned int facePoints;

            if (!(stream >> facePoints) || facePoints < 3)
            {
                std::cout << "Aborting load : unable to read face data" << std::endl;
                return;
            }
            
            unsigned int v0, v1, v2;

            //Minimum 3 index read
            if (!(stream >> v0 >> v1 >> v2))
                return;
            
            //force at least one pass
            for (int i = 3 - 1; i < facePoints; i++)
            {
                //Store points as triplets of indices
                indices.push_back(v0);
                indices.push_back(v1);
                indices.push_back(v2);

                //Shift v2 & v1 and consume another point
                if (i + 1 < facePoints)
                {
                    v1 = v2;
                    stream >> v2;
                }
            }

            //Colors are optional. Default to (1, 1, 1)
            glm::vec3 color;
            if (!(stream >> color.x >> color.y >> color.z))
            {
                color = glm::vec3{1.f, 1.f, 1.f};
            }
            colors.push_back(glm::normalize(color));

            //We're done loading Face data
            if (colors.size() == faceCount)
                section = Section::END;
        break;    
        }
    }
    this->positions = positions;
    this->colors = colors;
    this->indices = indices;
}

void Mesh::load_obj(const std::string& path)
{
    struct Vertex {
        unsigned int v0,  v1,  v2; // 3 positions
        unsigned int vt0, vt1, vt2;// 3 texCoord
        unsigned int vn0, vn1, vn2;// 3 normals
    };

    std::ifstream file(path);
    if (!file.is_open())
        return;
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    //OBJ index data
    std::vector<Vertex> vertices;


    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream stream(line);

        std::string header;
        if (std::getline(stream, header, ' '))
        {
            //Comment handling
            if (header == "#")
            {
                //Nothing to do : ignore line quietly.
                continue;
            }

            //Position handling
            if (header == "v")
            {
                glm::vec3 position;
                if (stream >> position.x >> position.y >> position.z)
                {
                    positions.push_back(position);
                    continue;
                }
            }

            //Normal handling
            if (header == "vn")
            {
                glm::vec3 normal;
                if (stream >> normal.x >> normal.y >> normal.z)
                {
                    normals.push_back(normal);
                    continue;
                }
            }

            //UVmapping handling
            if (header == "vt")
            {
                glm::vec2 uv;
                if (stream >> uv.x >> uv.y)
                {
                    uvs.push_back(uv);
                    continue;
                }
            }

            if (header == "f")
            {
                Vertex vertex;
                //Read first vertex info
                while (stream.peek() == ' ') stream.ignore();
                read_vertex_info(stream, vertex.v0, vertex.vt0, vertex.vn0);

                //Read second vertex info
                while (stream.peek() == ' ') stream.ignore();
                read_vertex_info(stream, vertex.v1, vertex.vt1, vertex.vn1);

                //Read third vertex info
                while (stream.peek() == ' ') stream.ignore();
                read_vertex_info(stream, vertex.v2, vertex.vt2, vertex.vn2);

                vertices.push_back(vertex);
                continue;
            }
        }
    }
    file.close();

    //Clear the Mesh
    this->positions.clear();
    this->normals.clear();
    this->uvs.clear();

    //preallocate memory
    this->positions = positions;
    this->normals = normals;
    this->uvs = uvs;

    //good enough but it's a total mess
    for (auto& vertex : vertices)
    {
        this->indices.push_back(vertex.v0 - 1);
        this->indices.push_back(vertex.v1 - 1);
        this->indices.push_back(vertex.v2 - 1);
        // //Copy vertices positions
        // this->positions.push_back(positions[vertex.v0]);
        // this->positions.push_back(positions[vertex.v1]);
        // this->positions.push_back(positions[vertex.v2]);
        // //Copy vertices normals (optional)
        // if (vertex.vn0 > 0) this->normals.push_back(normals[vertex.vn0 - 1]);
        // if (vertex.vn1 > 0) this->normals.push_back(normals[vertex.vn1 - 1]);
        // if (vertex.vn2 > 0) this->normals.push_back(normals[vertex.vn2 - 1]);
        // //Copy vertices texCoords (optional)
        // if (vertex.vt0 > 0) this->uvs.push_back(uvs[vertex.vt0 - 1]);
        // if (vertex.vt1 > 0) this->uvs.push_back(uvs[vertex.vt1 - 1]);
        // if (vertex.vt2 > 0) this->uvs.push_back(uvs[vertex.vt2 - 1]);
    }
}