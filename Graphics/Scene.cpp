#include "Scene.h"
#include "Instance.h"

#include "BaseCamera.h"
#include <imgui.h>
#include <string>
#include <Gizmos.h>

Scene::Scene(BaseCamera* camera, glm::vec2 windowSize, 
	Light& light, glm::vec3 ambientLightColor) :
	m_camera(camera), m_windowSize(windowSize), m_light(light), 
	m_ambientLightColor(ambientLightColor)
{

}

Scene::~Scene()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		delete* it;
	}
}

void Scene::AddInstance(Instance* instance)
{
	m_instances.push_back(instance);
}

void Scene::ClearInstance()
{
	m_instances.clear();
}

void Scene::Draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColors[i] = m_pointLights[i].color;

		aie::Gizmos::addSphere(m_pointLights[i].direction, 0.5, 10, 10, glm::vec4(m_pointLights[i].color, 0.5));
	}

	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}
}

void Scene::ImGUI()
{
	ImGui::Begin("Light Settings");
	if (ImGui::CollapsingHeader("SunLight"))
	{
		ImGui::DragFloat3("Sun Color",
			&m_light.color[0], 0.01, 0, 1);
		ImGui::DragFloat3("Sun Direction",
			&m_light.direction[0], 1, -100, 100);
	}

	for (int i = 0; i < m_pointLights.size(); i++)
	{
		std::string label = "Light " + i;
		if (ImGui::CollapsingHeader(label.c_str()))
		{
			label = "Light Color " + i;
			ImGui::DragFloat3(label.c_str(),
				&m_pointLights[i].color[0], 0.01, 0, 1);
			label = "Light Position " + i;
			ImGui::DragFloat3(label.c_str(),
				&m_pointLights[i].direction[0], 1, -100, 100);
		}
	}
	ImGui::End();

	ImGui::Begin("Texture Settings");
	if (ImGui::CollapsingHeader("Ambient Light"))
	{
		ImGui::DragFloat3("Ambient Light",
			&m_ambientLightColor[0], 0.1, 0, 100);

		//ImGui::DragFloat("Specular Strength",
		//	&m_specularStrength, 0.1, 0, 100);
	}
	ImGui::End();

}

glm::vec2 Scene::GetWindowSize()
{
	return m_windowSize;
}
