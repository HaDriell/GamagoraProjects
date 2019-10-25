#include "OBJ.h"

#include "maths.h"

#include <algorithm>
#include <fstream>
#include <sstream>

struct OBJVertex
{
    unsigned int position = 0;
    unsigned int normal   = 0;
    unsigned int uv       = 0;
};

bool load_obj_position(std::istringstream& stream, std::vector<vec3>& positions);
bool load_obj_normal(std::istringstream& stream, std::vector<vec3>& normals);
bool load_obj_uv(std::istringstream& stream, std::vector<vec2>& uvs);
bool load_obj_vertex(std::istringstream& stream, std::vector<OBJVertex>& vertices);
bool load_obj_face(std::istringstream& stream, std::vector<std::vector<OBJVertex>>& faces);

bool loadOBJFile(const std::string& filename, Mesh& mesh)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cout << "Failed to load Mesh from file : " << filename << " not found !" << std::endl;
        return false;
    }
    std::vector<std::vector<OBJVertex>> obj_faces;
    std::vector<vec3> obj_positions;
    std::vector<vec3> obj_normals;
    std::vector<vec2> obj_uvs;

    std::string line;
    while (std::getline(file >> std::ws, line))
    {
        //Remove return carriage from Window encoded files
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
        
        std::istringstream stream(line);
        std::string header;
        //Read the line header
        if (std::getline(stream >> std::ws, header, ' '))
        {
            //Parse a position
            if (header == "v")
            {
                if (!load_obj_position(stream, obj_positions))
                {
                    std::cout << "Failed to load Mesh from file : Failed to parse OBJ Position" << std::endl;
                    return false;
                }
            }

            //Parse a normal
            if (header == "vn")
            {
                if (!load_obj_normal(stream, obj_normals))
                {
                    std::cout << "Failed to load Mesh from file : Failed to parse OBJ Normal" << std::endl;
                    return false;
                }
            }

            //Parse a texture coordinate
            if (header == "vt")
            {
                if (!load_obj_uv(stream, obj_uvs))
                {
                    std::cout << "Failed to load Mesh from file : Failed to parse OBJ UV Mapping" << std::endl;
                    return false;
                }
            }

            //Parse a face
            if (header == "f")
            {
                if (!load_obj_face(stream, obj_faces))
                {
                    std::cout << "Failed to load Mesh from file : Failed to parse OBJ Face indices" << std::endl;
                    return false;
                }
            }
        }
    }

#if 0
    std::cout << "Loaded OBJ data" << std::endl;
    std::cout << "Positions : " << obj_positions.size() << std::endl;
    std::cout << "Normals   : " << obj_normals.size() << std::endl;
    std::cout << "UVs       : " << obj_uvs.size() << std::endl;
    std::cout << "Faces     : " << obj_faces.size() << std::endl;
#endif

    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec3> colors;
    std::vector<vec2> uvs;
    std::vector<unsigned int> indices;

    unsigned int index = 0;
    for (std::vector<OBJVertex>& vertices : obj_faces)
    {
        if (vertices.size() < 3)
        {
            std::cout << "Discarding Face data : " << vertices.size() << " < 3" << std::endl;
        }

        //Triangulate face using a basic fanning triangulation
        unsigned int fan = 2;
        while (fan < vertices.size()) {
            OBJVertex vertex0 = vertices[0];
            OBJVertex vertex1 = vertices[fan - 1];
            OBJVertex vertex2 = vertices[fan];

            vec3 p0 = obj_positions[vertex0.position - 1];
            vec3 p1 = obj_positions[vertex1.position - 1];
            vec3 p2 = obj_positions[vertex2.position - 1];
            vec3 edge1 = p1 - p0;
            vec3 edge2 = p2 - p0;
            vec3 default_normal = vec3::cross(edge1, edge2).normalize();

            //Generate Vertex data
            positions.push_back(p0);
            positions.push_back(p1);
            positions.push_back(p2);

            //Normal should be backed up using the computed triangle normal
            normals.push_back(vertex0.normal ? obj_normals[vertex0.normal - 1] : default_normal);
            normals.push_back(vertex1.normal ? obj_normals[vertex1.normal - 1] : default_normal);
            normals.push_back(vertex2.normal ? obj_normals[vertex2.normal - 1] : default_normal);

            //There is no color data in OBJ Format, use white by default
            colors.push_back(vec3(1));
            colors.push_back(vec3(1));
            colors.push_back(vec3(1));

            //If there is no UV just map all UVs to 0, 0
            uvs.push_back(vertex0.uv ? obj_uvs[vertex0.uv - 1] : vec2());
            uvs.push_back(vertex1.uv ? obj_uvs[vertex1.uv - 1] : vec2());
            uvs.push_back(vertex2.uv ? obj_uvs[vertex2.uv - 1] : vec2());

            //No Index optimization in OBJ Format
            indices.push_back(index++);
            indices.push_back(index++);
            indices.push_back(index++);

            //Increment to the next index
            fan++;
        }
    }

#if 0
    std::cout << "Transformed OBJ data" << std::endl;
    std::cout << "Positions : " << positions.size() << std::endl;
    std::cout << "Normals   : " << normals.size() << std::endl;
    std::cout << "Colors    : " << colors.size() << std::endl;
    std::cout << "UVs       : " << uvs.size() << std::endl;
    std::cout << "Indices   : " << indices.size() << std::endl;
    std::cin.get();
#endif

    mesh.setPositions(positions);
    mesh.setNormals(normals);
    mesh.setColors(colors);
    mesh.setUVs(uvs);
    mesh.setIndices(indices);
    return true;
}

bool load_obj_position(std::istringstream& stream, std::vector<vec3>& positions)
{
    stream >> std::ws; // consume whitespaces
    vec3 v = vec3(0);
    if (!(stream >> v.x >> v.y >> v.z))
        return false;
    positions.push_back(v);
    return true;
}

bool load_obj_normal(std::istringstream& stream, std::vector<vec3>& normals)
{
    stream >> std::ws; // consume whitespaces
    vec3 v = vec3(0);
    if (!(stream >> v.x >> v.y >> v.z))
        return false;
    normals.push_back(v);
    return true;
}

bool load_obj_uv(std::istringstream& stream, std::vector<vec2>& uvs)
{
    stream >> std::ws; // consume whitespaces
    vec2 v = vec2(0);
    if (!(stream >> v.x >> v.y))
        return false;
    uvs.push_back(v);
    return true;
}

bool load_obj_vertex(std::istringstream& stream, std::vector<OBJVertex>& vertices)
{
    OBJVertex vertex;
    unsigned int read = 0;

    if (stream >> vertex.position) { read++; }

    if (stream.peek() == '/') stream.ignore();

    if (stream.peek() == '/' || (stream >> vertex.uv)) { read++; }

    if (stream.peek() == '/') stream.ignore();

    if (stream.peek() == '/' ||  (stream >> vertex.normal)) { read++; }
    
    if (!read)
        return false;
    vertices.push_back(vertex);
    return true;
}

bool load_obj_face(std::istringstream& stream, std::vector<std::vector<OBJVertex>>& faces)
{
    std::vector<OBJVertex> face;

    std::string vertexStringData;
    while (std::getline(stream >> std::ws, vertexStringData, ' '))
    {
        std::istringstream vstream(vertexStringData);
        if (!load_obj_vertex(vstream, face))
            return false;
    }
    
    faces.push_back(face);
    return true;
}