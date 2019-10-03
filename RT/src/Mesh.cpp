#include "Mesh.h"

#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>

//Useful Parsing functions
bool __read_unsigned_int(std::istringstream& stream, unsigned int& value)
{
    if (!(stream >> value))
        return false;
    return true;
}

bool __read_vec3(std::istringstream& stream, vec3& v)
{
    if (!(stream >> v.x >> v.y >> v.z))
        return false;
    return true;
}

bool __read_vec2(std::istringstream& stream, vec2& v)
{
    if (!(stream >> v.x >> v.y))
        return false;
    return true;
}

void normalize_vertices(std::vector<Vertex>& vertices)
{
    vec3 centroid;
    float area = 0;

    int triangle = 0;
    while (triangle < vertices.size())
    {
        vec3 vertex0 = vertices[triangle++].position;
        vec3 vertex1 = vertices[triangle++].position;
        vec3 vertex2 = vertices[triangle++].position;
        vec3 edge1 = vertex1 - vertex0;
        vec3 edge2 = vertex2 - vertex0;

        //Compute triangle Centroid & Area
        vec3 tCentroid = (vertex0 + vertex1 + vertex2) / 3;
        float tArea = 0.5 * edge1.cross(edge2).length();

        //Sum
        centroid += tArea * tCentroid;
        area += tArea;
    }
    centroid /= area;

    //Center Mesh around origin
    for (Vertex& vertex : vertices)
    {
        vertex.position -= centroid;
    }

    //Compute the max X, Y and Z
    vec3 max = vec3(std::numeric_limits<float>::min());   
    for (Vertex& vertex : vertices)
    {
        max.x = std::max(max.x, vertex.position.x);
        max.y = std::max(max.y, vertex.position.y);
        max.z = std::max(max.z, vertex.position.z);
    }

    //Normalize if possible
    if (max.length2() > 0)
    {
        float scale = 1.f / max.length();
        for (Vertex& vertex : vertices)
        {
            vertex.position *= scale;
        }
    }
}


//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
HitResult intersect_triangle(const vec3& position, const vec3& direction, const vec3& vertex0, const vec3& vertex1, const vec3& vertex2)
{
    HitResult hit;

    const float EPSILON = 1e-6;
    vec3 edge1 = vertex1 - vertex0;
    vec3 edge2 = vertex2 - vertex0;

    vec3 h, s, q;
    float a, f, u, v;

    h = direction.cross(edge2);
    a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON)
        return hit;

    f = 1.0/a;
    s = position - vertex0;
    u = f * (s.dot(h));
    if (u < 0.0 || u > 1.0)
        return hit;

    q = s.cross(edge1);
    v = f * direction.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return hit;
    
    float t = f * edge2.dot(q);
    if (t > EPSILON)
    {
        hit.hit = true;
        hit.distance = t;
        hit.hitPoint = position + direction * t;
        hit.normal   = edge1.cross(edge2).normalize();
    }

    return hit;
}

HitResult Mesh::intersect(const vec3& position, const vec3& direction)
{
    HitResult hit;

    int triangle = 0;
    while (!hit.hit && triangle < vertices.size())
    {
        vec3 v0 = vertices[triangle++].position;
        vec3 v1 = vertices[triangle++].position;
        vec3 v2 = vertices[triangle++].position;
        hit = intersect_triangle(position, direction, v0, v1, v2);
    }

    if (hit.hit)
    {
        hit.instance = this;
        return hit;
    }

    return hit;
}

vec3 Mesh::get_random_point_on_surface(std::default_random_engine& random, float bias)
{
    //TODO : find a random triangle on surface 
    return vec3();
}

void Mesh::load_off_file(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return;

    enum Section
    {
        MAGICK_NUMBER = 0,
        HEADER,
        VERTEX_DATA,
        FACE_DATA,
        END
    };
    
    Section section = MAGICK_NUMBER;
    unsigned int vertexCount, vertexRead = 0;
    unsigned int faceCount, faceRead = 0;

    //Indexed data
    std::vector<vec3> positions;

    std::vector<Vertex> vertices;



    
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
        case MAGICK_NUMBER:
        {
            //This is a header file type check.
            if (line[0] != 'O' 
            ||  line[1] != 'F'
            ||  line[2] != 'F')
            {
                std::cout << "Aborting load : not an OFF file" << std::endl;
                return;
            }

            section = HEADER;            
        }
        break;

        case HEADER:
        {
            if (!(stream >> vertexCount >> faceCount))
            {
                std::cout << "Aborting load : unable to read vertex and face count" << std::endl;
                return;
            }

            positions.reserve(vertexCount);
            section = VERTEX_DATA;
        }
        break;

        case VERTEX_DATA:
        {
            vec3 v;
            if (!__read_vec3(stream, v))
            {
                std::cout << "Aborting load : unable to read vertex data" << std::endl;
                return;
            }
            positions.push_back(v);

            vertexRead++;
            if (vertexRead >= vertexCount)
                section = FACE_DATA;
        }
        break;

        case FACE_DATA:
        {
            //Tokenize everything then parse
            unsigned int value;
            std::vector<unsigned int> data;
            while (__read_unsigned_int(stream, value)) data.push_back(value);

            if (data.empty())
            {
                std::cout << "Aborting load : Face without data !" << std::endl;
            }

            unsigned int face_index_count = data[0];            

            //Check that Face declares a correct number of indices
            if (face_index_count < 3)
            {
                std::cout << "Aborting load : Face declared only " << face_index_count << " indices." << std::endl;
                return;
            }
            if (face_index_count > data.size() - 1)
            {
                std::cout << "Aborting load : Face declared " << face_index_count << " indices but there are only " << (data.size() - 1) << " available max !" << std::endl;
                return;
            }

            vec3 color = vec3(1, 1, 1);
            if (face_index_count + 3 + 1 == data.size()) // there is a custom color 
            {
                //Data stores RGB in a [0, 255] range
                color.x = data[data.size() - 3] / 255.f;
                color.y = data[data.size() - 2] / 255.f;
                color.z = data[data.size() - 1] / 255.f;
            }


            int fan = 2 + 1;

            while (fan < data.size() - 3)
            {
                Vertex v;
                v.color = color;

                //Fan base is at 1 instead of 0
                v.position = data[1]; 
                vertices.push_back(v);

                v.position = data[fan - 1];
                vertices.push_back(v);

                v.position = data[fan];
                vertices.push_back(v);

                fan++;
            }
            faceRead++;
            if (faceRead >= faceCount)
                section = END;
        }
        break;
        }
    }
    normalize_vertices(vertices);
    this->vertices = vertices;
}



//OBJ parser
void Mesh::load_obj_file(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return;

    //File data
    std::vector<vec3> positions;
    std::vector<vec3> normals; // unused
    std::vector<vec2> uvs;     // unused

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
                vec3 vertex;
                if (__read_vec3(stream, vertex))
                {
                    positions.push_back(vertex);
                    continue;
                }
            }

            //Normal handling
            if (header == "vn")
            {
                vec3 normal;
                if (__read_vec3(stream, normal))
                {
                    normals.push_back(normal);
                    continue;
                }
            }

            //UVmapping handling
            if (header == "vt")
            {
                vec2 uv;
                if (__read_vec2(stream, uv))
                {
                    uvs.push_back(uv);
                    continue;
                }
            }

            if (header == "f")
            {
                unsigned int v  = 0;
                unsigned int vn = 0;
                unsigned int vt = 0;

                Vertex v0;
                v0.color = vec3(1, 1, 1); // white by default
                stream >> v;
                if (!stream.fail()) v0.position = positions[v - 1];
                if (stream.peek() == '/')
                {
                    stream.ignore(1, '/');
                    stream >> vt;
                    //TODO : use vt
                }
                if (stream.peek() == '/')
                {
                    stream.ignore(1, '/');
                    stream >> vn;
                    //TODO : use vn
                }
                vertices.push_back(v0);

                Vertex v1;
                v1.color = vec3(1, 1, 1); // white by default
                stream >> v;
                if (!stream.fail()) v1.position = positions[v - 1];
                if (stream.peek() == '/')
                {
                    stream.ignore(1, '/');
                    stream >> vt;
                    //TODO : use vt
                }
                if (stream.peek() == '/')
                {
                    stream.ignore(1, '/');
                    stream >> vn;
                    //TODO : use vn
                }
                vertices.push_back(v1);

                Vertex v2;
                v2.color = vec3(1, 1, 1); // white by default
                stream >> v;
                if (!stream.fail()) v2.position = positions[v - 1];
                if (stream.peek() == '/')
                {
                    stream.ignore(1, '/');
                    stream >> vt;
                    //TODO : use vt
                }
                if (stream.peek() == '/')
                {
                    stream.ignore(1, '/');
                    stream >> vn;
                    //TODO : use vn
                }
                vertices.push_back(v2);
                continue;
            }
        }
    }
    file.close();

    normalize_vertices(vertices);
    this->vertices = vertices;
}


Mesh::~Mesh() {}