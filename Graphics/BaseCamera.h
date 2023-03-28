#pragma once
#include "glm/common.hpp"

class BaseCamera
{
public:
    BaseCamera();
    ~BaseCamera();

    virtual void update(float deltaTime);
    glm::vec3 GetPosition() { return m_position; }
    glm::mat4 GetTransform(glm::vec3 position, glm::vec3 eularAngles, glm::vec3 scale);
    float GetTheta() { return m_theta; }
    float GetPhi() { return m_phi; }

    void SetPosition(glm::vec3 pos) { m_position = pos; }
    void SetTheta(float theta) { m_theta = theta; }
    void SetPhi(float phi) { m_phi = phi; }

    glm::mat4 BaseCamera::getViewMatrix();
    glm::mat4 BaseCamera::getProjectionMatrix(float w, float h);

protected:
    float m_theta;
    float m_phi;
    glm::vec3 m_position;

    float m_lastMouseX;
    float m_lastMouseY;
};