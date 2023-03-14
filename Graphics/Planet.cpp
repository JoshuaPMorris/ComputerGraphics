#include "Planet.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>


Planet::Planet(glm::vec3 position, float speed, float time, float radius, int numOfMoons, bool hasRing, float ringSize, glm::vec4 colour) {
	m_position = position;
	m_speed = speed;
	m_radius = radius;
	m_time = time;
	m_numOfMoons = numOfMoons;
	m_hasRing = hasRing;
	m_ringSize = ringSize;
	m_colour = colour;
}

Planet::~Planet() {
}

void Planet::Draw()
{
	aie::Gizmos::addSphere(glm::vec3(glm::sin(m_time * m_speed) * m_position.x, m_position.y, glm::cos(m_time * m_speed) * m_position.z), m_radius, 10, 10, m_colour);
	if (GetHasRing() == true)
	{
		// Draw ring around the planet at 1.15x radius
		aie::Gizmos::addRing(glm::vec3(glm::sin(m_time * m_speed) * m_position.x, m_position.y, glm::cos(m_time * m_speed) * m_position.z), m_position.x * 0.08, m_position.x * m_ringSize, 100, m_colour);
	}
	if (GetNumOfMoons() > 0)
	{
		for (int i = 0; i <= GetNumOfMoons(); i++)
		{

		}
	}
}
