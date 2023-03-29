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

	bool QuadLoader();
	void QuadDraw(glm::mat4 pvm);

	bool BunnyLoader();
	void BunnyDraw(glm::mat4 pvm);
	
	// For textured OBJs
	void ObjDraw(glm::mat4 pv, glm::mat4 transform, aie::OBJMesh* objMesh);

	bool ObjLoader(aie::OBJMesh& objMesh, glm::mat4& transform,
		float scale, const char* filepath, const char* filename,
		bool flipTexture);

	bool SpearLoader();

	void PhongDraw(glm::mat4 pvm, glm::mat4 transform, float time);

	int m_postProcessEffect;

	bool BoxLoading();
	void BoxDraw(glm::mat4 pvm);

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
	bool m_plain;
	bool m_solarsystem;
	bool m_bunny;
	bool m_soulSpear;

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

	Mesh				m_quadMesh;
	Mesh				m_fullScreenQuad;
	glm::mat4			m_quadTransform;

	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTransform;

	aie::OBJMesh		m_spearMesh;
	glm::mat4			m_spearTransform;
	
	BaseCamera			*m_camera;
	FlyCamera			m_flyCamera;
	OribtalCamera		m_orbitalCamera;
	StationaryCamera	m_stationaryCamera;

	Light				m_light;
	glm::vec3			m_ambientLight;
	float				m_specularStrength;

	ParticleEmitter*	m_emitter;
	glm::mat4			m_particleEmitTransform;
};