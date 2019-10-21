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









VertexBuffer::VertexBuffer() : handle(0) {}

VertexBuffer::VertexBuffer(float* vertices, size_t count)
{
    size_t buffer_size = sizeof(float) * count;
    glGenBuffers(1, &handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, vertices, GL_STATIC_DRAW);
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

void VertexBuffer::setLayout(const BufferLayout& layout)
{
    this->layout = layout;
}









IndexBuffer::IndexBuffer() : handle(0), count(0) {}

IndexBuffer::IndexBuffer(unsigned int* indices, size_t count) : count(count)
{
    size_t buffer_size = sizeof(unsigned int) * count;
    glGenBuffers(1, &handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_size, indices, GL_STATIC_DRAW);
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