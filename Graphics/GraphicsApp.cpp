#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "imgui.h"



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

	m_togglePlain		= false;
	m_toggleSolarsystem = false;
	m_toggleBunny		= false;
	m_toggleSoulSpear	= false;
	m_toggleR2D2		= false;
	m_toggleBook		= false;
	m_toggleParticles	= false;

	m_toggleSunLight		= true;
	m_toggleHorizontalLight = true;
	m_toggleVerticalLight	= true;

	m_planetSpeed = 1;

	m_numOfSpear = 0;
	m_numOfBunny = 0;
	m_numOfBook = 0;
	m_numOfR2D2 = 0;


	m_specularStrength =	10.f;
	
	m_postProcessEffect =	0;

	m_emitter = new ParticleEmitter();
	m_emitter->Initialise(1000, 500, .1f, 1.0f, 
		1, 5, 1, .1f, glm::vec4(0, 0, 1, 1), glm::vec4(0, 1, 0, 1));

#pragma region PresetCamPositions
	m_stationaryXPos = glm::vec3(-10, 2, 0);
	m_stationaryYPos = glm::vec3(0, 12, 0);
	m_stationaryZPos = glm::vec3(0, 2, 10);
	m_flyPos = glm::vec3(-10, 2, 0);
	m_flyTheta = 0;
	m_flyPhi = 0;

	m_stCamX = true;
	m_stCamY = false;
	m_stCamZ = false;
	m_flyCam = false;
#pragma endregion

	m_camera = new StationaryCamera();

	Light light = Light(vec3(-5, 3, 0), vec3(0.5, 0.3, 0.1), 1); // Sun
	m_ambientLight =		{ 1.f, 1.f, 1.f };

	m_scene = new Scene(&*m_camera, glm::vec2(getWindowWidth(),
		getWindowHeight()), light, m_ambientLight);

	m_scene->AddPointLights(glm::vec3(5, 3, 0), glm::vec3(0, 0.3, 0.7), 1);
	m_scene->AddPointLights(glm::vec3(-5, 3, 0), glm::vec3(0, 0.3, 0.7), 1);
	m_scene->AddPointLights(glm::vec3(-5, 3, 0), glm::vec3(0, 0.3, 0.7), 1);

	return LaunchShaders();
}

void GraphicsApp::shutdown() {

	Gizmos::destroy();
	delete m_scene;
}

void GraphicsApp::update(float deltaTime) {

	float time = getTime();

	m_camera->update(deltaTime);

	if (m_postProcessEffect > 10)
		m_postProcessEffect = 0;
	if (m_postProcessEffect < 0)
		m_postProcessEffect = 10;

#pragma region CameraVariables
	if (m_stCamX)
		m_stationaryXPos = m_camera->GetPosition();
	if (m_stCamY)
		m_stationaryYPos = m_camera->GetPosition();
	if (m_stCamZ)
		m_stationaryZPos = m_camera->GetPosition();
	if (m_flyCam)
	{
		m_flyPos = m_camera->GetPosition();
		m_flyTheta = m_camera->GetTheta();
		m_flyPhi = m_camera->GetPhi();
	}
#pragma endregion

	// rotate camera
	//m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, 10, glm::cos(time) * 10),
	//	vec3(0), vec3(0, 1, 0));

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
	
#pragma region CreateObjects
	if (m_toggleSolarsystem) SolarSystem(time);
#pragma endregion

#pragma region CameraCylinders
	if (m_flyCam)
	{
		aie::Gizmos::addCylinderFilled(m_stationaryXPos, 0.5, 1, 10, glm::vec4(8, 126, 139, 0.5));
		aie::Gizmos::addCylinderFilled(m_stationaryYPos, 0.5, 1, 10, glm::vec4(255, 90, 95, 0.5));
		aie::Gizmos::addCylinderFilled(m_stationaryZPos, 0.5, 1, 10, glm::vec4(49, 203, 0, 0.5));
	}
	else if (m_stCamX)
	{
		aie::Gizmos::addCylinderFilled(m_flyPos, 0.5, 1, 10, glm::vec4(165, 148, 249, 0.5));
		aie::Gizmos::addCylinderFilled(m_stationaryYPos, 0.5, 1, 10, glm::vec4(255, 90, 95, 0.5));
		aie::Gizmos::addCylinderFilled(m_stationaryZPos, 0.5, 1, 10, glm::vec4(49, 203, 0, 0.5));
	}
	else if (m_stCamY)
	{
		aie::Gizmos::addCylinderFilled(m_flyPos, 0.5, 1, 10, glm::vec4(165, 148, 249, 0.5));
		aie::Gizmos::addCylinderFilled(m_stationaryXPos, 0.5, 1, 10, glm::vec4(8, 126, 0, 0.5));
		aie::Gizmos::addCylinderFilled(m_stationaryZPos, 0.5, 1, 10, glm::vec4(49, 203, 0, 0.5));
	}
	else if (m_stCamZ)
	{
		aie::Gizmos::addCylinderFilled(m_flyPos, 0.5, 1, 10, glm::vec4(165, 148, 249, 0.5));
		aie::Gizmos::addCylinderFilled(m_stationaryXPos, 0.5, 1, 10, glm::vec4(8, 126, 139, 0.5));
		aie::Gizmos::addCylinderFilled(m_stationaryYPos, 0.5, 1, 10, glm::vec4(255, 90, 95, 0.5));
	}
#pragma endregion

#pragma region ToggleInstances
	if (m_toggleSoulSpear)
	{
		m_scene->AddInstance(new Instance(glm::vec3(2 * m_numOfSpear, 0, 0), glm::vec3(0, 30 * m_numOfSpear, 0),
			glm::vec3(1), &m_spearMesh, &m_normalLitShader));
		m_toggleSoulSpear = false;
	}
	if (m_toggleBunny)
	{
		m_scene->AddInstance(new Instance(glm::vec3(2 * m_numOfBunny, 0, 0), glm::vec3(0, 30 * m_numOfBunny, 0),
			glm::vec3(1), &m_bunnyMesh, &m_normalLitShader));
		m_toggleBunny = false;
	}
	if (m_toggleR2D2)
	{
		m_scene->AddInstance(new Instance(glm::vec3(2 * m_numOfR2D2, 0, 0), glm::vec3(0, 30 * m_numOfR2D2, 0),
			glm::vec3(0.01), &m_r2d2Mesh, &m_normalLitShader));
		m_toggleR2D2 = false;
	}
	if (m_toggleBook)
	{
		m_scene->AddInstance(new Instance(glm::vec3(2 * m_numOfBook, 0, 0), glm::vec3(0, 30 * m_numOfBook, 0),
			glm::vec3(1), &m_bookMesh, &m_normalLitShader));
		m_toggleBook = false;
	}
#pragma endregion
	aie::Input* input = aie::Input::getInstance();

	ImGUIRefesher();

	// quit if we press escape
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (m_toggleParticles)
	{
		m_emitter->Update(deltaTime, m_scene->GetCamera()->GetTransform(
			m_camera->GetPosition(), glm::vec3(0), glm::vec3(1)));
	}
}

void GraphicsApp::draw() {

	float time = getTime();

	// Bind the render target as the first 
	// part of our draw function
	m_renderTarget.bind();

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = m_camera->getProjectionMatrix(getWindowWidth(), getWindowHeight()); 
	m_viewMatrix = m_camera->getViewMatrix();

	auto pv = m_projectionMatrix * m_viewMatrix;

	m_scene->Draw();

	m_particleShader.bind();
	m_particleShader.bindUniform("ProjectionViewModel", pv * m_particleEmitTransform);

	if (m_toggleParticles)
	{
		m_emitter->Draw();
	}

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// Unbind the target to return to the backbuffer
	m_renderTarget.unbind();

	clearScreen();

	// Draw the quad setup in QuadLoader()
	// QuadDraw(pv * m_quadTransform);

	if (m_toggleBunny)
	{
		//PhongDraw(pv, m_bunnyTransform, time);
	}

	// Bind the post process shader and the texture
	m_postProcessShader.bind();
	m_postProcessShader.bindUniform("colorTarget", 0);
	m_postProcessShader.bindUniform("postProcessTarget", m_postProcessEffect);
	m_postProcessShader.bindUniform("windowWidth", (int)getWindowWidth());
	m_postProcessShader.bindUniform("windowHeight", (int)getWindowHeight());
	m_postProcessShader.bindUniform("iTime", getTime());
	m_renderTarget.getTarget(0).bind(0);

	m_fullScreenQuad.Draw();
}

void GraphicsApp::SolarSystem(float time)
{
	// Creates a solar system of planets from class Planet
	Planet* sun = new Planet(vec3(0),					0,		time * m_planetSpeed, 1,	0, false, 0,	vec4(250, 255, 0,   0.5));
	Planet* mercury = new Planet(vec3(1.75, 0, 1.75),	4.787f, time * m_planetSpeed, 0.15, 0, false, 0,	vec4(189, 205, 214, 0.5));
	Planet* venus = new Planet(vec3(3, 0, 3),			3.502f, time * m_planetSpeed, 0.3,	0, false, 0,	vec4(255, 139, 19,  0.5));
	Planet* earth = new Planet(vec3(4.25, 0, 4.25),		2.978f, time * m_planetSpeed, 0.3,	0, false, 0,	vec4(0,	  146, 255, 0.5));
	Planet* mars = new Planet(vec3(5.5, 0, 5.5),		2.407f, time * m_planetSpeed, 0.2,	0, false, 0,	vec4(255, 17,  0,   0.5));
	Planet* jupiter = new Planet(vec3(6.75, 0, 6.75),	1.307f, time * m_planetSpeed, 0.4,	0, true,  0.09, vec4(242, 167, 90,  0.5));
	Planet* saturn = new Planet(vec3(8, 0, 8),			0.969,	time * m_planetSpeed, 0.4,	0, true,  0.13, vec4(255, 203, 47,  0.5)); // large
	Planet* uranus = new Planet(vec3(9.25, 0, 9.25),	0.681,	time * m_planetSpeed, 0.3,	0, true,  0.1,	vec4(135, 192, 245, 0.5));
	Planet* neptune = new Planet(vec3(10.5, 0, 10.5),	0.543,	time * m_planetSpeed, 0.3,	0, true,  0.09, vec4(35,  149, 255, 0.5));

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
	if (m_renderTarget.initialise(1, getWindowWidth(),
		getWindowHeight()) == false)
	{
		printf("Render Target Error!\n");
		return false;
	}


#pragma region LoadingShaders
	// Textured Mesh Shader
	m_normalLitShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/normalLit.vert");
	m_normalLitShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/normalLit.frag");

	if (m_normalLitShader.link() == false)
	{
		printf("Normal Lit Phong Shader Error: %s\n", m_normalLitShader.getLastError());
		return false;
	}

	// Untextured Mesh Shader
	m_phongShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/phong.frag");

	if (m_phongShader.link() == false)
	{
		printf("Phong Shader Error: %s\n", m_phongShader.getLastError());
		return false;
	}

	// Post Processing Shader
	m_postProcessShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/post.vert");
	m_postProcessShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/post.frag");

	if (m_postProcessShader.link() == false)
	{
		printf("Post Process Shader Error: %s\n", m_postProcessShader.getLastError());
		return false;
	}

	// Particle Shader
	m_particleShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/particle.vert");
	m_particleShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/particle.frag");

	if (m_particleShader.link() == false)
	{
		printf("Particle Shader Error : % s\n", m_particleShader.getLastError());
		return false;
	}
#pragma endregion

	m_particleEmitTransform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	// Create a full screen quad
	m_fullScreenQuad.InitialiseFullScreenQuad();

#pragma region LoadingMeshes
	ObjLoader(m_spearMesh, m_spearTransform, 1,	   "./soulspear/", "soulspear.obj", true);
	ObjLoader(m_bunnyMesh, m_bunnyTransform, 0.1,  "./stanford/",  "Bunny.obj",	    true);
	ObjLoader(m_r2d2Mesh,  m_r2d2Transform,	 0.01, "./r2d2/",      "R2-Unit.obj",		false);
	ObjLoader(m_bookMesh,  m_bookTransform,	 0.1,  "./book/",      "book_2.obj",	false);
#pragma endregion

	return true;
}

void GraphicsApp::ImGUIRefesher()
{
#pragma region ShaderEffects
	ImGui::Begin("Shader Effects");
	ImGui::InputInt("Post Process Effect",
		&m_postProcessEffect, 1, 2);
	ImGui::Checkbox("Particles", &m_toggleParticles);
	ImGui::End();
#pragma endregion

#pragma region MeshSelector
	ImGui::Begin("Mesh Selector");
	ImGui::Checkbox("Solar System", &m_toggleSolarsystem);
	if (m_toggleSolarsystem == true)
	{
		ImGui::DragFloat("Planet Speed",
			&m_planetSpeed, 0.1, 0, 10);
	}
	if (ImGui::Button("Soul Spear"))
	{
		m_toggleSoulSpear = true;
		m_numOfSpear++;
		// Rotate, Move, Scale
	}
	if (ImGui::Button("Bunny"))
	{
		m_toggleBunny = true;
		m_numOfBunny++;
		// Rotate, Move, Scale
	}
	if (ImGui::Button("R2D2"))
	{
		m_toggleR2D2 = true;
		m_numOfR2D2++;
		// Rotate, Move, Scale
	}
	if (ImGui::Button("Clear All"))
	{
		m_scene->ClearInstance();
		m_numOfSpear = 0;
		m_numOfBunny = 0;
		m_numOfR2D2 = 0;
		m_numOfBook = 0;
	}
	ImGui::End();
#pragma endregion

#pragma region CameraSelector
	ImGui::Begin("Camera Selector/Settings");
	if (ImGui::Button("StationaryCamera X-Axis"))
	{
		delete m_camera;
		m_camera = new StationaryCamera(true, false, false, m_stationaryXPos);
		m_scene->SetCamera(m_camera);
		m_flyCam = false;
		m_stCamX = true;
		m_stCamY = false;
		m_stCamZ = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("FlyCamera"))
	{
		delete m_camera;
		m_camera = new FlyCamera(m_flyPos, m_flyTheta, m_flyPhi);
		m_scene->SetCamera(m_camera);
		m_flyCam = true;
		m_stCamX = false;
		m_stCamY = false;
		m_stCamZ = false;
	}
	if (ImGui::Button("StationaryCamera Y-Axis"))
	{
		delete m_camera;
		m_camera = new StationaryCamera(false, true, false, m_stationaryYPos);
		m_scene->SetCamera(m_camera);
		m_flyCam = false;
		m_stCamX = false;
		m_stCamY = true;
		m_stCamZ = false;
	}
	if (ImGui::Button("StationaryCamera Z-Axis"))
	{
		delete m_camera;
		m_camera = new StationaryCamera(false, false, true, m_stationaryZPos);
		m_scene->SetCamera(m_camera);
		m_flyCam = false;
		m_stCamX = false;
		m_stCamY = false;
		m_stCamZ = true;
	}
	ImGui::End();
#pragma endregion

#pragma region LightSettings
	m_scene->ImGUI();
#pragma endregion
}

bool GraphicsApp::QuadLoader()
{
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/textured.frag");

	if (m_texturedShader.link() == false)
	{
		printf("Simple Shader has an Error %s|\n",
			m_texturedShader.getLastError());
		return false;
	}

	// Defined as 4 vertices for the 2 triangles
	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0,  0.5f, 1 };
	vertices[1].position = { 0.5f, 0,  0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	vertices[3].position = { 0.5f, 0, -0.5f, 1 };

	unsigned int indices[6] = { 0, 2, 1, 1, 2, 3 };

	m_quadMesh.Initialise(4, vertices, 6, indices);
	//m_quadMesh.InitialiseQuad();

	// This is a 10 'unit' wide quad
	m_quadTransform = {
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1
	};

	return true;
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

bool GraphicsApp::BookLoader()
{
	if (m_bookMesh.load("./book/book_2.obj", true, true) == false)
	{
		printf("Magic Book Mesh Error!\n");
		return false;
	}

	m_bookTransform = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
	};

	return true;
}

bool GraphicsApp::R2D2Loader()
{
	if (m_r2d2Mesh.load("./r2d2/r2-d2.obj", true, true) == false)
	{
		printf("R2D2 Mesh Error!\n");
		return false;
	}

	m_r2d2Transform = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
	};

	return true;
}

bool GraphicsApp::ObjLoader(aie::OBJMesh& objMesh, glm::mat4& transform,
	float scale, const char* filepath, const char* filename, bool flipTexture)
{
	std::string fileAdd = filepath;
	fileAdd += filename;

	if (objMesh.load(fileAdd.c_str(), true, flipTexture) == false)
	{
		std::string error = filename;
		error += " Mesh Error\n";
		printf(error.c_str());
		return false;
	}

	transform = {
	scale,  0,		0,		0,
	0,		scale,  0,		0,
	0,		0,		scale,  0,
	0,		0,		0,		1
	};

	return true;
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

void GraphicsApp::QuadDraw(glm::mat4 pvm)
{
	// Bind the shader
	m_texturedShader.bind();

	// Bind the transform
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);

	m_texturedShader.bindUniform("diffuseTexture", 0);

	m_renderTarget.getTarget(0).bind(0);

	// Draw the quad using Mesh's draw
	m_quadMesh.Draw();
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

void GraphicsApp::ObjDraw(glm::mat4 pv, glm::mat4 transform, aie::OBJMesh* objMesh)
{
	m_normalLitShader.bind();

	// Bind the camera position
	m_normalLitShader.bindUniform("CameraPosition",
		glm::vec3(glm::inverse(m_viewMatrix)[3]));

	// Bind the directional light we defined
	//m_normalLitShader.bindUniform("LightDirection", m_light.direction);
	//m_normalLitShader.bindUniform("LightColor", m_light.color);
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

	//if (m_toggleBunny) 
	m_bunnyMesh.draw();
	//if (m_togglePlain) m_quadMesh.Draw();
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