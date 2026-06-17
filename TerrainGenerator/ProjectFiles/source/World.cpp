#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../headers/World.hpp"

World::World()
{
    m_Window = nullptr;
}

World::~World()
{
}

void World::Init(GLFWwindow* window)
{
    m_Window = window;

    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    std::vector<unsigned int> indices = {0, 1, 2};

    m_Mesh.Upload(vertices, indices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    m_Mesh.Unbind();

    m_Shader = std::make_unique<Shader>(
        "ProjectFiles/Assets/Shaders/Vert/Test.vert",
        "ProjectFiles/Assets/Shaders/Frag/Test.frag");
}

void World::Update(float dt)
{
    m_Camera.Update(m_Window, dt);
}

void World::Render()
{
    float aspect = 1280.0f / 720.0f;

    glm::mat4 view = m_Camera.GetViewMatrix();

    glm::mat4 projection = m_Camera.GetProjectionMatrix(aspect);

    m_Shader->Bind();
    m_Shader->SetMat4("u_View", glm::value_ptr(view));
    m_Shader->SetMat4("u_Projection", glm::value_ptr(projection));

    m_Mesh.Draw();
}