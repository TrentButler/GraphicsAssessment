#include "LightingApp.h"
#include <glfw3.h>
#include <Mesh.h>
#include <Shader.h>
#include <FlyCamera.h>

#include <mat4x4.hpp>
#include <vec4.hpp>
#include <ext.hpp>
#include <gtc/constants.hpp>



#pragma region 4.Ability to render a plane with predefined vertex information.
Mesh* generatePlane(int width, int height)
{
	Mesh* plane = new Mesh();

	Vertex a = { glm::vec4(0, 0, 0, 1), glm::vec4(1.0f, 1.0f, 1.0f, 0.2f), glm::vec4(0), glm::vec4(0) }; //BOTTOM LEFT
	Vertex b = { glm::vec4(width, 0, 0, 1), glm::vec4(1.0f, 1.0f, 1.0f, 0.2f), glm::vec4(0), glm::vec4(0) }; //BOTTOM RIGHT
	Vertex c = { glm::vec4(width, 0, height, 1), glm::vec4(1.0f, 1.0f, 1.0f, 0.2f), glm::vec4(0), glm::vec4(0) }; //TOP RIGHT
	Vertex d = { glm::vec4(0, 0, height, 1), glm::vec4(1.0f, 1.0f, 1.0f, 0.2f), glm::vec4(0), glm::vec4(0) }; //TOP LEFT	

	std::vector<Vertex> verts = { a, b, c, d };
	std::vector<unsigned int> indes = { 0, 1, 3, 3, 2, 1 };
	plane->initialize(verts, indes);

	return plane;
}
#pragma endregion

LightingApp::LightingApp() {}
LightingApp::~LightingApp() {}

void LightingApp::startup()
{
	m_camera = new FlyCamera();
	m_camera->setLookAt(glm::vec3(-10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	m_camera->setPerspective(glm::pi<float>() / 4, (float)_width / (float)_height, 0.1f, 10000.0f);

	m_shader = new Shader();
	m_shader->load("basicShader.vert", GL_VERTEX_SHADER);
	m_shader->load("basicShader.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();

	m_lighting = new Shader();
	m_lighting->load("lightingShader.vert", GL_VERTEX_SHADER);
	m_lighting->load("lightingShader.frag", GL_FRAGMENT_SHADER);
	m_lighting->attach();

	m_plane = generatePlane(100, 100);

	m_loadOBJ = new Mesh();
	m_loadOBJ->loadOBJ("..//[bin]//objects//stanford", "Bunny.obj");	

}

void LightingApp::shutdown()
{
}

glm::mat4 loadOBJTransform = glm::mat4(1);
void LightingApp::update(float deltaTime)
{
	//CAMERA STUFF	
	if (glfwGetMouseButton(Application::_window, 1) == true) //LOOK AT CENTER (0,0,0)
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], glm::vec4(0), glm::vec3(0, 1, 0));
	}

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

	if (glfwGetKey(Application::_window, GLFW_KEY_Q))
	{
		//MOVE UP (+Y)
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(0, 100 * deltaTime, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		m_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_E))
	{
		//MOVE DOWN (-Y)
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(0, -100 * deltaTime, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		m_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_0))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], loadOBJTransform[3], glm::vec3(0, 1, 0));
	}

	auto loadOBJTranslation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(50, 0, 50, 1)
	);
	loadOBJTransform = loadOBJTranslation;
}

void LightingApp::draw()
{
	auto viewProjection = m_camera->getProjectionView();
	unsigned int defaultVPUniform = 0;
	unsigned int lightingVPUniform = 0;

#pragma region Plane
	m_shader->bind();
	defaultVPUniform = m_shader->getUniform("WVP");
	glUniformMatrix4fv(defaultVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * glm::mat4(1)));
	m_plane->draw(GL_TRIANGLES);
	m_shader->unbind();
#pragma endregion

#pragma region LoadedObject
	m_lighting->bind();
	lightingVPUniform = m_lighting->getUniform("WVP");
	auto lightingDirUniform = m_lighting->getUniform("lightDirection");
	auto lightingColorUniform = m_lighting->getUniform("lightColor");
	auto lightingCameraUniform = m_lighting->getUniform("cameraPosition");
	auto lightingSpecularUniform = m_lighting->getUniform("specularPower");
	glUniform3fv(lightingDirUniform, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f))); // SEND THE LIGHTING SHADER THE LIGHTS DIRECTION
	glUniform3fv(lightingColorUniform, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f))); // SEND THE LIGHTING SHADER THE LIGHTS COLOR
	glUniform3fv(lightingCameraUniform, 1, glm::value_ptr(m_camera->getView()[3])); // SEND THE LIGHTING SHADER THE CAMERA'S POSITION
	glUniform1f(lightingSpecularUniform, 128.0f); // SEND THE LIGHTING SHADER A VALUE FOR THE SPECULAR POWER
	glUniformMatrix4fv(lightingVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * loadOBJTransform));
	m_loadOBJ->draw(GL_TRIANGLES);
	m_lighting->unbind();
#pragma endregion
}
