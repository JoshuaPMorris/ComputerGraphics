#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Planet.h"

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

	return LaunchShaders();
}

void GraphicsApp::shutdown() {

	Gizmos::destroy();
}

void GraphicsApp::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// rotate camera
	//m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, 10, glm::cos(time) * 10),
		//vec3(0), vec3(0, 1, 0));


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
	
	//Gizmos::addSphere(vec3(glm::sin(time * 2) * 2, 0, glm::cos(time * 2) * 2), 1, 10, 10, vec4(250, 255, 0, 0.5));

	Planet* sun =		new Planet(vec3(0),			0,		time, 1,	0,	false, 0,	vec4(250, 255, 0, 0.5));
	Planet* mercury =	new Planet(vec3(1.75, 0, 1.75),4.787f,	time, 0.15, 0,	false, 0,	vec4(189, 205, 214, 0.5));
	Planet* venus =		new Planet(vec3(3, 0, 3),	3.502f,	time, 0.3,	0,	false, 0,	vec4(255, 139, 19, 0.5));
	Planet* earth =		new Planet(vec3(4.25, 0, 4.25),2.978f,	time, 0.3,	0,	false, 0,	vec4(0, 146, 255, 0.5));
	Planet* mars =		new Planet(vec3(5.5, 0, 5.5),	2.4077f,time, 0.2,	0,	false, 0,	vec4(255, 17, 0, 0.5));
	Planet* jupiter =	new Planet(vec3(6.75, 0, 6.75),1.307f,	time, 0.4,	0,	true, 0.09,	vec4(242, 167, 90, 0.5));
	Planet* saturn =	new Planet(vec3(8, 0, 8),	0.969,	time, 0.4,	0,	true, 0.13,	vec4(255, 203, 47, 0.5)); // large
	Planet* uranus =	new Planet(vec3(9.25, 0, 9.25),0.681,	time, 0.3,	0,	true, 0.1,	vec4(135, 192, 245, 0.5));
	Planet* neptune =	new Planet(vec3(10.5, 0, 10.5),0.543,	time, 0.3,	0,	true, 0.09,	vec4(35, 149, 255, 0.5));

	sun->Draw();
	mercury->Draw();
	venus->Draw();
	earth->Draw();
	mars->Draw();
	jupiter->Draw();
	saturn->Draw();
	uranus->Draw();
	neptune->Draw();

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 
		getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

#pragma region SimpleShader
	// Bind the shader
	m_simpleShader.bind();

	// Bind the transform
	auto pvm = m_projectionMatrix * m_viewMatrix * m_quadTransform;
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	// Draw the quad using Mesh's draw
	m_quadMesh.Draw();

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

bool GraphicsApp::LaunchShaders()
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

	m_quadMesh.InitialiseQuad();

	// This is a 10 'unit' wide quad
	m_quadTransform = {
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1
	};

	return true;
}
