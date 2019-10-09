#include "Mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


void normalize_vertices(std::vector<vec3>& vertices);

static inline float __compute_centroid_on_x(const Triangle* t) { return (t->vertex0.x + t->vertex1.x + t->vertex2.x) / 3.f; }
static inline float __compute_centroid_on_y(const Triangle* t) { return (t->vertex0.y + t->vertex1.y + t->vertex2.y) / 3.f; }
static inline float __compute_centroid_on_z(const Triangle* t) { return (t->vertex0.z + t->vertex1.z + t->vertex2.z) / 3.f; }

static inline vec3 __min_XYZ(const vec3& a, const vec3& b)
{
    vec3 out;
    out.x = std::min(a.x, b.x);
    out.y = std::min(a.y, b.y);
    out.z = std::min(a.z, b.z);
    return out;
}

static inline vec3 __max_XYZ(const vec3& a, const vec3& b)
{
    vec3 out;
    out.x = std::max(a.x, b.x);
    out.y = std::max(a.y, b.y);
    out.z = std::max(a.z, b.z);
    return out;
}


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


MeshBVH::~MeshBVH()
{
    if (left) delete left;
    if (right) delete right; 
}

MeshBVH::MeshBVH(const std::vector<Triangle*>& triangles, int level)
{
    box.min = vec3(std::numeric_limits<float>::max());
    box.max = vec3(std::numeric_limits<float>::min());

    for (Triangle* t : triangles)
    {
        //Expand min
        box.min = __min_XYZ(box.min, t->vertex0);
        box.min = __min_XYZ(box.min, t->vertex1);
        box.min = __min_XYZ(box.min, t->vertex2);
        //Expand max        
        box.max = __max_XYZ(box.max, t->vertex0);
        box.max = __max_XYZ(box.max, t->vertex1);
        box.max = __max_XYZ(box.max, t->vertex2);
    }

    if (triangles.size() <= 12 || level > 16)
    {
        //Leaf node. Simply store the triangles as is
        this->triangles = triangles;
        this->left = nullptr;
        this->right = nullptr;
    } else {
        //sort the triangles
        //Intermediate node. subdivide.

        std::vector<Triangle*> leftTriangles;
        std::vector<Triangle*> rightTriangles;

        //Find which axis has the highest delta
        float dx = box.max.x - box.min.x;
        float dy = box.max.y - box.min.y;
        float dz = box.max.z - box.min.z;

        for (Triangle* t : triangles)
        {
            //Subdivide along X
            if (dx > dy && dx > dz)
            {
                if (__compute_centroid_on_x(t) - box.min.x < dx / 2)
                    leftTriangles.push_back(t);
                else
                    rightTriangles.push_back(t);
            }
            //Subdivide along Y
            else if (dy > dx && dy > dz)
            {
                if (__compute_centroid_on_y(t) - box.min.y < dy / 2)
                    leftTriangles.push_back(t);
                else
                    rightTriangles.push_back(t);
            }
            //Subdivide along Z
            if (dz > dx && dz > dy)
            {
                if (__compute_centroid_on_z(t) - box.min.z < dz / 2)
                    leftTriangles.push_back(t);
                else
                    rightTriangles.push_back(t);
            }
        }

        //This is undivisible using this method
        if (leftTriangles.empty() || rightTriangles.empty())
        {
            this->left = nullptr;
            this->right = nullptr;
            this->triangles = triangles;
        } else {
            this->left  =  new MeshBVH(leftTriangles, level + 1);
            this->right = new MeshBVH(rightTriangles, level + 1);
        }
    }
}

bool MeshBVH::intersect(const vec3& position, const vec3& direction, float& distance, vec3& hitPoint, vec3& normal) const
{
    float d;
    vec3 hp;
    vec3 n;
    if(intersectAABB(position, direction, box, d, hp, n))
    {
        if (triangles.empty())
        {
            //Not a leaf, just forward the raycast
            if (left && left->intersect(position, direction, distance, hitPoint, normal))
                return true;
            if (right && right->intersect(position, direction, distance, hitPoint, normal))
                return true;
        } else {
            d = std::numeric_limits<float>::max();

            float currentDistance;
            vec3 currentHitPoint;
            vec3 currentNormal;
            for (Triangle* t : triangles)
            {
                if (intersectTriangle(position, direction, *t, currentDistance, currentHitPoint, currentNormal) && currentDistance < d)
                {
                    d = currentDistance;
                    hp = currentHitPoint;
                    n = currentNormal;
                }
            }

            if (d < std::numeric_limits<float>::max())
            {
                distance = d;
                hitPoint = hp;
                normal = n;
                return true;
            }
        }
    }
    return false;
}

bool Mesh::intersect(const vec3& position, const vec3& direction, float& distance, vec3& hitPoint, vec3& normal) const
{
#if 0
    {
        //Debugging old way of doing things
        distance = std::numeric_limits<float>::max();
        float currentDistance;
        vec3 currentHitPoint;
        vec3 currentNormal;
        for (Triangle* t : triangles)
        {
            if (intersectTriangle(position, direction, *t, currentDistance, currentHitPoint, currentNormal) && currentDistance < distance)
            {
                distance = currentDistance;
                hitPoint = currentHitPoint;
                normal = currentNormal;
            }
        }

        return distance < std::numeric_limits<float>::max();
    }
#endif

    if (bvh)
        return bvh->intersect(position, direction, distance, hitPoint, normal);
    return false;
}

Mesh::~Mesh()
{
    clear();
}

void Mesh::clear()
{
    if (bvh)
    {
        delete bvh;
        bvh = nullptr;
    }

    for (Triangle* triangle : triangles)
    {
        delete triangle;
    }
    triangles.clear();
}

void Mesh::set(const std::vector<vec3> vertices)
{
    if (vertices.size() % 3 != 0)
    {
        std::cout << "Error during Mesh set : Invalid number of vertices. Must be a multiple of 3 " << std::endl;
        return;
    }

    //Clear older data
    clear();

    //Preallocate memory space
    triangles.reserve(vertices.size() / 3);

    //transform vertices into Triangles
    int triangle = 0;
    while (triangle < vertices.size())
    {
        Triangle* t = new Triangle();
        t->vertex0 = vertices[triangle++];
        t->vertex1 = vertices[triangle++];
        t->vertex2 = vertices[triangle++];
        triangles.push_back(t);
    }

    bvh = new MeshBVH(triangles, 0);
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

    std::vector<vec3> vertices;



    
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
                unsigned int i0 = 1;       //first
                unsigned int i1 = fan - 1; //previous
                unsigned int i2 = fan - 0; //last

                vertices.push_back(positions[i0]);
                vertices.push_back(positions[i1]);
                vertices.push_back(positions[i2]);
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
    set(vertices);
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

    std::vector<vec3> vertices;

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

            //Face index parsing
            if (header == "f")
            {
                unsigned int v;
                
                //First vertex
                if (stream >> v)
                {
                    vertices.push_back(positions[v - 1]);
                } else {
                    std::cout << "Failed to read the first vertex" << std::endl;
                }

                while (!std::isspace(stream.peek())) stream.ignore();

                //Second vertex
                if (stream >> v)
                {
                    vertices.push_back(positions[v - 1]);
                } else {
                    std::cout << "Failed to read the second vertex" << std::endl;
                }

                while (!std::isspace(stream.peek())) stream.ignore();

                //Third vertex
                if (stream >> v)
                {
                    vertices.push_back(positions[v - 1]);
                } else {
                    std::cout << "Failed to read the third vertex" << std::endl;
                }
                continue;
            }
        }
    }
    file.close();

    normalize_vertices(vertices);
    set(vertices);
}

void normalize_vertices(std::vector<vec3>& vertices)
{
    vec3 centroid = vec3();
    float area = 0;


    int triangle = 0;
    while (triangle < vertices.size())
    {
        vec3 vertex0 = vertices[triangle++];
        vec3 vertex1 = vertices[triangle++];
        vec3 vertex2 = vertices[triangle++];
        vec3 edge1 = vertex1 - vertex0;
        vec3 edge2 = vertex2 - vertex0;

        //Compute triangle Centroid & Area
        vec3 tCentroid = (vertex0 + vertex1 + vertex2) / 3.f;
        float tArea = 0.5 * edge1.cross(edge2).length();

        //Sum
        centroid += tArea * tCentroid;
        area += tArea;
    }
    centroid /= area;

    //Center Mesh around origin
    for (vec3& vertex : vertices)
    {
        vertex -= centroid;
    }

    //Compute the max X, Y and Z
    vec3 max = vec3(std::numeric_limits<float>::min());
    for (vec3& vertex : vertices)
    {
        //no matter if it's "min" or "max" we only care about distance from the origin because we're centered around it
        max.x = std::max(max.x, std::abs(vertex.x));
        max.y = std::max(max.y, std::abs(vertex.y));
        max.z = std::max(max.z, std::abs(vertex.z));
    }

    //Normalize if possible
    if (max.length2() > 0)
    {
        float scale = 1.f / max.length();
        for (vec3& vertex : vertices)
        {
            vertex *= scale;
        }
    }
}
