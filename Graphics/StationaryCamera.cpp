#include "StationaryCamera.h"

StationaryCamera::StationaryCamera()
{
	m_theta = 0;
	m_phi = 0;
	m_position = glm::vec3(-10, 2, 0);
}

StationaryCamera::~StationaryCamera()
{
}

void StationaryCamera::update(float deltaTime)
{
}
