#include "RenderingGeometryApp.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
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
	m_shader->load("defaultVert.vert", GL_VERTEX_SHADER);
	m_shader->load("defaultFrag.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();

	triShader = new Shader();
	triShader->load("triShader.vert", GL_VERTEX_SHADER);
	triShader->load("triShader.frag", GL_FRAGMENT_SHADER);
	triShader->attach();

	m_texture = new Texture();
	m_texture->load("..//[bin]//textures", "jadeTexture.jpg");

	Vertex a = { glm::vec4(5.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0)};
	Vertex b = { glm::vec4(0.0f, 5.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(0) };
	Vertex c = { glm::vec4(-5.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0) };

	std::vector<Vertex> triVerts = {a, b, c};
	std::vector<unsigned int> triIndices = { 0, 1, 2 };

	m_object = new Mesh();
	m_object->initialize(triVerts, triIndices);

	loadOBJ = new Mesh();
	loadOBJ->loadOBJ("..//[bin]//objects//cat", "cat.obj");
}

void RenderingGeometryApp::shutdown() {}

static double prevMouseX = 0; //USED TO CALCULATE THE MOUSE DELTA X
static double prevMouseY = 0; //USED TO CALCULATE THE MOUSE DELTA Y
static double yaw = 0; //VARIABLE USED TO STORE THE 'YAW' AMOUNT
static double pitch = 0; //VARIABLE USED TO STORE THE 'PITCH' AMOUNT
static glm::vec2 deltaMouse = glm::vec2(0); //VECTOR2 TO STORE THE CHANGE IN MOUSE POSITION
glm::mat4 triTransform = glm::mat4(1);
glm::mat4 objTransform = glm::mat4(1);
void RenderingGeometryApp::update(float deltaTime) 
{
	//CAMERA STUFF
	//MOVE CAMERA
	static bool pressed = false;
	if (glfwGetMouseButton(Application::_window, 0) == true) //MOUSE CLICKED
	{
		if (pressed == false)
		{
			pressed = true;
			glfwGetCursorPos(Application::_window, &prevMouseX, &prevMouseY);
		}

		double newMouseX = 0;
		double newMouseY = 0;
		glfwGetCursorPos(Application::_window, &newMouseX, &newMouseY);

		deltaMouse = glm::vec2((newMouseX - prevMouseX), -(newMouseY - prevMouseY)); //CALCULATE THE CHANGE IN MOUSE POSITION
		prevMouseX = newMouseX; //STORE THE NEW VALUE OF THE MOUSE X
		prevMouseY = newMouseY; //STORE THE NEW VALUE OF THE MOUSE Y

		yaw += deltaMouse.x * 0.04; //INCREMENT THE 'YAW' VALUE BASED ON DELTA MOUSE X
		pitch += deltaMouse.y * 0.04; //INCREMENT THE 'PITCH' VALUE BASED ON DELTA MOUSE Y

		if (pitch > 89) //CLAMP PITCH VALUE BETWEEN -90 AND 90
		{
			pitch = 89;
		}
		if (pitch < -89) //CLAMP PITCH VALUE BETWEEN -90 AND 90
		{
			pitch = -89;
		}

		//auto Front = glm::vec3( //GENERATE AN FRONT DIRECTION FOR THE CAMERA USING THE YAW AND PITCH VALUES
		//	glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
		//	glm::sin(glm::radians(pitch)),
		//	glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
		//);
		//auto cameraFront = glm::normalize(Front);


		//this->_camera->setLookAt(
		//	this->_camera->getWorldTransform()[3],
		//	this->_camera->getWorldTransform()[3] + glm::vec4(cameraFront, 1),
		//	glm::vec3(0, 1, 0));

		auto YRot = glm::mat4(
		glm::vec4(cosf(deltaMouse.x * deltaTime), 0, -sinf(deltaMouse.x * deltaTime), 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(sinf(deltaMouse.x * deltaTime), 0, cosf(deltaMouse.x * deltaTime), 0),
		glm::vec4(0, 0, 0, 1)
		);

		auto XRot = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, cosf(deltaMouse.y * deltaTime), sinf(deltaMouse.y * deltaTime), 0),
		glm::vec4(0, -sinf(deltaMouse.y * deltaTime), cos(deltaMouse.y * deltaTime), 0),
		glm::vec4(0, 0, 0, 1)
		);

		objTransform = objTransform * YRot;
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
}

void RenderingGeometryApp::draw()
{
	triShader->bind();
	
	unsigned int projectionviewUniform = triShader->getUniform("worldViewProjection");
	auto projView = m_camera->getProjectionView();

	glUniformMatrix4fv(projectionviewUniform, 1, GL_FALSE, glm::value_ptr(projView * triTransform));
	m_object->bind();
	glDrawElements(GL_TRIANGLES, m_object->index_count, GL_UNSIGNED_INT, 0);
	m_object->unbind();

	triShader->unbind();


	m_shader->bind();
	
	unsigned int pvUniform = m_shader->getUniform("worldViewProjection");
	unsigned int diffuseUniform = m_shader->getUniform("diffuse");
	
	glUniformMatrix4fv(pvUniform, 1, GL_FALSE, glm::value_ptr(projView* objTransform));
	glUniform1i(diffuseUniform, 0);

	loadOBJ->bind();
	glDrawElements(GL_TRIANGLES, loadOBJ->index_count, GL_UNSIGNED_INT, 0);
	loadOBJ->unbind();

	m_shader->unbind();
}
