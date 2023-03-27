#include "BaseCamera.h"
#include "glm/gtc/matrix_transform.hpp"

BaseCamera::BaseCamera()
{
}

BaseCamera::~BaseCamera()
{
}

void BaseCamera::update(float deltaTime)
{
}

glm::mat4 BaseCamera::getViewMatrix()
{
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
    return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 BaseCamera::getProjectionMatrix(float w, float h)
{
    return glm::perspective(glm::pi<float>() * 0.25f,
        w / h,
        0.1f, 1000.f);
}