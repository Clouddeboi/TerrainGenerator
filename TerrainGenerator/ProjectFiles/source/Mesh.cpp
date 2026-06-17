#include <glad/glad.h>

#include "../headers/Mesh.hpp"

Mesh::Mesh()
{
}

//Destructor, glDelete* silently ignores 0, so safe even if Upload was never called
Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

//Upload vertex and index data to the GPU
//GPU objects are created here, not in the constructor, so Mesh is safe
//to hold as a member before OpenGL is initialised
void Mesh::Upload(const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices)
{
    m_IndexCount = static_cast<unsigned int>(indices.size());

    //Create GPU objects on first upload
    if (m_VAO == 0)
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
    }

    glBindVertexArray(m_VAO);

    //Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW);

    //Upload index data, stored in VAO state while VAO is bound
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW);

    //VAO stays bound, caller sets up glVertexAttribPointer then calls Unbind()
}

void Mesh::Bind() const
{
    glBindVertexArray(m_VAO);
}

void Mesh::Unbind() const
{
    glBindVertexArray(0);
}

//Draw indexed geometry
void Mesh::Draw() const
{
    glBindVertexArray(m_VAO);

    glDrawElements(
        GL_TRIANGLES,
        m_IndexCount,
        GL_UNSIGNED_INT,
        0);
}