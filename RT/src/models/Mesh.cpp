#include "Mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>

void load_mesh_obj(const std::string& path, std::vector<Mesh*>& meshes)
{
    std::ifstream file;
    file.open(path);
    if (file.is_open())
    {
        //Context
        std::string header;
        std::string name;
        Mesh* mesh = nullptr;

        glm::vec3 normal;
        glm::vec3 vertex;
        glm::vec2 uv;

        Face face;

        //Parse the obj file
        file.seekg(0, std::ios::beg);
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream stream(line);
            if (std::getline(stream, header, ' '))
            {
                //Comment handling
                if (header == "#")
                {
                    //Nothing to do : ignore line quietly.
                    continue;
                }

                //Mesh handling                
                if (header == "o" && (stream >> name))
                {
                    std::cout << "Mesh " << name << std::endl;
                    //Push already existing mesh into the meshes vector
                    if (mesh)
                        meshes.push_back(mesh);
                    //Create a new Mesh 
                    mesh = new Mesh();
                    continue;
                }

                //Vertex handling
                if (header == "v" && (stream >> vertex.x >> vertex.y >> vertex.z))
                {
                    std::cout << "Vertex " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
                    mesh->vertices.push_back(vertex);
                    continue;
                }

                //UVmapping handling
                if (header == "vt" && (stream >> uv.x >> uv.y))
                {
                    std::cout << "UVMapping " << uv.x << " " << uv.y << std::endl;
                    mesh->uvs.push_back(uv);
                    continue;
                }

                //Normal handling
                if (header == "vn" && (stream >> normal.x >> normal.y >> normal.z))
                {
                    std::cout << "Normal " << normal.x << " " << normal.y << " " << normal.z << std::endl;
                    mesh->normals.push_back(normal);
                    continue;
                }

                //Face handling
                if (header == "f" 
                && stream >> face.v[0] && stream.ignore() 
                && stream >> face.vt[0] && stream.ignore() 
                && stream >> face.vn[0]
                
                && stream >> face.v[1] && stream.ignore() 
                && stream >> face.vt[1] && stream.ignore() 
                && stream >> face.vn[1]
                
                && stream >> face.v[2] && stream.ignore() 
                && stream >> face.vt[2] && stream.ignore() 
                && stream >> face.vn[2])
                {
                    std::cout << "Face " 
                    << face.v[0] << "/" << face.vt[0] << "/" << face.vn[0] << " "
                    << face.v[1] << "/" << face.vt[1] << "/" << face.vn[1] << " "
                    << face.v[2] << "/" << face.vt[2] << "/" << face.vn[2] << std::endl;
                    mesh->faces.push_back(face);
                    continue;
                }

                //Not handled. Panick ! ! ! (but keep going)
                std::cout << "Unhandled line !!! '"<< line << "'" << std::endl;
            }
        }
        //Load the last mesh before exiting
        if (mesh)
            meshes.push_back(mesh);
        mesh = nullptr;
    }
    file.close();
}