#include "CameraApp.h"
#include "FPSCamera.h"
#include <glfw3.h>
#include "Gizmos.h"

#include <iostream>
using namespace std;


CameraApp::CameraApp() {};
CameraApp::~CameraApp() {};
void CameraApp::run(const char* title, unsigned int width, unsigned int height, bool fullscreen) {};

void CameraApp::startup()
{
	//INITLIZE CAMERA
	this->_camera = new FPSCamera();
	this->_camera->setLookAt(glm::vec3(0.1f, 0.1f, -10.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	this->_camera->setPerspective(3.14f / 4.0f, 16.0f / 9.0f, 0.1f, 100.0f);
	//this->_camera->setOrtho(0.0f, 1.50f, 1.50f, 0.0f, 100.0f, 0.1f);

	Gizmos::create();
}

void CameraApp::shutdown() {};

static double prevMouseX = 0;
static double prevMouseY = 0;
static glm::vec2 deltaMouse = glm::vec2(0);
glm::mat4 sphere = glm::mat4(1);
glm::mat4 biggerSphere = glm::mat4(1);
void CameraApp::update(float deltaTime)
{
	//NEEDS WORK
	//LOOK AROUND
	//DERIVE A DELTA FROM MOUSE MOVEMENT
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

		deltaMouse = glm::vec2((newMouseX - prevMouseX) / 1200, -(newMouseY - prevMouseY) / 1200);

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

		auto World = this->_camera->getWorldTransform();

		auto newWorld = World * YRot;
		//auto newWorld = YRot * World;

		this->_camera->setWorldTransform(newWorld);
		std::system("cls");
		std::cout << "<" << deltaMouse.x << "," << deltaMouse.y << ">";

	}

	//MOVE CAMERA
	if (glfwGetKey(Application::_window, GLFW_KEY_W))
	{
		//MOVE FOWARD (-Z)
		//GET PREVIOUS POSITION, TRANSLATE, THEN SET IT
		auto Previous = this->_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, -4 * deltaTime, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		this->_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_S))
	{
		//MOVE BACKWARD (+Z)
		auto Previous = this->_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 4 * deltaTime, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		this->_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_A))
	{
		//MOVE LEFT (-X)
		auto Previous = this->_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(-4 * deltaTime, 0, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		this->_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_D))
	{
		//MOVE RIGHT (+X)
		auto Previous = this->_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(4 * deltaTime, 0, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		this->_camera->setWorldTransform(newWorld);
	}


	//CHANGE CURRENT PROJECTION
	if (glfwGetKey(Application::_window, GLFW_KEY_F1))
	{
		//CHANGE PROJECTION
		this->_camera->setOrtho(0.0f, 2.0f, 2.0f, 0.0f, 100.0f, 0.1f);

	}

	if (glfwGetKey(Application::_window, GLFW_KEY_F2))
	{
		this->_camera->setPerspective(3.14f / 4.0f, 16.0f / 9.0f, 0.1f, 100.0f);
	}

	//LOOK AT A OBJECT
	if (glfwGetKey(Application::_window, GLFW_KEY_1))
	{
		this->_camera->setLookAt(this->_camera->getView()[3], sphere[3], glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_2))
	{
		this->_camera->setLookAt(this->_camera->getView()[3], biggerSphere[3], glm::vec3(0, 1, 0));
	}

	auto Translation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(10, 0, -12, 1));
	biggerSphere = sphere * Translation;

}


void CameraApp::draw()
{
	Gizmos::clear();

	Gizmos::addSphere(sphere[3], 1, 10, 10, glm::vec4(0.0f, 1.0f, 0.0f, 0.4f), &sphere);
	Gizmos::addTransform(sphere, 4);

	Gizmos::addSphere(biggerSphere[3], 2, 10, 10, glm::vec4(0.0f, 0.0f, 1.0f, 0.4f), &biggerSphere);
	Gizmos::addTransform(sphere, 4);

	//Gizmos::addTri(glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::bvec3(1, 0, 0), glm::vec4(0.0f, 0.8f, 0.0f, 0.6f));


	Gizmos::draw(this->_camera->getProjectionView());
	/*Gizmos::draw(glm::mat4(1));
	auto View = this->_camera->getView();*/
}