#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct GLFWwindow;

struct FrustumPlane
{
    glm::vec3 Normal;
    float Distance;
};

class Camera
{
public:

    Camera();

    void Update(GLFWwindow* window, float dt);

    glm::mat4 GetViewMatrix() const;

    glm::mat4 GetProjectionMatrix(float aspectRatio, float nearPlane = 0.1f, float farPlane = 1000.0f) const;

    void UpdateFrustum(const glm::mat4& viewProjection);

public:

    glm::vec3 Position;

    float Yaw;
    float Pitch;

    float MoveSpeed;
    float MouseSensitivity;
    float FOV;

    FrustumPlane Frustum[6];

private:

    void UpdateVectors();

private:

    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 Up;

    glm::vec3 WorldUp;

    bool FirstMouse;

    double LastMouseX;
    double LastMouseY;

    bool MouseCaptured;
};