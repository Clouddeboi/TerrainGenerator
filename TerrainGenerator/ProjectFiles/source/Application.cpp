#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../headers/Application.hpp"
#include <iostream>


Application::Application(
    int width,
    int height,
    const char* title)
    : m_Width(width),
    m_Height(height),
    m_Title(title)
{
    Init();
}

Application::~Application()
{
    Shutdown();
}

void Application::Init()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(
        m_Width,
        m_Height,
        m_Title,
        nullptr,
        nullptr);

    if (!m_Window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }

    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader(
        (GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, m_Width, m_Height);

    std::cout << "OpenGL: "
        << glGetString(GL_VERSION)
        << std::endl;

    m_Shader = new Shader(
        "ProjectFiles/Assets/Shaders/Vert/Test.vert",
        "ProjectFiles/Assets/Shaders/Frag/Test.frag"
    );

    float vertices[] = {
     0.0f,  0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Application::Shutdown()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    delete m_Shader;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Application::Update(double fixedDeltaTime)
{
    static double timer = 0.0;

    timer += fixedDeltaTime;

    if (timer >= 1.0)
    {
        timer = 0.0;

        std::cout << "Tick\n";
    }
}

void Application::Render()
{
    glClearColor(
        0.1f,
        0.15f,
        0.25f,
        1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    m_Shader->Bind();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(m_Window);
}

void Application::Run()
{
    constexpr double FIXED_TIMESTEP =
        1.0 / 60.0;

    double previousTime =
        glfwGetTime();

    double accumulator = 0.0;

    while (!glfwWindowShouldClose(m_Window))
    {
        double currentTime =
            glfwGetTime();

        double frameTime =
            currentTime - previousTime;

        previousTime = currentTime;

        accumulator += frameTime;

        glfwPollEvents();

        while (accumulator >= FIXED_TIMESTEP)
        {
            Update(FIXED_TIMESTEP);

            accumulator -= FIXED_TIMESTEP;
        }

        Render();
    }
}