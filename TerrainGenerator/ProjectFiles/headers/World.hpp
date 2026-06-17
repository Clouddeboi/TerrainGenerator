#pragma once

#include "../headers/Camera.hpp"
#include "../headers/Shader.hpp"
#include "../headers/Mesh.hpp"
#include "../headers/Framebuffer.hpp"
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

    void CreateCompositeQuad();

private:

    GLFWwindow* m_Window;

    Camera m_Camera;

	Mesh m_Mesh;

    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Shader> m_CompositeShader;

    Framebuffer m_Framebuffer;
    Mesh m_CompositeQuad;
};