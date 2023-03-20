#pragma once
#include "glm/common.hpp"

class Camera
{
public:

    void update(float deltaTime, GLFWwindow* window);

    glm::mat4 Camera::getViewMatrix();
    glm::mat4 Camera::getProjectionMatrix(float w, float h);


private:
    float m_theta;
    float m_phi;
    glm::vec3 m_position;
};