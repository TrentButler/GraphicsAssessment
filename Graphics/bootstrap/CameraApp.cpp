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
	

	/*static bool MousePressed = false;
	if (glfwGetMouseButton(Application::_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) //MOUSE CLICKED
	{	
		static double PrevMouseX = 0;
		static double PrevMouseY = 0;		

		if (MousePressed == false)
		{
			MousePressed = true;
			glfwGetCursorPos(Application::_window, &PrevMouseX, &PrevMouseY); // GET THE MOUSE POSITION AFTER CLICK
		}

		double NewMouseX = 0;
		double NewMouseY = 0;

		//CALCULATE MOUSE DELTA
		glfwGetCursorPos(Application::_window, &NewMouseX, &NewMouseY);

		double mouseDeltaX = NewMouseX - PrevMouseX;
		double mouseDeltaY = NewMouseY - PrevMouseY;

		PrevMouseX = NewMouseX;
		PrevMouseY = NewMouseY;

		glm::vec2 mouseDelta = glm::vec2(mouseDeltaX / 1200.f, -mouseDeltaY / 1200.f);

		auto YRot = glm::mat4(
			glm::vec4(cosf(mouseDelta.x), 0, -sinf(mouseDelta.x), 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(sinf(mouseDelta.x), 0, cosf(mouseDelta.x), 0),
			glm::vec4(0, 0, 0, 1)
		);

		auto XRot = glm::mat4(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, cosf(mouseDelta.y), sinf(mouseDelta.y), 0),
			glm::vec4(0, -sinf(mouseDelta.y), cos(mouseDelta.y), 0),
			glm::vec4(0, 0, 0, 1)
		);

		auto World = this->_camera->getWorldTransform();

		auto newWorld = World * YRot;
		//auto newWorld = YRot * World;
		
		system("cls");
		cout << "\n" << mouseDelta.x << mouseDelta.y;

		this->_camera->setWorldTransform(newWorld);
	}
*/

	static bool pressed = false;
	if (glfwGetMouseButton(Application::_window, 0) == true)
	{
		if (pressed == false)
		{
			pressed = true;
			glfwGetCursorPos(Application::_window, &prevMouseX, &prevMouseY);
		}

		//auto mousePos = 
		//deltaMouse = glm::vec2()
	}

	//MOVE CAMERA
	if (glfwGetKey(Application::_window, GLFW_KEY_W))
	{
		//MOVE FOWARD (+Z)
		//GET PREVIOUS POSITION, TRANSLATE, THEN SET IT
		auto Previous = this->_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, -0.5, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		this->_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_S))
	{
		//MOVE BACKWARD (-Z)
		auto Previous = this->_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0.5f, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		this->_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_A))
	{
		//MOVE LEFT (+X)
		auto Previous = this->_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(-0.5, 0, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		this->_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_D))
	{
		//MOVE RIGHT (-X)
		auto Previous = this->_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(0.5, 0, 0, 1));

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