#include "Buffer.h"

#include <glad/glad.h>

BufferLayout::BufferLayout(const std::initializer_list<VertexAttribute>& attributes)
: vertexAttributes(attributes)
{
    calculateOffsetsAndStride();
}

void BufferLayout::calculateOffsetsAndStride()
{
    size_t offset = 0;
    stride = 0;
    for (auto& attribute : vertexAttributes)
    {
        attribute.offset = offset;
        offset += attribute.size;
        stride += attribute.size;
    }
}

VertexAttribute::VertexAttribute(VertexAttributeType type, const std::string& name, bool normalized)
: type(type), name(name), normalized(normalized), size(sizeOfVertexAttribute(type)), offset(0)
{
}

unsigned int VertexAttribute::getComponentCount() const
{
    switch (type)
    {
        case VertexAttributeType::Float:    return 1;
        case VertexAttributeType::Float2:   return 2;
        case VertexAttributeType::Float3:   return 3;
        case VertexAttributeType::Float4:   return 4;

        case VertexAttributeType::Mat3:     return 3 * 3;
        case VertexAttributeType::Mat4:     return 4 * 4;

        case VertexAttributeType::Int:      return 1;
        case VertexAttributeType::Int2:     return 2;
        case VertexAttributeType::Int3:     return 3;
        case VertexAttributeType::Int4:     return 4;

        case VertexAttributeType::Bool:     return 1;
    }
    return 0;
}









VertexBuffer::VertexBuffer() : size(0), handle(0), layout()
{
    glGenBuffers(1, &handle);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &handle);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, handle);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const BufferLayout& VertexBuffer::getLayout() const
{
    return layout;
}

void VertexBuffer::defineLayout(const BufferLayout& layout)
{
    this->layout = layout;
}

void VertexBuffer::bufferData(const void* data, size_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}









IndexBuffer::IndexBuffer() : handle(0), size(0), format(GL_UNSIGNED_SHORT), count(0)
{
    glGenBuffers(1, &handle);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &handle);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::defineData(const std::vector<unsigned short> elements)
{
    const void* data = elements.data();
    count  = elements.size();
    size   = elements.size() * sizeof(unsigned short);
    format = GL_UNSIGNED_SHORT;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void IndexBuffer::defineData(const std::vector<unsigned int> elements)
{
    const void* data = elements.data();
    count  = elements.size();
    size   = elements.size() * sizeof(unsigned int);
    format = GL_UNSIGNED_INT;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}