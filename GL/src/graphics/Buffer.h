#pragma once

#include <string>
#include <vector>

enum class VertexAttributeType 
{
    None = 0,
    Float, Float2, Float3, Float4,
    Mat3, Mat4,
    Int, Int2, Int3, Int4,
    Bool
};

static unsigned int sizeOfVertexAttribute(VertexAttributeType type)
{
    switch (type)
    {
        case VertexAttributeType::Float:    return 4;
        case VertexAttributeType::Float2:   return 4 * 2;
        case VertexAttributeType::Float3:   return 4 * 3;
        case VertexAttributeType::Float4:   return 4 * 4;

        case VertexAttributeType::Mat3:     return 4 * 3 * 3;
        case VertexAttributeType::Mat4:     return 4 * 4 * 4;

        case VertexAttributeType::Int:      return 4;
        case VertexAttributeType::Int2:     return 4 * 2;
        case VertexAttributeType::Int3:     return 4 * 3;
        case VertexAttributeType::Int4:     return 4 * 4;

        case VertexAttributeType::Bool:     return 1;
    }
    return 0;
}

struct VertexAttribute
{
    std::string         name;
    VertexAttributeType type;
    unsigned int        size;
    size_t              offset;
    bool                normalized;

    VertexAttribute() = default;
    VertexAttribute(VertexAttributeType type, const std::string& name, bool normalized = false);

    unsigned int getComponentCount() const;
};

class BufferLayout
{
private:
    std::vector<VertexAttribute> vertexAttributes;
    unsigned int stride;

    void calculateOffsetsAndStride();

public:
    BufferLayout() {}
    BufferLayout(const std::initializer_list<VertexAttribute>& attributes);

    unsigned int getStride() const { return stride; }

    std::vector<VertexAttribute>::iterator begin() { return vertexAttributes.begin(); }
    std::vector<VertexAttribute>::iterator end() { return vertexAttributes.end(); }
    std::vector<VertexAttribute>::const_iterator begin() const { return vertexAttributes.begin(); }
    std::vector<VertexAttribute>::const_iterator end() const { return vertexAttributes.end(); }
};

class VertexBuffer
{
private:
    unsigned int handle;
    BufferLayout layout;

public:
    VertexBuffer();
    VertexBuffer(float* vertices, size_t count);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    const BufferLayout& getLayout() const;
    void setLayout(const BufferLayout& layout);
};

class IndexBuffer
{
private:
    unsigned int handle;
    unsigned int count;

public:
    IndexBuffer();
    IndexBuffer(unsigned int* indices, size_t count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    unsigned int getCount() { return count; }
};