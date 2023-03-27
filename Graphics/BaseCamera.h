#pragma once
#include "glm/common.hpp"

class BaseCamera
{
public:
    BaseCamera();
    ~BaseCamera();

    virtual void update(float deltaTime);
    glm::vec2 GetPosition() { return m_position; }

    glm::mat4 BaseCamera::getViewMatrix();
    glm::mat4 BaseCamera::getProjectionMatrix(float w, float h);

protected:
    float m_theta;
    float m_phi;
    glm::vec3 m_position;

    float m_lastMouseX;
    float m_lastMouseY;
};