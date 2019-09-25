#include "Mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>

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

void Mesh::load_obj(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return;
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
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
                    std::cout << "Vertex " << position.x << " " << position.y << " " << position.z << std::endl;
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
                    std::cout << "Normal " << normal.x << " " << normal.y << " " << normal.z << std::endl;
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
                    std::cout << "UV " << uv.x << " " << uv.y << std::endl;
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

            //Not handled. Panick ! ! ! (but keep going)
            std::cout << "Unhandled line !!! '"<< line << "'" << std::endl;
        }
    }
    file.close();

    //Update Mesh properties
    this->positions = positions;
    this->normals = normals;
    this->uvs = uvs;
    this->vertices = vertices;
}