#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Planet.h"

#include "imgui.h"

#include "SimpleCamera.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsApp::GraphicsApp() {

}

GraphicsApp::~GraphicsApp() {

}

bool GraphicsApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);




	m_bunny = false;
	m_solarsystem = false;
	m_soulSpear = false;
	m_plain = false;

	Light light;
	light.color = { 4, 3, 5 };
	light.direction = { 1, -1, 1 };
	m_ambientLight = { 30.f, 30.f, 30.f };
	m_specularStrength = 0.f;

	m_scene = new Scene(&m_camera, glm::vec2(getWindowWidth(),
		getWindowHeight()), light, m_ambientLight);

	return LaunchShaders();
}

void GraphicsApp::shutdown() {

	Gizmos::destroy();
	delete m_scene;
}

void GraphicsApp::update(float deltaTime) {

	m_camera.update(deltaTime);

	// query time since application started
	float time = getTime();

	// rotate camera
	m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, 10, glm::cos(time) * 10),
		vec3(0), vec3(0, 1, 0));

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));
	
	if (m_solarsystem) SolarSystem(time);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();


	// Rotate the light to emulate a 'day/night' cycle
	m_light.direction = 
		glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));


	ImGUIRefesher();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = m_camera.getProjectionMatrix(getWindowWidth(), getWindowHeight()); 
	m_viewMatrix = m_camera.getViewMatrix();

	auto pv = m_projectionMatrix * m_viewMatrix;

	m_scene->Draw();

	//if (m_soulSpear) ObjDraw(pv, m_spearTransform, &m_spearMesh);
	//if (m_bunny) ObjDraw(pv, m_bunnyTransform, &m_bunnyMesh);
	
	//if (m_plain) ObjDraw(pv, m_quadTransform, &m_quadMesh);

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

void GraphicsApp::SolarSystem(float time)
{
	Planet* sun = new Planet(vec3(0), 0, time, 1, 0, false, 0, vec4(250, 255, 0, 0.5));
	Planet* mercury = new Planet(vec3(1.75, 0, 1.75), 4.787f, time, 0.15, 0, false, 0, vec4(189, 205, 214, 0.5));
	Planet* venus = new Planet(vec3(3, 0, 3), 3.502f, time, 0.3, 0, false, 0, vec4(255, 139, 19, 0.5));
	Planet* earth = new Planet(vec3(4.25, 0, 4.25), 2.978f, time, 0.3, 0, false, 0, vec4(0, 146, 255, 0.5));
	Planet* mars = new Planet(vec3(5.5, 0, 5.5), 2.4077f, time, 0.2, 0, false, 0, vec4(255, 17, 0, 0.5));
	Planet* jupiter = new Planet(vec3(6.75, 0, 6.75), 1.307f, time, 0.4, 0, true, 0.09, vec4(242, 167, 90, 0.5));
	Planet* saturn = new Planet(vec3(8, 0, 8), 0.969, time, 0.4, 0, true, 0.13, vec4(255, 203, 47, 0.5)); // large
	Planet* uranus = new Planet(vec3(9.25, 0, 9.25), 0.681, time, 0.3, 0, true, 0.1, vec4(135, 192, 245, 0.5));
	Planet* neptune = new Planet(vec3(10.5, 0, 10.5), 0.543, time, 0.3, 0, true, 0.09, vec4(35, 149, 255, 0.5));

	sun->Draw();
	mercury->Draw();
	venus->Draw();
	earth->Draw();
	mars->Draw();
	jupiter->Draw();
	saturn->Draw();
	uranus->Draw();
	neptune->Draw();
}

bool GraphicsApp::LaunchShaders()
{
	m_normalLitShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/normalLit.vert");
	m_normalLitShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/normalLit.frag");

	if (m_normalLitShader.link() == false)
	{
		printf("Normal Lit Phong Shader Error: %s\n", m_normalLitShader.getLastError());
		return false;
	}

	// Used for loading in a simple quad
	if (!QuadLoader())
		return false;

	// Used for loading in an OBJ bunny
	if (!BunnyLoader())
		return false;

	if (!SpearLoader())
		return false;

	m_scene->AddInstance(new Instance(m_spearTransform, 
		&m_spearMesh, &m_normalLitShader));

	return true;
}

void GraphicsApp::ImGUIRefesher()
{
	ImGui::Begin("Light Settings");
	ImGui::DragFloat3("Global Light Color", 
		&m_light.color[0], 0.1, 0, 100);
	ImGui::DragFloat3("Global Light Direction", 
		&m_light.direction[0], 0.1, -1, 100);
	ImGui::DragFloat3("Ambient Light",
		&m_ambientLight[0], 0.1, 0, 100);
	ImGui::DragFloat("Specular Strength",
		&m_specularStrength, 1, 0, 100);
	ImGui::End();

	ImGui::Begin("Mesh Selector");
	ImGui::Checkbox("Solar System", &m_solarsystem);
	ImGui::Checkbox("Bunny", &m_bunny);
	ImGui::Checkbox("Soul Spear", &m_soulSpear);
	ImGui::Checkbox("Plain", &m_plain);
	ImGui::End();

	ImGui::Begin("Camera Selector/Settings");

	ImGui::End();
}

bool GraphicsApp::QuadLoader()
{
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");

	if (m_simpleShader.link() == false)
	{
		printf("Simple Shader has an Error %s|\n",
			m_simpleShader.getLastError());
		return false;
	}

	// Defined as 4 vertices for the 2 triangles
	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0,  0.5f, 1 };
	vertices[1].position = { 0.5f, 0,  0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	vertices[3].position = { 0.5f, 0, -0.5f, 1 };

	unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

	m_quadMesh.Initialise(4, vertices, 6, indices);

	// This is a 10 'unit' wide quad
	m_quadTransform = {
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1
	};

	return true;
}

void GraphicsApp::QuadDraw(glm::mat4 pvm)
{
	// Bind the shader
	m_simpleShader.bind();

	// Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	// Draw the quad using Mesh's draw
	m_quadMesh.Draw();
}

bool GraphicsApp::BunnyLoader()
{
	m_phongShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/phong.frag");

	if (m_phongShader.link() == false) {
		printf("Color Shader Error: &s\n", m_phongShader.getLastError());
		return false;
	}

	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny Mesh Error!\n");
		return false;
	}

	m_bunnyTransform = {
		0.5, 0, 0, 0,
		0, 0.5, 0, 0,
		0, 0, 0.5, 0,
		0, 0, 0, 1
	};
	return true;
}

void GraphicsApp::BunnyDraw(glm::mat4 pvm)
{
	// Bind the shader
	m_colorShader.bind();

	// Bind the transform
	m_colorShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the color
	m_colorShader.bindUniform("BaseColor", glm::vec4(1));

	m_bunnyMesh.draw();
}

bool GraphicsApp::BoxLoading()
{
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");

	if (m_simpleShader.link() == false)
	{
		printf("Simple Shader has an Error %s|\n",
			m_simpleShader.getLastError());
		return false;
	}

	// Defined as 4 vertices for the 2 triangles
	Mesh::Vertex vertices[8];
	vertices[0].position = { -0.5f, 0,  0.5f, 1 };
	vertices[1].position = {  0.5f, 0,  0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	vertices[3].position = {  0.5f, 0, -0.5f, 1 };
	vertices[4].position = { -0.5f, 1,  0.5f, 1 };
	vertices[5].position = {  0.5f, 1,  0.5f, 1 };
	vertices[6].position = { -0.5f, 1, -0.5f, 1 };
	vertices[7].position = {  0.5f, 1, -0.5f, 1 };

	unsigned int indices[36] = { 
		0, 1, 2, 2, 1, 3,
		4, 0, 6, 6, 0, 2,
		5, 1, 4, 4, 1, 0,
		7, 3, 5, 5, 3, 1,
		6, 2, 7, 7, 2, 3,
		7, 5, 6, 6, 5, 4
	};

	m_quadMesh.Initialise(8, vertices, 36, indices);

	// This is a 10 'unit' wide quad
	m_quadTransform = {
		10, 0,  0,  0,
		0,  10, 0,  0,
		0,  0,  10, 0,
		0,  0,  0,  1
	};

	return true;
}

void GraphicsApp::BoxDraw(glm::mat4 pvm)
{
	// Bind the shader
	m_simpleShader.bind();

	// Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	// Draw the quad using Mesh's draw
	m_quadMesh.Draw();
}

bool GraphicsApp::SpearLoader()
{
	if (m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	m_spearTransform = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
	};

	return true;
}

void GraphicsApp::ObjDraw(glm::mat4 pv, glm::mat4 transform, aie::OBJMesh* objMesh)
{
	m_normalLitShader.bind();

	// Bind the camera position
	m_normalLitShader.bindUniform("CameraPosition",
		glm::vec3(glm::inverse(m_viewMatrix)[3]));

	// Bind the directional light we defined
	m_normalLitShader.bindUniform("LightDirection", m_light.direction);
	m_normalLitShader.bindUniform("LightColor", m_light.color);
	m_normalLitShader.bindUniform("AmbientColor", m_ambientLight);
	m_normalLitShader.bindUniform("SpecularPower", m_specularStrength);

	// Bind the texture location
	m_normalLitShader.bindUniform("diffuseTexture", 0);

	// Bind the pvm using the one provided
	m_normalLitShader.bindUniform("ProjectionViewModel", pv * transform);

	// Bind the transform using the one provided
	m_normalLitShader.bindUniform("ModelMatrix", transform);

	objMesh->draw();
}

void GraphicsApp::PhongDraw(glm::mat4 pvm, glm::mat4 transform, float time)
{
	// Bind the phong shader
	m_phongShader.bind();

	// Bind the camera position
	m_phongShader.bindUniform("CameraPosition", 
		glm::vec3(glm::inverse(m_viewMatrix)[3]));

	// Bind the directional light we defined
	m_phongShader.bindUniform("LightDirection", m_light.direction);
	m_phongShader.bindUniform("LightColor", m_light.color);
	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
	m_phongShader.bindUniform("SpecularPower", m_specularStrength);

	// Bind the pvm using the one provided
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the transform using the one provided
	m_phongShader.bindUniform("ModelMatrix", transform);

	if (m_soulSpear) m_spearMesh.draw();
	if (m_solarsystem) SolarSystem(time);
	if (m_bunny) m_bunnyMesh.draw();
	if (m_plain) m_quadMesh.Draw();
}
