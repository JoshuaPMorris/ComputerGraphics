#include "StationaryCamera.h"
#include <Input.h>
#include <glm/detail/func_trigonometric.hpp>

StationaryCamera::StationaryCamera()
{
	m_theta = 0;
	m_phi = 0;
	m_position = glm::vec3(-10, 2, 0);
}

StationaryCamera::StationaryCamera(bool xAxis, bool yAxis, bool zAxis)
{
	m_theta = 0;
	m_phi = 0;
	m_position = glm::vec3(-10, 2, 0);

	m_xAxis = xAxis;
	m_yAxis = yAxis;
	m_zAxis = zAxis;

	if (xAxis)
	{
		m_position = glm::vec3(-10, 2, 0);
		m_theta = 0;
		m_phi = 0;
	}
	if (yAxis)
	{
		m_position = glm::vec3(0, 12, 0);
		m_theta = 0;
		m_phi = -90;
	}
	if (zAxis)
	{
		m_position = glm::vec3(0, 2, 10);
		m_theta = -90;
		m_phi = 0;
	}
}

StationaryCamera::~StationaryCamera()
{
}

void StationaryCamera::update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	//input->getMouseScroll();

	//float thetaR = glm::radians(m_theta);
	//float phiR = glm::radians(m_phi);

	//glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	//glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
	//glm::vec3 up(0, 1, 0);

	if (m_xAxis)
	{
		if (input->isKeyDown(aie::INPUT_KEY_W)) // Forward
		{
			m_position += glm::vec3(4, 0, 0) * 0.1f;
		}
		if (input->isKeyDown(aie::INPUT_KEY_S)) // Backwards
		{
			m_position += glm::vec3(-4, 0, 0) * 0.1f;
		}
	}
	if (m_yAxis)
	{
		if (input->isKeyDown(aie::INPUT_KEY_W)) // Forward
		{
			m_position += glm::vec3(0, 4, 0) * 0.1f;
		}
		if (input->isKeyDown(aie::INPUT_KEY_S)) // Backwards
		{
			m_position += glm::vec3(0, -4, 0) * 0.1f;
		}
	}
	if (m_zAxis)
	{

		if (input->isKeyDown(aie::INPUT_KEY_W)) // Forward
		{
			m_position += glm::vec3(0, 0, -4) * 0.1f;
		}
		if (input->isKeyDown(aie::INPUT_KEY_S)) // Backwards
		{
			m_position += glm::vec3(0, 0, 4) * 0.1f;
		}
	}
}
