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

    std::cout << "Loaded OBJ data" << std::endl;
    std::cout << "Positions : " << obj_positions.size() << std::endl;
    std::cout << "Normals   : " << obj_normals.size() << std::endl;
    std::cout << "UVs       : " << obj_uvs.size() << std::endl;
    std::cout << "Faces     : " << obj_faces.size() << std::endl;

    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec4> colors;
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
        OBJVertex vertex;
        unsigned int fan = 2;
        while (fan < vertices.size()) {
            vertex = vertices[0];
            if (vertex.position) indices.push_back(index++);
            if (vertex.position) colors.push_back(vec4(1));
            if (vertex.position) positions.push_back(obj_positions[vertex.position - 1]);
            if (vertex.normal)   normals.push_back(obj_normals[vertex.normal - 1]);
            if (vertex.uv)       uvs.push_back(obj_uvs[vertex.uv - 1]);
 
            vertex = vertices[fan - 1];
            if (vertex.position) indices.push_back(index++);
            if (vertex.position) colors.push_back(vec4(1));
            if (vertex.position) positions.push_back(obj_positions[vertex.position - 1]);
            if (vertex.normal)   normals.push_back(obj_normals[vertex.normal - 1]);
            if (vertex.uv)       uvs.push_back(obj_uvs[vertex.uv - 1]);

            vertex = vertices[fan - 0];
            if (vertex.position) indices.push_back(index++);
            if (vertex.position) colors.push_back(vec4(1));
            if (vertex.position) positions.push_back(obj_positions[vertex.position - 1]);
            if (vertex.normal)   normals.push_back(obj_normals[vertex.normal - 1]);
            if (vertex.uv)       uvs.push_back(obj_uvs[vertex.uv - 1]);

            //Increment to the next index
            fan++;
        }
    }

    std::cout << "Transformed OBJ data" << std::endl;
    std::cout << "Positions : " << positions.size() << std::endl;
    std::cout << "Normals   : " << normals.size() << std::endl;
    std::cout << "Colors    : " << colors.size() << std::endl;
    std::cout << "UVs       : " << uvs.size() << std::endl;
    std::cout << "Indices   : " << indices.size() << std::endl;


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
    if (stream.peek() == '/') stream.ignore(1);
    if (stream >> vertex.uv) { read++; }
    if (stream.peek() == '/') stream.ignore(1);
    if (stream >> vertex.normal) { read++; }
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

///////////////////// TODO : remove below

// void Mesh::loadFromOBJ(const std::string& path)
// {
//     std::ifstream file(path);

//     if (!file.is_open())
//     {
//         std::cout << "Failed to load Mesh from file : " << path << " not found !" << std::endl;
//         return;
//     }

//     //Face Vertex
//     struct Vertex
//     {
//         unsigned int v = 0;
//         unsigned int vn = 0;
//         unsigned int vt = 0;
//     };

//     std::vector<vec3> obj_positions;
//     std::vector<vec3> obj_normals;
//     std::vector<vec2> obj_uvs;

//     std::vector<Vertex> obj_indices;

//     std::string line;
//     while (std::getline(file, line))
//     {
//         std::istringstream stream(line);
//         std::string header;

//         //Read header
//         if (!std::getline(stream, header, ' '))
//             continue;

//         //Ignore comment
//         if (header == "#")
//             continue;
        
//         //Reading a vertex
//         if (header == "v")
//         {
//             vec3 data;
//             if (__read_vec3(stream, data))
//                 obj_positions.push_back(data);
//         }

//         //Reading a vertex normal
//         if (header == "vn")
//         {
//             vec3 data;
//             if (__read_vec3(stream, data))
//                 obj_normals.push_back(data);
//         }

//         //Reading a vertex UVMapping
//         if (header == "vt")
//         {
//             vec2 data;
//             if (__read_vec2(stream, data))
//                 obj_uvs.push_back(data);
//         }

//         if (header == "f")
//         {
//             Vertex vertex0, vertex1, vertex2;
//             if (__read_vertex(stream, vertex0.v, vertex0.vn, vertex0.vt)
//             &&  __read_vertex(stream, vertex1.v, vertex1.vn, vertex1.vt)
//             &&  __read_vertex(stream, vertex2.v, vertex2.vn, vertex2.vt))
//             {
//                 obj_indices.push_back(vertex0);
//                 obj_indices.push_back(vertex1);
//                 obj_indices.push_back(vertex2);
//             } else {
//                 std::cout << "Error loading face data" << std::endl;
//             }
//         }
//     }

//     std::vector<unsigned int> indices;
//     std::vector<vec3> positions;
//     std::vector<vec3> normals;
//     std::vector<vec3> colors;
//     std::vector<vec2> uvs;

//     unsigned int vertexIndex = 0;
//     for (Vertex vertex : obj_indices)
//     {
//         indices.push_back(vertexIndex++);
//         positions.push_back(obj_positions[vertex.v - 1]);
//         colors.push_back(vec3(1));
//         if (vertex.vn) 
//         {
//             normals.push_back(obj_normals[vertex.vn - 1]);
//         }
//         if (vertex.vt)
//         {
//             uvs.push_back(obj_uvs[vertex.vt - 1]);
//         }
//     }

//     //Upload data to the according buffers
//     setPositions(positions);
//     setNormals(normals);
//     setColors(colors);
//     setUVs(uvs);
//     setIndices(indices);
// }

// //Useful Parsing functions
// bool __read_unsigned_int(std::istringstream& stream, unsigned int& value)
// {
//     stream >> std::ws; // consume whitespaces

//     if (!(stream >> value))
//         return false;
//     return true;
// }

// bool __read_vec3(std::istringstream& stream, vec3& v)
// {
//     stream >> std::ws; // consume whitespaces

//     if (!(stream >> v.x >> v.y >> v.z))
//         return false;
//     return true;
// }

// bool __read_vec2(std::istringstream& stream, vec2& v)
// {
//     stream >> std::ws; // consume whitespaces

//     if (!(stream >> v.x >> v.y))
//         return false;
//     return true;
// }

// bool __read_vertex(std::istringstream& stream, unsigned int& v, unsigned int& vn, unsigned int& vt)
// {
//     stream >> std::ws; // consume whitespaces

//     std::string data;
//     std::getline(stream, data, ' ');
//     if (stream.fail())
//         return false;

//     //Vertex data only
//     if (data.find("/") == std::string::npos)
//     {
//         v = std::stoi(data);
//         vn = 0;
//         vt = 0;
//         return true;
//     }

//     //Vertex + Normal data only
//     if (data.find("//") != std::string::npos)
//     {
//         std::istringstream dstream(data);
//         dstream >> v;
//         dstream.ignore(2); // skip '//'
//         dstream >> vn;
//         vt = 0;
//         return !dstream.fail();
//     }

//     //Vertex + UV data only
//     if (data.find("/") == data.rfind("/"))
//     {
//         std::istringstream dstream(data);
//         dstream >> v;
//         dstream.ignore(1); // skip '/'
//         dstream >> vt;
//         return !dstream.fail();
//     }
//     else //Vertex + UV + Normal
//     {
//         std::istringstream dstream(data);
//         dstream >> v;
//         dstream.ignore(1); // skip '/'
//         dstream >> vt;
//         dstream.ignore(1); // skip '/'
//         dstream >> vn;
//         return !dstream.fail();
//     }
//     return true;
// }