#include "FlyCamera.h"
#include "Input.h"
#include <glm/detail/func_trigonometric.hpp>
#include <iostream>

FlyCamera::FlyCamera()
{
    m_theta = 0;
    m_phi = 0;
    m_position = glm::vec3(-10, 2, 0);
}

FlyCamera::~FlyCamera()
{
}

void FlyCamera::update(float deltaTime)
{
    aie::Input* input = aie::Input::getInstance();
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);

    /*std::cout << "Phi: \n" << m_phi;
    std::cout << "Theta: \n" << m_theta;*/

    //std::cout << "Pos: " << m_position.y;

    //calculate the forwards and right axes and up axis for the camera
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
    glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
    glm::vec3 up(0, 1, 0);

    // use WASD, ZX keys to move camera around
    if (input->isKeyDown(aie::INPUT_KEY_X))
        m_position += up * deltaTime;

    // get the current mouse coordinates
    float mx = input->getMouseX();
    float my = input->getMouseY();
    const float turnSpeed = 0.1f;
    // if the right button is down, increment theta and phi
    if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
    {
        m_theta += turnSpeed * (mx - m_lastMouseX);
        m_phi += turnSpeed * (my - m_lastMouseY);
    }
    if (input->isKeyDown(aie::INPUT_KEY_W)) // Forward
    {
        m_position += forward * 0.1f;
    }
    if (input->isKeyDown(aie::INPUT_KEY_S)) // Backwards
    {
        m_position -= forward * 0.1f;
    }
    if (input->isKeyDown(aie::INPUT_KEY_A)) // Left
    {
        m_position -= right * 0.1f;
    }
    if (input->isKeyDown(aie::INPUT_KEY_D)) // Right
    {
        m_position += right * 0.1f;
    }
    if (input->isKeyDown(aie::INPUT_KEY_SPACE)) // Up
    {
        m_position += up * 0.1f;
    }
    if (input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL)) // Down
    {
        m_position -= up * 0.1f;
    }
    // store this frames values for next framem_lastMouseX = mx;m_lastMouseY = my;
    m_lastMouseX = mx;
    m_lastMouseY = my;
}
