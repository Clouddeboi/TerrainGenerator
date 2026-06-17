#include "../headers/Camera.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

Camera::Camera()
{
    Position = {0.0f, 2.0f, 8.0f};

    Yaw = -90.0f;
    Pitch = 0.0f;

    MoveSpeed = 10.0f;
    MouseSensitivity = 0.1f;
    FOV = 70.0f;

    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    FirstMouse = true;

    LastMouseX = 0.0;
    LastMouseY = 0.0;

    MouseCaptured = false;

    UpdateVectors();
}

void Camera::Update(GLFWwindow* window, float dt)
{
    float velocity =
        MoveSpeed * dt;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Position += Front * velocity;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position -= Front * velocity;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position -= Right * velocity;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += Right * velocity;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        Position += WorldUp * velocity;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) 
    {
        Position -= WorldUp * velocity;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (!MouseCaptured)
        {
            glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            MouseCaptured = true;
            FirstMouse = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        MouseCaptured = false;
    }

    if (!MouseCaptured)
        return;

    double mouseX;
    double mouseY;

    glfwGetCursorPos( window, &mouseX, &mouseY);

    if (FirstMouse)
    {
        LastMouseX = mouseX;
        LastMouseY = mouseY;

        FirstMouse = false;
    }

    float xOffset = static_cast<float>(mouseX - LastMouseX);
    float yOffset = static_cast<float>(LastMouseY - mouseY);

    LastMouseX = mouseX;
    LastMouseY = mouseY;

    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    Pitch = std::clamp( Pitch, -89.0f, 89.0f);

    UpdateVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt( Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio, float nearPlane, float farPlane) const
{
    return glm::perspective(glm::radians(FOV), aspectRatio, nearPlane, farPlane);
}

void Camera::UpdateVectors()
{
    glm::vec3 front;

    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    front.y = sin(glm::radians(Pitch));

    front.z = sin(glm::radians(Yaw))* cos(glm::radians(Pitch));

    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));

    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::UpdateFrustum(const glm::mat4& vp)
{
    glm::vec4 planes[6];

    planes[0] =
        glm::vec4(
            vp[0][3] + vp[0][0],
            vp[1][3] + vp[1][0],
            vp[2][3] + vp[2][0],
            vp[3][3] + vp[3][0]);

    planes[1] =
        glm::vec4(
            vp[0][3] - vp[0][0],
            vp[1][3] - vp[1][0],
            vp[2][3] - vp[2][0],
            vp[3][3] - vp[3][0]);

    planes[2] =
        glm::vec4(
            vp[0][3] + vp[0][1],
            vp[1][3] + vp[1][1],
            vp[2][3] + vp[2][1],
            vp[3][3] + vp[3][1]);

    planes[3] =
        glm::vec4(
            vp[0][3] - vp[0][1],
            vp[1][3] - vp[1][1],
            vp[2][3] - vp[2][1],
            vp[3][3] - vp[3][1]);

    planes[4] =
        glm::vec4(
            vp[0][3] + vp[0][2],
            vp[1][3] + vp[1][2],
            vp[2][3] + vp[2][2],
            vp[3][3] + vp[3][2]);

    planes[5] =
        glm::vec4(
            vp[0][3] - vp[0][2],
            vp[1][3] - vp[1][2],
            vp[2][3] - vp[2][2],
            vp[3][3] - vp[3][2]);

    for (int i = 0; i < 6; i++)
    {
        glm::vec3 normal =
            glm::vec3(
                planes[i].x,
                planes[i].y,
                planes[i].z);

        float length = glm::length(normal);

        Frustum[i].Normal = normal / length;

        Frustum[i].Distance = planes[i].w / length;
    }
}