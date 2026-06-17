#pragma once

#include "../headers/Camera.hpp"
#include "../headers/Shader.hpp"
#include "../headers/Mesh.hpp"
#include <memory>

struct GLFWwindow;

class World
{
public:

    World();

    ~World();

    void Init(GLFWwindow* window);

    void Update(float dt);

    void Render();

private:

    GLFWwindow* m_Window;

    Camera m_Camera;

	Mesh m_Mesh;

    std::unique_ptr<Shader> m_Shader;
};