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

glm::mat4 BaseCamera::GetTransform(glm::vec3 position, glm::vec3 eularAngles, glm::vec3 scale)
{
    return glm::translate(glm::mat4(1), position)
        * glm::rotate(glm::mat4(1),
            glm::radians(eularAngles.z), glm::vec3(0, 0, 1))
        * glm::rotate(glm::mat4(1),
            glm::radians(eularAngles.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1),
            glm::radians(eularAngles.x), glm::vec3(1, 0, 0))
        * glm::scale(glm::mat4(1), scale);
}