#pragma once

#include <vector>
#include <memory>

#include "Buffer.h"

class VertexArray
{
private:
    unsigned int handle;
    unsigned int vertexAttributePointer;
    std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;

public:
    VertexArray();
    ~VertexArray();
    
    void bind() const;
    void unbind() const;

    void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);

    const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const { return vertexBuffers; }
};