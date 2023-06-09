#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <list>

class BaseCamera;
class Instance;

const int MAX_LIGHTS = 4;

struct Light
{
	Light()
	{
		direction = glm::vec3(0);
		color = glm::vec3(1);
		//position = glm::vec3(0);
	}
	Light(glm::vec3 _position, glm::vec3 _color, float _intensity) 
	{
		direction = _position;
		color = _color * _intensity;
		//position = _position;
	}
	glm::vec3 direction;
	glm::vec3 color;
	//glm::vec3 position;
};

class Scene
{
public:
	Scene(BaseCamera* camera, glm::vec2 windowSize, 
		Light& light, glm::vec3 ambientLightColor);
	~Scene();
	void AddInstance(Instance* instance);
	void ClearInstance();
	void Draw();

	void ImGUI();

	void AddPointLights(Light light) { m_pointLights.push_back(light); }
	void AddPointLights(glm::vec3 direction, glm::vec3 color, float intensity)
	{
		m_pointLights.push_back(Light(direction, color, intensity));
	}

	// Getters
	BaseCamera* GetCamera() { return m_camera; }
	glm::vec2 GetWindowSize();
	glm::vec3 GetAmbientLightColor() { return m_ambientLightColor; }
	Light GetLight() { return m_light; }
	std::vector<Light>& GetPointLights() { return m_pointLights; }
	int GetNumberOfLights() { return m_pointLights.size(); }
	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColors() { return &m_pointLightColors[0]; }

	// Setters
	BaseCamera* SetCamera(BaseCamera* camera) { return m_camera = camera; };
	void SetAmbientLightColor(glm::vec3 _color) { m_ambientLightColor = _color; }

	void SetLight(Light _light) { m_light = _light; }
	void SetLight(glm::vec3 direction, glm::vec3 color, float intensity) { m_light = Light(direction, color, intensity); }

protected:
	BaseCamera*				m_camera;
	glm::vec2				m_windowSize;

	std::list<Instance*>	m_instances;
	Light					unLight;
	std::vector<Light>		m_pointLights;
	Light					m_light;
	glm::vec3				m_ambientLightColor;

	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColors[MAX_LIGHTS];
	//glm::vec3 m_pointLightDirections[MAX_LIGHTS];
};

