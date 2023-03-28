#include "StationaryCamera.h"
#include <Input.h>
#include <glm/detail/func_trigonometric.hpp>
#include <Gizmos.h>

StationaryCamera::StationaryCamera()
{
	m_theta = 0;
	m_phi = 0;
	m_position = glm::vec3(-10, 2, 0);
}

StationaryCamera::StationaryCamera(bool xAxis, bool yAxis, bool zAxis, glm::vec3 pos)
{
	m_theta = 0;
	m_phi = 0;
	m_position = pos;

	m_xAxis = xAxis;
	m_yAxis = yAxis;
	m_zAxis = zAxis;

	if (xAxis)
	{
		m_theta = 0;
		m_phi = 0;
	}
	if (yAxis)
	{
		m_theta = 0;
		m_phi = -90;
	}
	if (zAxis)
	{
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
