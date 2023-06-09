#pragma once
#include "glm/common.hpp"

class SimpleCamera
{
public:
    SimpleCamera();
    ~SimpleCamera();

    void update(float deltaTime);
    glm::vec2 GetPosition() { return m_position; }

    glm::mat4 SimpleCamera::getViewMatrix();
    glm::mat4 SimpleCamera::getProjectionMatrix(float w, float h);


private:
    float m_theta;
    float m_phi;
    glm::vec3 m_position;

    float m_lastMouseX;
    float m_lastMouseY;
};