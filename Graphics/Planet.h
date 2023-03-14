#pragma once
#include <glm/common.hpp>

class Planet {
public:
	Planet(glm::vec3 position, float speed, float radius, float time, int numOfMoons, bool hasRing, float ringSize, glm::vec4 colour);
	~Planet();

	virtual void Draw();

	// Getters
	glm::vec3 GetPosition() { return m_position; }
	float GetSpeed() { return m_speed; }
	float GetTime() { return m_time; }
	glm::vec4 GetColour() { return m_colour; }
	float GetRadius() { return m_radius; }
	int GetNumOfMoons() { return m_numOfMoons; }
	bool GetHasRing() { return m_hasRing; }
	float GetRingSize() { return m_ringSize; }

	// Setters
	void SetPosition(glm::vec3 position) { m_position = position; }
	void SetSpeed(float speed) { m_speed = speed; }
	void SetColour(glm::vec4 colour) { m_colour = colour; }
	void SetRadius(float radius) { m_radius = radius; }
	void SetNumOfMoons(int numOfMoons) { m_numOfMoons = numOfMoons; }
	void SetHasRing(bool hasRing) { m_hasRing = hasRing; }

protected:
	glm::vec3 m_position;
	float m_speed;
	float m_ringSize;
	glm::vec4 m_colour;
	float m_radius;
	int m_numOfMoons;
	bool m_hasRing;
	float m_time;
};

