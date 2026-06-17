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
}