#pragma once

#include <GLFW/glfw3.h>

class Application
{
public:
    Application(int width, int height, const char* title);
    ~Application();

    void Run();

private:
    void Init();
    void Shutdown();

    void Update(double fixedDeltaTime);
    void Render();

private:
    GLFWwindow* m_Window = nullptr;

    int m_Width;
    int m_Height;
    const char* m_Title;

    bool m_Running = true;
};