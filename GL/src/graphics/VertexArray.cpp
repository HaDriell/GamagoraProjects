#include "VertexArray.h"

#include <glad/glad.h>

static GLenum toOpenGLType(VertexAttributeType type)
{
    switch (type)
    {
        case VertexAttributeType::Float:    return GL_FLOAT;
        case VertexAttributeType::Float2:   return GL_FLOAT;
        case VertexAttributeType::Float3:   return GL_FLOAT;
        case VertexAttributeType::Float4:   return GL_FLOAT;
        case VertexAttributeType::Mat3:     return GL_FLOAT;
        case VertexAttributeType::Mat4:     return GL_FLOAT;
        case VertexAttributeType::Int:      return GL_INT;
        case VertexAttributeType::Int2:     return GL_INT;
        case VertexAttributeType::Int3:     return GL_INT;
        case VertexAttributeType::Int4:     return GL_INT;
        case VertexAttributeType::Bool:     return GL_BOOL;
    }
    return 0;
}


VertexArray::VertexArray() : handle(0), vertexAttributePointer(0)
{
    glGenVertexArrays(1, &handle);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &handle);
}

void VertexArray::bind() const
{
    glBindVertexArray(handle);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
    glBindVertexArray(handle);

    vertexBuffer->bind();
    const BufferLayout& layout = vertexBuffer->getLayout();
    for (const VertexAttribute& attribute : layout)
    {
        //Enable & Setup Vertex Attribute
        glEnableVertexAttribArray(vertexAttributePointer);
        glVertexAttribPointer(vertexAttributePointer,
                attribute.getComponentCount(),
                toOpenGLType(attribute.type),
                attribute.normalized,
                layout.getStride(),
                (const void*) attribute.offset);

        //Push Vertex Attribute Pointer
        vertexAttributePointer++;
    }
    vertexBuffers.push_back(vertexBuffer);
}