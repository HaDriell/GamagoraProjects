#include "Mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Timer.h"

void normalize_vertices(std::vector<vec3>& vertices);

static inline float __compute_centroid_on_x(const Triangle* t) { return (t->vertex0.x + t->vertex1.x + t->vertex2.x) / 3.f; }
static inline float __compute_centroid_on_y(const Triangle* t) { return (t->vertex0.y + t->vertex1.y + t->vertex2.y) / 3.f; }
static inline float __compute_centroid_on_z(const Triangle* t) { return (t->vertex0.z + t->vertex1.z + t->vertex2.z) / 3.f; }
static inline float __compute_min_on_x(const Triangle* t) { return std::min(t->vertex0.x, std::min(t->vertex1.x, t->vertex2.x)); }
static inline float __compute_min_on_y(const Triangle* t) { return std::min(t->vertex0.y, std::min(t->vertex1.y, t->vertex2.y)); }
static inline float __compute_min_on_z(const Triangle* t) { return std::min(t->vertex0.z, std::min(t->vertex1.z, t->vertex2.z)); }

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

MeshBVH::MeshBVH(const std::vector<Triangle*>& triangles, int maxTriangles, int maxLevels)
{
    //Debug info
    level = maxLevels;

    //Build the Bounding Box
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

    //Check if subdivision should be done
    if (triangles.size() <=  maxTriangles || maxLevels <= 1)
    {
        //Bottom of the tree or small batch of Triangles, set this MeshBVH instance as a leaf node
        this->triangles = triangles;
        this->left = nullptr;
        this->right = nullptr;
    } else {
        //Intermediate node in the tree. Split the current vector into 2 subdivisions using the widest axis

        std::vector<Triangle*> leftTriangles;
        std::vector<Triangle*> rightTriangles;

        //Find the axis that should be cut
        float dx = box.max.x - box.min.x;
        float dy = box.max.y - box.min.y;
        float dz = box.max.z - box.min.z;
        bool X_CUT = dx >= dy && dx >= dz;
        bool Y_CUT = dy >= dx && dy >= dz && !X_CUT;
        bool Z_CUT = dz >= dx && dz >= dy && !X_CUT && !Y_CUT;

        //Split each Triangle into either left or right
        for (Triangle* t : triangles)
        {
            //Subdivide along X
            if (X_CUT)
            {
                if (__compute_min_on_x(t) <= box.min.x + dx / 2)
                    leftTriangles.push_back(t);
                else
                    rightTriangles.push_back(t);
            }
            
            //Subdivide along Y
            if (Y_CUT)
            {
                if (__compute_min_on_y(t) <= box.min.y + dy / 2)
                    leftTriangles.push_back(t);
                else
                    rightTriangles.push_back(t);
            }
            
            //Subdivide along Z
            if (Z_CUT)
            {
                if (__compute_min_on_z(t) <= box.min.z + dz / 2)
                    leftTriangles.push_back(t);
                else
                    rightTriangles.push_back(t);
            }
        }

        //Special case : cannot subdivide using this heuristic anymore (stacked triangles)
        if (leftTriangles.empty() || rightTriangles.empty())
        {
            //Behave like a botom level of the MeshBVH 
            this->triangles = triangles;
            this->left = nullptr;
            this->right = nullptr;
            return;
        }

        //Subdivide
        this->left  =  new MeshBVH(leftTriangles, maxTriangles, maxLevels - 1);
        this->right = new MeshBVH(rightTriangles, maxTriangles, maxLevels - 1);
    }
}

bool MeshBVH::intersect(const vec3& position, const vec3& direction, float& distance, vec3& hitPoint, vec3& normal) const
{
    if (intersectAABB(position, direction, box))
    {
        float minimumDistance = std::numeric_limits<float>::max();
        vec3 minimumHitPoint;
        vec3 minimumNormal;

        float currentDistance;
        vec3 currentHitPoint;
        vec3 currentNormal;

        //Hit left child
        if (left && left->intersect(position, direction, currentDistance, currentHitPoint, currentNormal))
        {
            if (currentDistance < minimumDistance)
            {
                minimumDistance = currentDistance;
                minimumHitPoint = currentHitPoint;
                minimumNormal = currentNormal;
            }
        }

        //Hit right child
        if (right && right->intersect(position, direction, currentDistance, currentHitPoint, currentNormal))
        {
            if (currentDistance < minimumDistance)
            {
                minimumDistance = currentDistance;
                minimumHitPoint = currentHitPoint;
                minimumNormal = currentNormal;
            }
        }

        //Hit Triangles
        for (Triangle* t : triangles)
        {
            if (intersectTriangle(position, direction, *t, currentDistance, currentHitPoint, currentNormal))
            {
                if (currentDistance < minimumDistance)
                {
                    minimumDistance = currentDistance;
                    minimumHitPoint = currentHitPoint;
                    minimumNormal = currentNormal;
                }
            }
        }

        //Hit Result
        if (minimumDistance < std::numeric_limits<float>::max())
        {
            distance = minimumDistance;
            hitPoint = minimumHitPoint;
            normal = minimumNormal;
            return true;
        }
    }
    return false;
}

AABB MeshBVH::get_bounding_box() const
{
    return box;
}


vec3 Mesh::get_centroid() const
{
    vec3 centroid = vec3();
    float area = 0;

    for (const Triangle* t : triangles)
    {
        vec3 edge1 = t->vertex1 - t->vertex0;
        vec3 edge2 = t->vertex2 - t->vertex0;

        //Compute triangle Centroid & Area
        vec3 tCentroid = (t->vertex0 + t->vertex1 + t->vertex2) / 3.f;
        float tArea = 0.5 * edge1.cross(edge2).length();

        //Sum
        centroid += tArea * tCentroid;
        area += tArea;
    }
    centroid /= area;
    return centroid;
}

AABB Mesh::get_bounding_box() const
{
    if (bvh)
        return bvh->get_bounding_box();
    return AABB();
}

bool Mesh::intersect(const vec3& position, const vec3& direction, float& distance, vec3& hitPoint, vec3& normal) const
{
//Legacy render
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
#else

    if (!bvh)
        return false;

    return bvh->intersect(position, direction, distance, hitPoint, normal);
#endif
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

    //Update metrics
    metrics.triangleCount = triangles.size();

    Timer bvhBuildTimer;
    bvhBuildTimer.reset();

    //Compute BVH
    bvh = new MeshBVH(triangles, 0);

    //Update metrics
    metrics.bvhBuildTime = bvhBuildTimer.elapsed();
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
        stream >> std::ws;

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
