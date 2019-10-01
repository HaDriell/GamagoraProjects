#include "Mesh.h"

#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>

Mesh::~Mesh() {}

//https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
vec3 triangle_normal(const vec3& vertex0, const vec3& vertex1, const vec3& vertex2)
{
    vec3 edge1 = vertex1 - vertex0;
    vec3 edge2 = vertex2 - vertex0;
    return edge1.cross(edge2).normalise();
}


//https://fr.wikipedia.org/wiki/Algorithme_d%27intersection_de_M%C3%B6ller%E2%80%93Trumbore
HitResult intersect_triangle(const vec3& position, const vec3& direction, const vec3& vertex0, const vec3& vertex1, const vec3& vertex2)
{
    HitResult hit;
    hit.hit = false;

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
        hit.normal   = triangle_normal(vertex0, vertex1, vertex2);
    }

    return hit;
}


HitResult Mesh::intersect(const vec3& position, const vec3& direction)
{
    HitResult hit;
    int tCount = indices.size() / 3;
    for (int t = 0; t < tCount; t += 3)
    {
        vec3 v0 = positions[t + 0];
        vec3 v1 = positions[t + 1];
        vec3 v2 = positions[t + 2];
        hit = intersect_triangle(position, direction, v0, v1, v2);

        //Intersect already setups everything else for us
        if (hit.hit)
        {
            hit.instance = this;
            return hit;
        }
    }
    return hit;
}

vec3 Mesh::get_random_point_on_surface(std::default_random_engine& random, float bias)
{
    //TODO : find a random triangle on surface 
    return vec3();
}


unsigned int __obj_read_vertex_info(std::istringstream& stream, unsigned int& v, unsigned int& vt, unsigned int& vn)
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
    unsigned int vertexCount;
    unsigned int faceCount;
    std::vector<vec3> positions;
    std::vector<vec3> colors;
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
            colors.reserve(faceCount);
            section = VERTEX_DATA;
        }
        break;

        case VERTEX_DATA:
        {
            vec3 v;

            if (!(stream >> v.x >> v.y >> v.z))
            {
                std::cout << "Aborting load : unable to read vertex data" << std::endl;
                return;
            }
            
            positions.push_back(v);

            //We're done loading vertex positions
            if (positions.size() == vertexCount)
                section = FACE_DATA;
        }
        break;

        case FACE_DATA:
        {
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
            vec3 color;
            if (!(stream >> color.x >> color.y >> color.z))
            {
                color = vec3{1.f, 1.f, 1.f};
            }
            colors.push_back(color.normalise());

            //We're done loading Face data
            if (colors.size() == faceCount)
                section = Section::END;
        }
        break;
        }
    }
    this->positions = positions;
    this->colors = colors;
    this->indices = indices;
}

void Mesh::load_obj_file(const std::string& path)
{
    struct Vertex {
        unsigned int v0,  v1,  v2; // 3 positions
        unsigned int vt0, vt1, vt2;// 3 texCoord
        unsigned int vn0, vn1, vn2;// 3 normals
    };

    std::ifstream file(path);
    if (!file.is_open())
        return;
    
    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec2> uvs;

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
                vec3 position;
                if (stream >> position.x >> position.y >> position.z)
                {
                    positions.push_back(position);
                    continue;
                }
            }

            //Normal handling
            if (header == "vn")
            {
                vec3 normal;
                if (stream >> normal.x >> normal.y >> normal.z)
                {
                    normals.push_back(normal);
                    continue;
                }
            }

            //UVmapping handling
            if (header == "vt")
            {
                vec2 uv;
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
                __obj_read_vertex_info(stream, vertex.v0, vertex.vt0, vertex.vn0);

                //Read second vertex info
                while (stream.peek() == ' ') stream.ignore();
                __obj_read_vertex_info(stream, vertex.v1, vertex.vt1, vertex.vn1);

                //Read third vertex info
                while (stream.peek() == ' ') stream.ignore();
                __obj_read_vertex_info(stream, vertex.v2, vertex.vt2, vertex.vn2);

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
    }
}


vec3 Mesh::compute_centroid()
{
    float area = 0;
    vec3 centroid = {0, 0, 0};
    int tCount = indices.size();
    for (int t = 0; t < tCount; t += 3)
    {
        vec3 vertex0 = positions[t + 0];
        vec3 vertex1 = positions[t + 1];
        vec3 vertex2 = positions[t + 2];
        vec3 edge1 = vertex1 - vertex0;
        vec3 edge2 = vertex2 - vertex0;
        //Compute triangle Centroid & Area
        vec3 tCentroid = (vertex0 + vertex1 + vertex2) / 3;
        float tArea = 0.5 * edge1.cross(edge2).length();

        //Sum
        centroid += tArea * tCentroid;
        area += tArea;
    }
    return centroid /= area;
}


void Mesh::normalize()
{
    vec3 min, max;
    
    min.x = std::numeric_limits<float>::max();
    min.y = std::numeric_limits<float>::max();
    min.z = std::numeric_limits<float>::max();
    
    max.x = std::numeric_limits<float>::min();
    max.y = std::numeric_limits<float>::min();
    max.z = std::numeric_limits<float>::min();
    
    for (vec3 v : positions)
    {
        //Update min
        if (v.x < min.x) min.x = v.x;
        if (v.y < min.y) min.y = v.y;
        if (v.z < min.z) min.z = v.z;
        //Update max
        if (v.x > max.x) max.x = v.x;
        if (v.y > max.y) max.y = v.y;
        if (v.z > max.z) max.z = v.z;
    }
    vec3 min2max = max - min;

    //Normalize every vertex into the [-1;1] space
    for (int v = 0; v < positions.size(); v++)
    {
        positions[v] = 2 * (positions[v] - min) / min2max - 1;
    }
}