#pragma once

#include "Application.h"
#include "Shader.h"

#include "Planet.h"
#include "Mesh.h"

#include <glm/mat4x4.hpp>
#include <list>

#include "OBJMesh.h"

#include "Scene.h"
#include "Instance.h"
#include "RenderTarget.h"

#include "FlyCamera.h"
#include "OribtalCamera.h"
#include "StationaryCamera.h"

#include "ParticleEmitter.h"

class GraphicsApp : public aie::Application {
public:

	GraphicsApp();
	virtual ~GraphicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void SolarSystem(float time);
	
protected:
	bool LaunchShaders();
	void ImGUIRefesher();

	// Loaders
	bool QuadLoader();
	bool BunnyLoader();
	bool SpearLoader();
	bool BookLoader();
	bool R2D2Loader();
	bool ObjLoader(aie::OBJMesh& objMesh, glm::mat4& transform,
		float scale, const char* filepath, const char* filename,
		bool flipTexture);

	bool BoxLoading();

	// Drawers
	void QuadDraw(glm::mat4 pvm);
	void BunnyDraw(glm::mat4 pvm);
	void ObjDraw(glm::mat4 pv, glm::mat4 transform, aie::OBJMesh* objMesh);
	void PhongDraw(glm::mat4 pvm, glm::mat4 transform, float time);

	void BoxDraw(glm::mat4 pvm);

	// Mesh variables
	float m_planetSpeed;
	int m_postProcessEffect;
	bool m_toggleParticles;

	int m_numOfSpear;
	int m_numOfBunny;
	int m_numOfBook;
	int m_numOfR2D2;

	bool m_toggleSunLight;
	bool m_toggleHorizontalLight;
	bool m_toggleVerticalLight;

	// Camera Positions
	glm::vec3 m_stationaryXPos;
	glm::vec3 m_stationaryYPos;
	glm::vec3 m_stationaryZPos;
	glm::vec3 m_flyPos;

	float m_flyTheta;
	float m_flyPhi;

	// Camera selected
	bool m_stCamX;
	bool m_stCamY;
	bool m_stCamZ;
	bool m_flyCam;

	// Scene Selector
	bool m_togglePlain;
	bool m_toggleSolarsystem;
	bool m_toggleBunny;
	bool m_toggleSoulSpear;
	bool m_toggleR2D2;
	bool m_toggleBook;

	Scene*				m_scene;

	// camera transforms
	glm::mat4			m_viewMatrix;
	glm::mat4			m_projectionMatrix;

	aie::ShaderProgram	m_simpleShader;
	aie::ShaderProgram	m_colorShader;
	aie::ShaderProgram	m_phongShader;
	aie::ShaderProgram	m_texturedShader;
	aie::ShaderProgram  m_normalLitShader;
	aie::ShaderProgram  m_postProcessShader;
	aie::ShaderProgram  m_particleShader;

	aie::RenderTarget	m_renderTarget;

	std::list<Instance*>	m_instances;

	Mesh				m_quadMesh;
	Mesh				m_fullScreenQuad;
	glm::mat4			m_quadTransform;

	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTransform;

	aie::OBJMesh		m_spearMesh;
	glm::mat4			m_spearTransform;

	aie::OBJMesh		m_r2d2Mesh;
	glm::mat4			m_r2d2Transform;

	aie::OBJMesh		m_bookMesh;
	glm::mat4			m_bookTransform;
	
	BaseCamera			*m_camera;
	FlyCamera			m_flyCamera;
	OribtalCamera		m_orbitalCamera;
	StationaryCamera	m_stationaryCamera;

	glm::vec3			m_ambientLight;
	float				m_specularStrength;

	ParticleEmitter*	m_emitter;
	glm::mat4			m_particleEmitTransform;

	Light m_light;
};