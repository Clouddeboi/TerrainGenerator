#pragma once

#include <vector>

/*
    Mesh class

    Responsibilities:
    - Wrap VAO, VBO, EBO (index buffer)
    - Upload vertex and index data to the GPU
    - Draw indexed geometry (glDrawElements)
    - No vertex format assumption, caller configures glVertexAttribPointer
      after Upload() while the VAO is still bound, then calls Unbind()
*/

class Mesh
{
public:
    Mesh();

    //Destructor, cleans up OpenGL buffers
    ~Mesh();

    //Prevent copying, owns GPU resources
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    //Upload raw vertex and index data to the GPU
    //VAO remains bound after this call, configure vertex attributes then call Unbind()
    void Upload(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices);

    //Bind/unbind the VAO
    void Bind() const;
    void Unbind() const;

    //Draw indexed geometry
    void Draw() const;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;

    unsigned int m_IndexCount = 0;
};