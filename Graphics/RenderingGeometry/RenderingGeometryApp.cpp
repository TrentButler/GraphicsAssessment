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

	Vertex a = { glm::vec4(5.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)};
	Vertex b = { glm::vec4(0.0f, 5.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)};
	Vertex c = { glm::vec4(-5.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)};

	std::vector<Vertex> triVerts = {a, b, c};
	std::vector<unsigned int> triIndices = { 0, 1, 2 };

	m_object = new Mesh();
	m_object->initialize(triVerts, triIndices);

	cat = new Mesh();
	cat->loadOBJ("cat.obj");

	/*human = new Mesh();
	human->loadOBJ("MaleLow.obj");*/
}

void RenderingGeometryApp::shutdown() {}

glm::mat4 triTransform = glm::mat4(1);
glm::mat4 catTransform = glm::mat4(1);
void RenderingGeometryApp::update(float deltaTime) 
{
	//CAMERA STUFF
	//MOVE CAMERA

	if (glfwGetKey(Application::_window, GLFW_KEY_W))
	{
		//MOVE FOWARD (-Z)
		//GET PREVIOUS POSITION, TRANSLATE, THEN SET IT
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, -100 * deltaTime, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		m_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_S))
	{
		//MOVE BACKWARD (+Z)
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 100 * deltaTime, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		m_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_A))
	{
		//MOVE LEFT (-X)
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(-100 * deltaTime, 0, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		m_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_D))
	{
		//MOVE RIGHT (+X)
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(100 * deltaTime, 0, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		m_camera->setWorldTransform(newWorld);
	}

	auto T = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(10, 0, 0, 1)
		);

	catTransform = T * 0.004;
}

void RenderingGeometryApp::draw()
{
	m_shader->bind();
	
	unsigned int projectionviewUniform = m_shader->getUniform("worldViewProjection");
	auto projView = m_camera->getProjectionView();

	glUniformMatrix4fv(projectionviewUniform, 1, GL_FALSE, glm::value_ptr(projView * triTransform));
	m_object->bind();
	glDrawElements(GL_TRIANGLES, m_object->index_count, GL_UNSIGNED_INT, 0);
	m_object->unbind();

	glUniformMatrix4fv(projectionviewUniform, 1, GL_FALSE, glm::value_ptr(projView * catTransform));
	cat->bind();
	glDrawElements(GL_TRIANGLE_STRIP, cat->index_count, GL_UNSIGNED_INT, 0);
	cat->unbind();

	m_shader->unbind();
}
