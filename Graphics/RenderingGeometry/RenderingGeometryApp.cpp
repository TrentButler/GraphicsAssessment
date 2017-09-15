#include "RenderingGeometryApp.h"
#include "Shader.h"
#include "Mesh.h"
#include <glfw3.h>
#include <FlyCamera.h>
#include <vector>
#include <vec4.hpp>
#include <ext.hpp>
#include <gtc/constants.hpp>


#pragma region 1.Function that generates a half circle given a number of points and radius.

#pragma endregion

#pragma region 2.Function that generates a sphere given a half circle, and number of meridians.

#pragma endregion

#pragma region 3.Function that generates indices for geometry to be rendered using triangle strips.

#pragma endregion

#pragma region 4.Ability to render a plane with predefined vertex information.
Mesh* generatePlane()
{
	Mesh* plane = new Mesh();

	Vertex a = { glm::vec4(0, 0, 0, 1), glm::vec4(0.0f, 0.0f, 1.0f, 0.2f), glm::vec4(0), glm::vec4(0) }; //BOTTOM LEFT
	Vertex b = { glm::vec4(1, 0, 0, 1), glm::vec4(0.0f, 0.0f, 1.0f, 0.2f), glm::vec4(0), glm::vec4(0) }; //BOTTOM RIGHT
	Vertex c = { glm::vec4(1, 0, 1, 1), glm::vec4(0.0f, 0.0f, 1.0f, 0.2f), glm::vec4(0), glm::vec4(0) }; //TOP RIGHT
	Vertex d = { glm::vec4(0, 0, 1, 1), glm::vec4(0.0f, 0.0f, 1.0f, 0.2f), glm::vec4(0), glm::vec4(0) }; //TOP LEFT
	

	std::vector<Vertex> verts = {a, b, c, d};
	std::vector<unsigned int> indes = {0, 1, 3, 3, 2, 1};
	plane->initialize(verts, indes);

	return plane;
}
#pragma endregion

#pragma region 5.Ability to render a cube with predefined vertex information.

Mesh* generateCube()
{
	Mesh* Cube = new Mesh();

	Vertex a = { glm::vec4(0.0f,0.0f,0.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BOTTOM LEFT
	Vertex b = { glm::vec4(1.0f,0.0f,0.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BOTTOM RIGHT
	Vertex c = { glm::vec4(1.0f,1.0f,0.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // TOP RIGHT
	Vertex d = { glm::vec4(0.0f,1.0f,0.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // TOP LEFT
	Vertex e = { glm::vec4(0.0f,0.0f, 1.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BACK BOTTOM LEFT
	Vertex f = { glm::vec4(1.0f,0.0f, 1.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BACK BACK BOTTOM RIGHT
	Vertex g = { glm::vec4(1.0f,1.0f, 1.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BACK TOP RIGHT
	Vertex h = { glm::vec4(0.0f,1.0f, 1.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BACK TOP LEFT

	std::vector<Vertex> verts = { a, b, c, d, e, f, g, h };
	std::vector<unsigned int> indes = { 
		0,1,3,
		3,1,2, //FRONT FACE

		4,5,7,
		7,5,6, //BACK FACE
		
		4,0,7,
		7,0,3, //LEFT FACE

		1,5,2,
		2,5,6, //RIGHT FACE

		3,2,7,
		7,2,6, //TOP FACE

		0,1,4,
		4,1,5  //BOTTOM FACE
	};

	Cube->initialize(verts, indes);
	return Cube;
}

#pragma endregion

#pragma region 6.Ability to render a sphere with Triangles

#pragma endregion

#pragma region 7.Ability to render the procedurally generated sphere with triangle strips.

#pragma endregion

#pragma region 8.Ability to load shaders from file using a Shader class object.


#pragma endregion

#pragma region 9.Ability to load geometry into a Mesh class object.

#pragma endregion


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

	Vertex a = { glm::vec4(5.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
	Vertex b = { glm::vec4(0.0f, 5.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) };
	Vertex c = { glm::vec4(-5.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) };

	std::vector<Vertex> triVerts = {a, b, c};
	std::vector<unsigned int> triIndices = { 0, 1, 2 };

	m_object = generatePlane();
	//m_object->initialize(triVerts, triIndices);

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
	bool pressed = false;
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
	m_shader->bind();
	
	unsigned int projectionviewUniform = m_shader->getUniform("worldViewProjection");

	auto projView = m_camera->getProjectionView();

	glUniformMatrix4fv(projectionviewUniform, 1, GL_FALSE, glm::value_ptr(projView * objTransform));

	m_object->bind();

	glDrawElements(GL_TRIANGLES, m_object->index_count, GL_UNSIGNED_INT, 0);

	m_object->unbind();
	m_shader->unbind();

}
