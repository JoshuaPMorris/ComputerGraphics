#pragma once
#include <glm/common.hpp>

class StationaryCamera
{
public:
	StationaryCamera();
	~StationaryCamera();

	virtual void update(float deltaTime);

	glm::mat4 StationaryCamera::getViewMatrix();
	glm::mat4 StationaryCamera::getProjectionMatrix(float w, float h);

protected:
	glm::vec3 m_position;

};

