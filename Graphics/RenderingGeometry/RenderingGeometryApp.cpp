#include "RenderingGeometryApp.h"
#include "Shader.h"
#include "Mesh.h"
#include <glfw3.h>
#include <FlyCamera.h>
#include <vector>
#include <vec4.hpp>
#include <ext.hpp>
#include <gtc/constants.hpp>

RenderingGeometryApp::RenderingGeometryApp() {};
RenderingGeometryApp::~RenderingGeometryApp() {};

void RenderingGeometryApp::startup()
{

	m_camera = new FlyCamera();
	m_camera->setLookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	m_camera->setPerspective(glm::pi<float>() / 4, (float)_width / (float)_height, 0.1f, 1000.0f);

	m_shader = new Shader();
	m_shader->load("triShader.vert", GL_VERTEX_SHADER);
	m_shader->load("triShader.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();

	Vertex a = { glm::vec4(5.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0) };
	Vertex b = { glm::vec4(0.0f, 5.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(0) };
	Vertex c = { glm::vec4(-5.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0) };

	std::vector<Vertex> triVerts = {a, b, c};
	std::vector<unsigned int> triIndices = { 0, 1, 2 };

	m_object = new Mesh();
	m_object->initialize(triVerts, triIndices);

}

void RenderingGeometryApp::shutdown() {}

void RenderingGeometryApp::update(float time) 
{
	//CAMERA STUFF

}

void RenderingGeometryApp::draw()
{
	m_shader->bind();
	
	unsigned int projectionviewUniform = m_shader->getUniform("worldViewProjection");

	auto projView = m_camera->getProjectionView();

	glUniformMatrix4fv(projectionviewUniform, 1, GL_FALSE, glm::value_ptr(projView));

	m_object->bind();

	glDrawElements(GL_TRIANGLES, m_object->index_count, GL_UNSIGNED_INT, 0);

	m_object->unbind();
	m_shader->unbind();

}
