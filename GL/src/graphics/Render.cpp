#include "Render.h"

#include <glad/glad.h>

void drawTriangles(unsigned int vertexArray, unsigned int indexBuffer, unsigned int count)
{
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}