#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

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

    //Get initial window size
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);

    //Ensure window is not minimized
    if (windowWidth <= 0 || windowHeight <= 0)
    {
        windowWidth = 1280;
        windowHeight = 720;
    }

    //Create pixel framebuffer
    m_Framebuffer.Create(windowWidth, windowHeight);

    //Hard-coded triangle for pipeline sanity check
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    std::vector<unsigned int> indices = { 0, 1, 2 };

    m_Mesh.Upload(vertices, indices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    m_Mesh.Unbind();

    m_Shader = std::make_unique<Shader>(
        "ProjectFiles/assets/shaders/vert/Test.vert",
        "ProjectFiles/assets/shaders/frag/Test.frag");

    //Create composite shader and fullscreen quad
    m_CompositeShader = std::make_unique<Shader>(
        "ProjectFiles/assets/shaders/vert/Composite.vert",
        "ProjectFiles/assets/shaders/frag/Composite.frag");

    CreateCompositeQuad();
}

void World::CreateCompositeQuad()
{
    //Fullscreen quad in NDC (normalized device coordinates)
    std::vector<float> quadVertices = {
        //Position           TexCoord
        -1.0f,  1.0f,       0.0f, 1.0f,   //Top-left
         1.0f,  1.0f,       1.0f, 1.0f,   //Top-right
        -1.0f, -1.0f,       0.0f, 0.0f,   //Bottom-left
         1.0f, -1.0f,       1.0f, 0.0f    //Bottom-right
    };

    std::vector<unsigned int> quadIndices = { 0, 1, 2, 1, 3, 2 };

    m_CompositeQuad.Upload(quadVertices, quadIndices);

    //Position attribute (location 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //TexCoord attribute (location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
        (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    m_CompositeQuad.Unbind();
}

void World::Update(float dt)
{
    m_Camera.Update(m_Window, dt);
}

void World::Render()
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);

    //Guard against minimized window
    if (windowWidth <= 0 || windowHeight <= 0)
        return;

    //Check if window was resized, recreate FBO if needed
    if (m_Framebuffer.NeedsRecreate(windowWidth, windowHeight))
    {
        m_Framebuffer.Create(windowWidth, windowHeight);
    }

    //SCENE PASS: Render to pixel framebuffer
    m_Framebuffer.Bind();

    glClearColor(0.1f, 0.15f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Compute aspect ratio from FBO dimensions (guaranteed valid)
    float aspect = static_cast<float>(m_Framebuffer.GetPixelWidth()) /
        static_cast<float>(m_Framebuffer.GetPixelHeight());

    glm::mat4 view = m_Camera.GetViewMatrix();
    glm::mat4 projection = m_Camera.GetProjectionMatrix(aspect);

    glm::mat4 viewProjection = projection * view;
    m_Camera.UpdateFrustum(viewProjection);

    m_Shader->Bind();
    m_Shader->SetMat4("u_View", glm::value_ptr(view));
    m_Shader->SetMat4("u_Projection", glm::value_ptr(projection));

    m_Mesh.Draw();

    //COMPOSITE PASS: Blit to screen with GL_NEAREST
    m_Framebuffer.Unbind();
    glViewport(0, 0, windowWidth, windowHeight);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Disable depth testing for 2D composite pass
    glDisable(GL_DEPTH_TEST);

    m_CompositeShader->Bind();
    glActiveTexture(GL_TEXTURE0);

    //Use the proper accessor to get the colour texture
    glBindTexture(GL_TEXTURE_2D, m_Framebuffer.GetColourTexture());
    m_CompositeShader->SetInt("u_ScreenTexture", 0);

    m_CompositeQuad.Draw();

    glEnable(GL_DEPTH_TEST);
}