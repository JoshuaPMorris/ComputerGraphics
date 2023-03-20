#include "Camera.h"
#include "glm/geometric.hpp"
#include "glm/detail/func_trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../dependencies/glfw/include/GLFW/glfw3.h"

void Camera::update(float deltaTime, GLFWwindow* window)
{
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);

    //calculate the forwards and right axes and up axis for the camera
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
    glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
    glm::vec3 up(0, 1, 0);

    // use WASD, ZX keys to move camera around
    if (glfwGetKey(window, GLFW_KEY_X))
        m_position += up * deltaTime;
}

glm::mat4 Camera::getViewMatrix()
{
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
    return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjectionMatrix(float w, float h)
{
    return glm::perspective(glm::pi<float>() * 0.25f,
        w / h,
        0.1f, 1000.f);
}
